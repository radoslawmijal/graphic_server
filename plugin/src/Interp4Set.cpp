#include <iostream>
#include "Interp4Set.hh"


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
Interp4Set::Interp4Set(): _x_pos(0), _y_pos(0), _z_pos(0), _rot_x(0), _rot_y(0), _rot_z(0)
{}


/*!
 *
 */
void Interp4Set::PrintCmd() const
{
  /*
   *  Tu trzeba napisać odpowiednio zmodyfikować kod poniżej.
   */
  cout << GetCmdName() << " " << _x_pos << " (x pos value) " << _y_pos << " (y pos value) " << _z_pos << " (z pos value) "
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
                           const char         *sMobObjName,
			   AbstractComChannel &rComChann
			 )
{
  /*
   *  Tu trzeba napisać odpowiedni kod.
   */
  return true;
}


/*!
 *
 */
bool Interp4Set::ReadParams(std::istream& Strm_CmdsList)
{
  std::string obj_name;

  if (!(Strm_CmdsList >> obj_name >> _x_pos >> _y_pos >> _z_pos >> _rot_x >> _rot_y >> _rot_z)) {
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
  cout << "   Set  Nazwa_Obiektu  wsp_x  wsp_y  kąt " << endl;
}
