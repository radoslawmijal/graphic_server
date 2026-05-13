#include <iostream>
#include <xercesc/util/PlatformUtils.hpp>
#include <xercesc/sax2/SAX2XMLReader.hpp>
#include <xercesc/sax2/XMLReaderFactory.hpp>
#include <xercesc/sax2/DefaultHandler.hpp>
#include <xercesc/util/XMLString.hpp>

#include "AbstractInterp4Command.hh"
#include "Set4LibInterfaces.hh"
#include "ProgramInterpreter.hh"
#include "Configuration.hh"
#include "xmlinterp.hh" 

using namespace std;
using namespace xercesc; 


bool ReadFile(const char* sFileName, Configuration &rConfig, ProgramInterpreter &rInterp)
{
  // Initialize the XML
   try {
            XMLPlatformUtils::Initialize();
   }
   catch (const XMLException& toCatch) {
            char* message = XMLString::transcode(toCatch.getMessage());
            cerr << "Error during initialization! :\n";
            cerr << "Exception message is: \n"
                 << message << "\n";
            XMLString::release(&message);
            return false;
   }

   // Create parser object
   SAX2XMLReader* pParser = XMLReaderFactory::createXMLReader();

   pParser->setFeature(XMLUni::fgSAX2CoreNameSpaces, true);
   pParser->setFeature(XMLUni::fgSAX2CoreValidation, true);
   pParser->setFeature(XMLUni::fgXercesDynamic, false);
   pParser->setFeature(XMLUni::fgXercesSchema, true);
   pParser->setFeature(XMLUni::fgXercesSchemaFullChecking, true);
   pParser->setFeature(XMLUni::fgXercesValidationErrorAsFatal, true);

   DefaultHandler* pHandler = new XMLInterp4Config(rConfig, &rInterp);
   pParser->setContentHandler(pHandler);
   pParser->setErrorHandler(pHandler);

   // Parse the XML file
   try {
     if (!pParser->loadGrammar("../config/config.xsd", xercesc::Grammar::SchemaGrammarType, true)) {
       cerr << "!!! Plik ../config/config.xsd nie moze zostac wczytany." << endl;
       return false;
     }
     pParser->setFeature(XMLUni::fgXercesUseCachedGrammarInParse, true);
     pParser->parse(sFileName);
   }
   catch (const XMLException& Exception) {
            char* sMessage = XMLString::transcode(Exception.getMessage());
            cerr << "Informacja o wyjatku: \n" << sMessage << "\n";
            XMLString::release(&sMessage);
            return false;
   }
   catch (const SAXParseException& Exception) {
            char* sMessage = XMLString::transcode(Exception.getMessage());
            char* sSystemId = xercesc::XMLString::transcode(Exception.getSystemId());
            cerr << "Blad! " << endl
                 << "    Plik:  " << sSystemId << endl
                 << "   Linia: " << Exception.getLineNumber() << endl
                 << " Informacja: " << sMessage << endl;
            XMLString::release(&sMessage);
            XMLString::release(&sSystemId);
            return false;
   }
   catch (...) {
            cout << "Zgloszony zostal nieoczekiwany wyjatek!\n" ;
            return false;
   }

   delete pParser;
   delete pHandler;
   return true;
}

int main(int argc, char* argv[])
{
  // Get args
  if (argc < 3) {
      cerr << "Uzycie: ./interpreter plik_komend.cmd plik_konfiguracyjny.xml" << endl;
      return 1;
  }

  Configuration Config;
  ProgramInterpreter progInterp(Config.Scn, Config.LibManager);

  // connect to graphics server
  if (!progInterp.ConnectToServer("127.0.0.1", 6217)) {
      cerr << "Nie udalo sie polaczyc z serwerem graficznym!" << endl;
      return 1;
  }

  // read configuration file
  if (!ReadFile(argv[2], Config, progInterp)) {
      return 1;
  }

  // execute program
  progInterp.ExecProgram(argv[1]);

  return 0;
}