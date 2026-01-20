#include <iostream>
#include "Interp4Set.hh"
#include <sstream>
#include <unistd.h>
#include <Vector3D.hh>


using std::cout;
using std::endl;


extern "C" {
  AbstractInterp4Command* CreateCmd(void);
  const char* GetCmdName() { return "Set"; }
}




/*!
 * \brief
 *
 *
 */
AbstractInterp4Command* CreateCmd(void)
{
  return Interp4Set::CreateCmd();
}


/*!
 *
 */
Interp4Set::Interp4Set(): _ObjName(""), _x_pos(0), _y_pos(0), _z_pos(0), _rot_x(0), _rot_y(0), _rot_z(0)
{}


/*!
 *
 */
void Interp4Set::PrintCmd() const
{
  /*
   *  Tu trzeba napisać odpowiednio zmodyfikować kod poniżej.
   */
  cout << GetCmdName() << " " << _ObjName << " " << _x_pos << " (x pos value) " << _y_pos << " (y pos value) " << _z_pos << " (z pos value) "
  << _rot_x << " (x rot value) " << _rot_y << " (y rot value) " << _rot_z << " (z rot value) " << endl;
}


/*!
 *
 */
const char* Interp4Set::GetCmdName() const
{
  return ::GetCmdName();
}


/*!
 *
 */
bool Interp4Set::ExecCmd( AbstractScene      &rScn, 
			   AbstractComChannel *pComChann
			 )
{
  // get object by name
  AbstractMobileObj* pObj = rScn.FindMobileObj(_ObjName.c_str());
  if (pObj == nullptr) {
      std::cerr << "Blad: Nie znaleziono obiektu: " << _ObjName << std::endl;
      return false;
  }

  // get new position and orientation
  Vector3D newPos;
  newPos[0] = _x_pos;
  newPos[1] = _y_pos;
  newPos[2] = _z_pos;
  pObj->SetPosition_m(newPos);
  pObj->SetAng_Roll_deg(_rot_x);
  pObj->SetAng_Pitch_deg(_rot_y);
  pObj->SetAng_Yaw_deg(_rot_z);

  // prepare command string
  std::ostringstream cmdStream;
  cmdStream << "UpdateObj Name=" << _ObjName 
            << " Shift=(" << _x_pos << "," << _y_pos << "," << _z_pos << ")"
            << " RotXYZ_deg=(" << _rot_x << "," << _rot_y << "," << _rot_z << ")\n";
  
  std::string command = cmdStream.str();

  // send to server
  {
      std::lock_guard<std::mutex> Lock(pComChann->UseGuard());
      int socket = pComChann->GetSocket();
      if (write(socket, command.c_str(), command.length()) < 0) {
          std::cerr << "Blad wysylania polecenia Set do serwera!" << std::endl;
          return false;
      }
  }

  return true;
}


/*!
 *
 */
bool Interp4Set::ReadParams(std::istream& Strm_CmdsList)
{
  if (!(Strm_CmdsList >> _ObjName >> _x_pos >> _y_pos >> _z_pos >> _rot_x >> _rot_y >> _rot_z)) {
      cout << "Blad wczytywania parametrow Set!" << endl;
      return false;
  }
  return true;
}


/*!
 *
 */
AbstractInterp4Command* Interp4Set::CreateCmd()
{
  return new Interp4Set();
}


/*!
 *
 */
void Interp4Set::PrintSyntax() const
{
  cout << "   Set  Nazwa_Obiektu  x  y  z  rotX  rotY  rotZ " << endl;
}
