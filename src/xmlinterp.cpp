#include <xercesc/util/PlatformUtils.hpp>
#include "xmlinterp.hh"
#include <cassert>
#include <sstream>
#include <cstdlib>
#include <iostream>
#include "Cuboid.hh"
#include "geomVector.hh"
#include <unistd.h>


using namespace std;


/*!
 * Konstruktor klasy. Tutaj należy zainicjalizować wszystkie
 * dodatkowe pola.
 */
XMLInterp4Config::XMLInterp4Config(Configuration &rConfig, AbstractComChannel *pComChann) : _config(rConfig), _pComChann(pComChann)
{
}


/*!
 * Metoda wywoływana jest bezpośrednio przed rozpoczęciem
 * przetwarzana dokumentu XML.
 */
void XMLInterp4Config::startDocument()
{
  cout << "*** Rozpoczecie przetwarzania dokumentu XML." << endl;
}


/*!
 * Metoda wywoływana jest bezpośrednio po zakończeniu
 * przetwarzana dokumentu XML.
 */
void XMLInterp4Config::endDocument()
{
  cout << "=== Koniec przetwarzania dokumentu XML." << endl;
}





/*!
 * Analizuje atrybuty elementu XML \p "Lib" i odpowiednio je interpretuje.
 * \param[in] rAttrs - atrybuty elementu XML \p "Lib".
 */
void XMLInterp4Config::ProcessLibAttrs(const xercesc::Attributes  &rAttrs)
{
 if (rAttrs.getLength() != 1) {
      cerr << "Zla ilosc atrybutow dla \"Lib\"" << endl;
      exit(1);
 }

 char* sParamName = xercesc::XMLString::transcode(rAttrs.getQName(0));

 if (strcmp(sParamName,"Name")) {
      cerr << "Zla nazwa atrybutu dla Lib" << endl;
      exit(1);
 }         

 XMLSize_t  Size = 0;
 char* sLibName = xercesc::XMLString::transcode(rAttrs.getValue(Size));

 std::string LibPath = "../build/plugin/" + std::string(sLibName);

 cout << "  Szukam wtyczki w: " << LibPath << endl;

 if (!_config.LibManager.AddPlugin(LibPath)) {
     cerr << "  BLAD: Nie udalo sie zaladowac biblioteki: " << sLibName << endl;
 } else {
     cout << "  Biblioteka zaladowana poprawnie." << endl;
 }

 xercesc::XMLString::release(&sParamName);
 xercesc::XMLString::release(&sLibName);
}


/*!
 * Analizuje atrybuty. Sprawdza czy ich nazwy są poprawne. Jeśli tak,
 * to pobiera wartości atrybutów (w postaci napisów) i przekazuje ...
 * \param[in] rAttrs - atrybuty elementu XML \p "Cube".
 */
