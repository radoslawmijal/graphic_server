#include "ProgramInterpreter.hh"
#include "Preprocessor.hh"
#include "AbstractInterp4Command.hh" 
#include "Configuration.hh"
#include <sstream>
#include <memory>


using namespace std;



bool ProgramInterpreter::ExecProgram(const char* fileName) 
{
    Preprocessor preproc;
    string processedCode = preproc.ProcessFile(fileName);
    
    if (processedCode.empty()) {
        cerr << "Blad preprocesora lub pusty plik!" << endl;
        return false;
    }

    istringstream IStrm(processedCode);
    string cmdName;


    while (IStrm >> cmdName) {
        
        cout << "Wczytano polecenie: " << cmdName << endl;

        std::shared_ptr<LibInterface> plugin = _libManager.FindPlugin(cmdName);
        
        if (!plugin) {
            cerr << "Nieznana komenda: " << cmdName << endl;
            string unknown_command; getline(IStrm, unknown_command); 
            continue; 
        }

        AbstractInterp4Command *cmd = plugin->CreateInterp();
        
        if (!cmd) {
            cerr << "Blad tworzenia instancji polecenia!" << endl;
            continue;
        }

        if (!cmd->ReadParams(IStrm)) {
            cerr << "Blad wczytywania parametrow!" << endl;
            delete cmd; 
            return false;
        }

        cmd->PrintCmd();
        
        delete cmd;
    }

    return true;
}