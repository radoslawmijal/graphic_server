#include <iostream>
#include <sstream>
#include <cmath>
#include <unistd.h>
#include "Interp4Rotate.hh"


using std::cout;
using std::endl;


extern "C" {
  AbstractInterp4Command* CreateCmd(void);
  const char* GetCmdName() { return "Rotate"; }
}




/*!
 * \brief
 *
 *
 */
AbstractInterp4Command* CreateCmd(void)
{
  return Interp4Rotate::CreateCmd();
}


/*!
 *
 */
Interp4Rotate::Interp4Rotate(): _Angle_speed(0), _Angle_degree(0)
{}


/*!
 *
 */
void Interp4Rotate::PrintCmd() const
{
  /*
   *  Tu trzeba napisać odpowiednio zmodyfikować kod poniżej.
   */
  cout << GetCmdName() << " " << _Obj_name << " " << _Axis_name << " (axis name) " << _Angle_speed << " (speed value) " << _Angle_degree << " (degree value) " << endl;
}


/*!
 *
 */
const char* Interp4Rotate::GetCmdName() const
{
  return ::GetCmdName();
}


/*!
 *
 */
bool Interp4Rotate::ExecCmd( AbstractScene      &rScn, 
                               AbstractComChannel *pComChann
                             )
{
    AbstractMobileObj* pObj = rScn.FindMobileObj(_Obj_name.c_str());
    
    // error if object not found
    if (pObj == nullptr) {
        std::cerr << "Blad: Nie znaleziono obiektu: " << _Obj_name << std::endl;
        return false;
    }

    // animation parameters
    const int FPS = 50;
    double TotalTime_s = std::abs(_Angle_degree / _Angle_speed);
    int NSteps = static_cast<int>(TotalTime_s * FPS);
    if (NSteps < 1) NSteps = 1; 

    double stepAngle = _Angle_degree / NSteps;
    int TimePerStep_us = static_cast<int>(1e6 / FPS);

    for (int i = 0; i < NSteps; ++i) {

        // perform rotation
        if (_Axis_name == "OX") {
            double current = pObj->GetAng_Roll_deg();
            pObj->SetAng_Roll_deg(current + stepAngle);
        } else if (_Axis_name == "OY") {
            double current = pObj->GetAng_Pitch_deg();
            pObj->SetAng_Pitch_deg(current + stepAngle);
        } else if (_Axis_name == "OZ") {
            double current = pObj->GetAng_Yaw_deg();
            pObj->SetAng_Yaw_deg(current + stepAngle);
        } else {
            std::cerr << "Blad: Nieznana os obrotu: " << _Axis_name << std::endl;
            return false;
        }

        // prepare command to send to server
        std::ostringstream cmdStream;
        const Vector3D &pos = pObj->GetPosition_m();
        
        cmdStream << "UpdateObj Name=" << _Obj_name 
                  << " Shift=(" << pos[0] << "," << pos[1] << "," << pos[2] << ")"
                  << " RotXYZ_deg=(" << pObj->GetAng_Roll_deg() << "," 
                                     << pObj->GetAng_Pitch_deg() << "," 
                                     << pObj->GetAng_Yaw_deg() << ")\n";
        
        std::string command = cmdStream.str();

        // send command to server
        {
            std::lock_guard<std::mutex> Lock(pComChann->UseGuard());
            int socket = pComChann->GetSocket();

            if (write(socket, command.c_str(), command.length()) < 0) {
                std::cerr << "Blad wysylania polecenia Rotate do serwera!" << std::endl;
                return false;
            }
        }

        usleep(TimePerStep_us); 
    }

    return true;
}


/*!
 *
 */
bool Interp4Rotate::ReadParams(std::istream& Strm_CmdsList)
{
  if (!(Strm_CmdsList >> _Obj_name >> _Axis_name >> _Angle_speed >> _Angle_degree)) {
      cout << "Blad wczytywania parametrow Rotate!" << endl;
      return false;
  }
  return true;
}


/*!
 *
 */
AbstractInterp4Command* Interp4Rotate::CreateCmd()
{
  return new Interp4Rotate();
}


/*!
 *
 */
void Interp4Rotate::PrintSyntax() const
{
  cout << "   Rotate  Nazwa_Obiektu Oś Prędkosc_Kątowa[◦/s]  Kąt[◦]" << endl;
}