void XMLInterp4Config::ProcessCubeAttrs(const xercesc::Attributes  &rAttrs)
{
 if (rAttrs.getLength() < 1) {
      cerr << "Zla ilosc atrybutow dla \"Cube\"" << endl;
      exit(1);
 }
    Cuboid *newObj = new Cuboid();
    cout << " Atrybuty:" << endl; 

    for(XMLSize_t i = 0; i < rAttrs.getLength(); i++) {
        
        char* sAttrName = xercesc::XMLString::transcode(rAttrs.getQName(i));
        char* sAttrValue = xercesc::XMLString::transcode(rAttrs.getValue(i));
        cout << "     " << sAttrName << " = \"" << sAttrValue << "\"" << endl;

        string attrName = sAttrName;
        istringstream IStrm(sAttrValue);

        if (attrName == "Name") {
            newObj->SetName(sAttrValue);
        }
        else if (attrName == "Scale") {
             Vector3D scale;
             IStrm >> scale;
             if (!IStrm.fail()) newObj->SetScale(scale);
        }
        else if (attrName == "Shift") {
             Vector3D shift;
             IStrm >> shift;
             newObj->SetShift(shift);
        }
        else if (attrName == "Trans_m") {
             Vector3D trans;
             IStrm >> trans;
             newObj->SetPosition_m(trans);
        }
        else if (attrName == "RotXYZ_deg") {
             Vector3D rot;
             IStrm >> rot;
             newObj->SetAng_Roll_deg(rot[0]);
             newObj->SetAng_Pitch_deg(rot[1]);
             newObj->SetAng_Yaw_deg(rot[2]);
        }
        else if (attrName == "RGB") {
             Vector3D rgb;
             int r, g, b;
             IStrm >> r >> g >> b;
             rgb[0]=r; rgb[1]=g; rgb[2]=b;
             newObj->SetRGB(rgb);
        }


        xercesc::XMLString::release(&sAttrName);
        xercesc::XMLString::release(&sAttrValue);
    }
    cout << endl; 

    _config.Scn.AddMobileObj(newObj);

    if (_pComChann != nullptr) {
        std::ostringstream cmdStream;
        
        // get parameters of the new object
        Vector3D pos = newObj->GetPosition_m();
        Vector3D scale = newObj->GetScale(); 
        Vector3D rgb = newObj->GetRGB();

        // AddObj command
        cmdStream << "AddObj Name=" << newObj->GetName()
                  << " Type=Cuboid" 
                  << " RGB=(" << (int)rgb[0] << "," << (int)rgb[1] << "," << (int)rgb[2] << ")"
                  << " Scale=(" << scale[0] << "," << scale[1] << "," << scale[2] << ")"
                  << " Shift=(" << pos[0] << "," << pos[1] << "," << pos[2] << ")"
                  << " RotXYZ_deg=(" << newObj->GetAng_Roll_deg() << "," 
                                     << newObj->GetAng_Pitch_deg() << "," 
                                     << newObj->GetAng_Yaw_deg() << ")\n";

        std::string command = cmdStream.str();
        
        // send the command to the server
        std::lock_guard<std::mutex> Lock(_pComChann->UseGuard());
        ssize_t bytesSent = write(_pComChann->GetSocket(), command.c_str(), command.length());
        
        if (bytesSent < 0) {
            cerr << "!!! Blad wysylania AddObj dla obiektu: " << newObj->GetName() << endl;
        }
    }
}


/*!
 * Wykonuje operacje związane z wystąpieniem danego elementu XML.
 * W przypadku elementu \p "Action" będzie to utworzenie obiektu
 * reprezentującego odpowiednią działanie robota.
 * W przypadku elementu \p "Parameter" będzie to zapisanie parametrów
 * związanych z danym działaniem.
 * \param[in] rElemName - nazwa elementu XML.
 * \param[in] rAttrs - atrybuty napotkanego elementu XML.
 */
void XMLInterp4Config::WhenStartElement( const std::string           &rElemName,
		                         const xercesc::Attributes   &rAttrs
                                       )
{
  if (rElemName == "Lib") {
    ProcessLibAttrs(rAttrs);   return;   
  }

  if (rElemName == "Cube") {
    ProcessCubeAttrs(rAttrs);  return;
  }
}




/*!
 * Metoda jest wywoływana po napotkaniu nowego elementu XML, np.
 * gdy zostanie napotkany element:
   \verbatim
     <Action Name="Rotate">
   \endverbatim
 *  to poprzez parametr \e xscElemName jest dostęp do nazwy elementu
 *  tzn. \p Action, 
 *  \param[in] pURI - (Uniform Resource Identifier) jeżeli nie jest on wyspecyfikowany
 *                    poprzez użycie atrybutów \p xmlns (o ile jego użycie jest przewidziane w gramatyce,
 *                    w tym przykładzie nie jest to przewidziane), to będzie to napis pusty.
 *  \param[in] pLocalName -  umożliwia dostęp do nazwy elementu XML.
 *                 W podanym przykładzie nazwą elementu XML jest "Action".
 *  \param[in] pQName - pełna kwalifikowana nazwa. To ma znaczenie, gdy użyta jest przestrzeń nazwa.
 *                      Wówczas poprzez ten parametr można otrzymać nazwę elementu wraz z prefiksem
 *                      przestrzeni nazwa. Jeśli przestrzeń nazw nie jest użyta (taka jak w tym
 *                      przykładzie), to \p pQName i \p pLocalName dostaczają identyczne napisy.
 *  \param[in] rAttrs -  lista atrybutów danego symbolu XML.
 *                 W przykładzie pokazanym powyżej listę atrybutów
 *                 będą stanowiły napisy:
 */
/*
 * Te metode nalezy odpowiednio zdekomponowac!!!
 */
void XMLInterp4Config::startElement(  const   XMLCh* const            pURI,
                                      const   XMLCh* const            pLocalName,
                                      const   XMLCh* const            pQName,
				      const   xercesc::Attributes&    rAttrs
                                    )
{
  char* sElemName = xercesc::XMLString::transcode(pLocalName);
  cout << "+++ Poczatek elementu: "<< sElemName << endl;

  WhenStartElement(sElemName, rAttrs);

  xercesc::XMLString::release(&sElemName);
}




