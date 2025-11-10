#include <iostream>
#include <fstream>
#include <string>
#include <dlfcn.h>
#include "AbstractInterp4Command.hh"
#include "Set4LibInterfaces.hh"

using namespace std;

#define PLUGIN_NAME__move "libInterp4Move.so"
#define PLUGIN_NAME__pause "libInterp4Pause.so"
#define PLUGIN_NAME__rotate "libInterp4Rotate.so"
#define PLUGIN_NAME__set "libInterp4Set.so"

#define COMMANDS_FILE_NAME "commands.txt"

int main()
{
  Set4LibInterfaces LibManager;
  cout << "Wczytywanie wtyczek..." << endl;

  LibManager.AddPlugin(PLUGIN_NAME__move);
  LibManager.AddPlugin(PLUGIN_NAME__pause);
  LibManager.AddPlugin(PLUGIN_NAME__rotate);
  LibManager.AddPlugin(PLUGIN_NAME__set);
  cout << "Wtyczki wczytane." << endl;

  ifstream CmdFile(COMMANDS_FILE_NAME);
  if (!CmdFile.is_open())
  {
    cerr << "!!! Błąd: Nie mozna otworzyc pliku z komendami: " << COMMANDS_FILE_NAME << endl;
    return 1;
  }
  cout << "Plik z komendami otwarty." << endl;
  string CommandName;

  while (CmdFile >> CommandName){
    cout << "Wczytano komende: " << CommandName << endl;

    shared_ptr<LibInterface> pPlugin = LibManager.FindPlugin(CommandName);
    if (pPlugin == nullptr){
      cerr << "!!! Błąd: Nie znaleziono wtyczki dla komendy: " << CommandName << endl;
      continue;
    }

    AbstractInterp4Command* pInterpreter = pPlugin->CreateInterp();
    if (pInterpreter == nullptr){
      cerr << "!!! Błąd: Nie mozna utworzyc interpretera dla komendy: " << CommandName << endl;
      continue;
    }

    cout << "Składnia komendy: " << CommandName << endl;
    pInterpreter->PrintSyntax();

    delete pInterpreter;
    cout << endl;
  }

  CmdFile.close();
  cout << "Plik z komendami zamknięty." << endl;

  return 0;
}
