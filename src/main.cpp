#include <iostream>
#include <dlfcn.h>
#include <cassert>
#include "AbstractInterp4Command.hh"

using namespace std;

#define PLUGIN_NAME__move "libInterp4Move.so"
#define PLUGIN_NAME__pause "libInterp4Pause.so"
#define PLUGIN_NAME__rotate "libInterp4Rotate.so"
#define PLUGIN_NAME__set "libInterp4Set.so"


int main()
{
  void *pLibHnd_Move = dlopen(PLUGIN_NAME__move,RTLD_LAZY);
  AbstractInterp4Command *(*pCreateCmd_Move)(void);

  void *pFun;

  if (!pLibHnd_Move) {
    cerr << "!!! Brak biblioteki: " PLUGIN_NAME__move << endl;
    return 1;
  }

  pFun = dlsym(pLibHnd_Move,"CreateCmd");
  if (!pFun) {
    cerr << "!!! Nie znaleziono funkcji CreateCmd" << endl;
    return 1;
  }
  pCreateCmd_Move = reinterpret_cast<AbstractInterp4Command* (*)(void)>(pFun);


  AbstractInterp4Command *pCmd_Move = pCreateCmd_Move();

  cout << endl;
  cout << pCmd_Move->GetCmdName() << endl;
  cout << endl;
  pCmd_Move->PrintSyntax();
  cout << endl;
  pCmd_Move->PrintCmd();
  cout << endl;
  
  delete pCmd_Move;

  dlclose(pLibHnd_Move);


  /*    PAUSE   */

  void *pLibHnd_Pause = dlopen(PLUGIN_NAME__pause,RTLD_LAZY);
  AbstractInterp4Command *(*pCreateCmd_Pause)(void);

  if (!pLibHnd_Pause) {
    cerr << "!!! Brak biblioteki: " PLUGIN_NAME__pause << endl;
    return 1;
  }

  pFun = dlsym(pLibHnd_Pause,"CreateCmd");
  if (!pFun) {
    cerr << "!!! Nie znaleziono funkcji CreateCmd" << endl;
    return 1;
  }
  pCreateCmd_Pause = reinterpret_cast<AbstractInterp4Command* (*)(void)>(pFun);


  AbstractInterp4Command *pCmd_Pause = pCreateCmd_Pause();

  cout << endl;
  cout << pCmd_Pause->GetCmdName() << endl;
  cout << endl;
  pCmd_Pause->PrintSyntax();
  cout << endl;
  pCmd_Pause->PrintCmd();
  cout << endl;
  
  delete pCmd_Pause;

  dlclose(pLibHnd_Pause);

    /*    ROTATE   */

  void *pLibHnd_Rotate = dlopen(PLUGIN_NAME__rotate,RTLD_LAZY);
  AbstractInterp4Command *(*pCreateCmd_Rotate)(void);

  if (!pLibHnd_Rotate) {
    cerr << "!!! Brak biblioteki: " PLUGIN_NAME__rotate << endl;
    return 1;
  }

  pFun = dlsym(pLibHnd_Rotate,"CreateCmd");
  if (!pFun) {
    cerr << "!!! Nie znaleziono funkcji CreateCmd" << endl;
    return 1;
  }
  pCreateCmd_Rotate = reinterpret_cast<AbstractInterp4Command* (*)(void)>(pFun);


  AbstractInterp4Command *pCmd_Rotate = pCreateCmd_Rotate();

  cout << endl;
  cout << pCmd_Rotate->GetCmdName() << endl;
  cout << endl;
  pCmd_Rotate->PrintSyntax();
  cout << endl;
  pCmd_Rotate->PrintCmd();
  cout << endl;
  
  delete pCmd_Rotate;

  dlclose(pLibHnd_Rotate);

  /*    SET   */

  void *pLibHnd_Set = dlopen(PLUGIN_NAME__set,RTLD_LAZY);
  AbstractInterp4Command *(*pCreateCmd_Set)(void);

  if (!pLibHnd_Set) {
    cerr << "!!! Brak biblioteki: " PLUGIN_NAME__set << endl;
    return 1;
  }

  pFun = dlsym(pLibHnd_Set,"CreateCmd");
  if (!pFun) {
    cerr << "!!! Nie znaleziono funkcji CreateCmd" << endl;
    return 1;
  }
  pCreateCmd_Set = reinterpret_cast<AbstractInterp4Command* (*)(void)>(pFun);


  AbstractInterp4Command *pCmd_Set = pCreateCmd_Set();

  cout << endl;
  cout << pCmd_Set->GetCmdName() << endl;
  cout << endl;
  pCmd_Set->PrintSyntax();
  cout << endl;
  pCmd_Set->PrintCmd();
  cout << endl;
  
  delete pCmd_Set;

  dlclose(pLibHnd_Set);
}
