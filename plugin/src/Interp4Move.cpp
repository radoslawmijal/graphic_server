#include <iostream>
#include "Interp4Move.hh"
#include <unistd.h>
#include <cmath>
#include <sstream>
#include <cstring>


using std::cout;
using std::endl;


extern "C" {
  AbstractInterp4Command* CreateCmd(void);
  const char* GetCmdName() { return "Move"; }
}




/*!
 * \brief
 *
 *
 */
AbstractInterp4Command* CreateCmd(void)
{
  return Interp4Move::CreateCmd();
}


/*!
 *
 */
Interp4Move::Interp4Move():_Speed_mmS(0), _Distance_mm(0)
{}


/*!
 *
 */
void Interp4Move::PrintCmd() const
{
  /*
   *  Tu trzeba napisać odpowiednio zmodyfikować kod poniżej.
   */
  cout << GetCmdName() << " " << _Obj_name << " " << _Speed_mmS  << " (speed value) " << _Distance_mm << " (distance value) " << endl;
}


/*!
 *
 */
const char* Interp4Move::GetCmdName() const
{
  return ::GetCmdName();
}


/*!
 *
 */
bool Interp4Move::ExecCmd(AbstractScene &rScn, AbstractComChannel *pComChann) 
{
    AbstractMobileObj* pObj = rScn.FindMobileObj(_Obj_name.c_str());
    
    // if object not found
    if (pObj == nullptr) {
        std::cerr << "Blad: Nie znaleziono obiektu: " << _Obj_name << std::endl;
        return false;
    }

    // get data in proper units
    Vector3D currentPos = pObj->GetPosition_m();
    
    double angle_yaw_rad = pObj->GetAng_Yaw_deg() * M_PI / 180.0;
    double angle_pitch_rad = pObj->GetAng_Pitch_deg() * M_PI / 180.0;

    // update position
    currentPos[0] += _Distance_mm * cos(angle_pitch_rad) * cos(angle_yaw_rad);
    currentPos[1] += _Distance_mm * cos(angle_pitch_rad) * sin(angle_yaw_rad);
    currentPos[2] += _Distance_mm * sin(angle_pitch_rad);
    
    pObj->SetPosition_m(currentPos);

    // prepare command
    std::ostringstream cmdStream;
    cmdStream << "UpdateObj Name=" << _Obj_name 
              << " Shift=(" << currentPos[0] << "," << currentPos[1] << "," << currentPos[2] << ")"
              << " RotXYZ_deg=(" << pObj->GetAng_Roll_deg() << "," 
                                 << pObj->GetAng_Pitch_deg() << "," 
                                 << pObj->GetAng_Yaw_deg() << ")\n";
    
    std::string command = cmdStream.str();

    // send command to server
    {
        std::lock_guard<std::mutex> Lock(pComChann->UseGuard());
        int socket = pComChann->GetSocket();
        
        if (write(socket, command.c_str(), command.length()) < 0) {
            std::cerr << "Blad wysylania polecenia do serwera!" << std::endl;
            return false;
        }
    }

    usleep(100000);

    return true;
}


/*!
 *
 */
bool Interp4Move::ReadParams(std::istream& Strm_CmdsList)
{
  if (!(Strm_CmdsList >> _Obj_name >> _Speed_mmS >> _Distance_mm)) {
      cout << "Blad wczytywania parametrow Move!" << endl;
      return false;
  }
  return true;
}


/*!
 *
 */
AbstractInterp4Command* Interp4Move::CreateCmd()
{
  return new Interp4Move();
}


/*!
 *
 */
void Interp4Move::PrintSyntax() const
{
  cout << "   Move  Nazwa_Obiektu  Szybkosc[mm/s]  Dlugosc_Drogi[mm]" << endl;
}