/*!
 * Metoda zostaje wywołana w momencie zakończenia danego elementu
 * XML, np. jeżeli w pliku jest wpis:
   \verbatim
     <Lib Name="Rotate">
     </Lib>
   \endverbatim
 * to metoda ta zostanie wywołana po napotkaniu znacznika:
 * \verbatim</Lib>\endverbatim
 * Jeżeli element XML ma formę skróconą, tzn.
   \verbatim
     <Parametr Name="Rotate"/>
   \endverbatim
 * to wówczas wywołana metoda wywołana zostanie w momencie
 * napotkania sekwencji "/>"
 *  \param[in] pURI - (Uniform Resource Identifier) jeżeli nie jest on wyspecyfikowany
 *                    poprzez użycie atrybutów \p xmlns (o ile jego użycie jest przewidziane w gramatyce,
 *                    w tym przykładzie nie jest to przewidziane), to będzie to napis pusty.
 *  \param[in] pLocalName -  umożliwia dostęp do nazwy elementu XML.
 *                 W podanym przykładzie nazwą elementu XML jest "Lib".
 *  \param[in] pQName - pełna kwalifikowana nazwa. To ma znaczenie, gdy użyta jest przestrzeń nazwa.
 *                      Wówczas poprzez ten parametr można otrzymać nazwę elementu wraz z prefiksem
 *                      przestrzeni nazwa. Jeśli przestrzeń nazw nie jest użyta (taka jak w tym
 *                      przykładzie), to \p pQName i \p pLocalName dostaczają identyczne napisy.
 *                      
 */
void XMLInterp4Config::endElement(  const   XMLCh* const    pURI,
                                    const   XMLCh* const    pLocalName,
                                    const   XMLCh* const    pQName
                                 )
{
   char* sURI =  xercesc::XMLString::transcode(pURI);
   char* sElemName = xercesc::XMLString::transcode(pLocalName);
   char* sQName =  xercesc::XMLString::transcode(pQName);
   cout << "   URI: " << sURI << endl;
   cout << " QName: " << sQName << endl;
   cout << "----- Koniec elementu: "<< sElemName << endl;

   xercesc::XMLString::release(&sElemName);
}




/*!
 * Metoda wywoływana jest, gdy napotkany zostanie błąd fatalny,
 * np.
  \verbatim
    <Action Name="Rotate">
    </Parametr>
  \endverbatim
 * \param[in] rException - zawiera informacje dotyczące błędu w dokumencie,
 *                         linii, kolumny itp.
 */
void XMLInterp4Config::fatalError(const xercesc::SAXParseException&  rException)
{
   char* sMessage = xercesc::XMLString::transcode(rException.getMessage());
   char* sSystemId = xercesc::XMLString::transcode(rException.getSystemId());

   cerr << "Blad fatalny! " << endl
        << "    Plik:  " << sSystemId << endl
        << "   Linia: " << rException.getLineNumber() << endl
        << " Kolumna: " << rException.getColumnNumber() << endl
        << " Informacja: " << sMessage 
        << endl;

   xercesc::XMLString::release(&sMessage);
   xercesc::XMLString::release(&sSystemId);
}

/*!
 * Metoda jest wywoływana, gdy napotkany zostanie błąd, który nie
 * jest traktowany jako fatalny. W ten sposób traktowane są błędy
 * występujące w opisie gramatyki dokumentu.
 * \param[in] rException - zawiera informacje dotyczące błędu. Informacje
 *                     te to opis błędu oraz numer linii, w której
 *                     wystąpił błąd.
 */
void XMLInterp4Config::error(const xercesc::SAXParseException&  rException)
{
  cerr << "Blad!" << endl;
  fatalError(rException);
}


/*!
 * Metoda wywołana jest w przypadku błędów, które nie są fatalne
 * i mogą być traktowane jako ostrzeżenia.
 * \param[in] rException - zawiera informacje dotyczące błędu w dokumencie,
 *                         linii, kolumny itp.
 */
void XMLInterp4Config::warning(const xercesc::SAXParseException&  rException)  
{
  char* sMessage = xercesc::XMLString::transcode(rException.getMessage());
  cerr << "Ostrzezenie XML: " << sMessage << " (linia: " << rException.getLineNumber() << ")" << endl;
  xercesc::XMLString::release(&sMessage);
}

