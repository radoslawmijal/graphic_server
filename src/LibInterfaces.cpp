#include "LibInterfaces.hh"
#include <iostream> 

LibInterface::LibInterface(const std::string& LibPath) {

    // open library
    _LibHandler = dlopen(LibPath.c_str(), RTLD_LAZY);
    if (!_LibHandler) {
        std::cerr << "!!! Błąd: Nie można otworzyć biblioteki " << LibPath << std::endl;
        return; 
    }

    // find function CreateCmd
    void* pFuncCreate = dlsym(_LibHandler, "CreateCmd");
    if (!pFuncCreate) {
        std::cerr << "!!! Błąd: Nie znaleziono funkcji CreateCmd w " << LibPath << std::endl;
        dlclose(_LibHandler); 
        _LibHandler = nullptr;
        return;
    }
    // reinterpret cast to proper function pointer type
    _pCreateInterp = reinterpret_cast<CreateInterpFunc>(pFuncCreate);

    // find function GetCmdName
    void* pFuncGetName = dlsym(_LibHandler, "GetCmdName");
    if (!pFuncGetName) {
        std::cerr << "!!! Błąd: Nie znaleziono funkcji GetCmdName w " << LibPath << std::endl;
        dlclose(_LibHandler);
        _LibHandler = nullptr;
        return;
    }
    GetCmdNameFunc pGetName = reinterpret_cast<GetCmdNameFunc>(pFuncGetName);
    
    // get command name
    _CmdName = pGetName();
}

LibInterface::~LibInterface() {
    if (_LibHandler) {
        dlclose(_LibHandler);
    }
}

bool LibInterface::IsValid() const {
    return _LibHandler && _pCreateInterp && !_CmdName.empty();
}

const std::string& LibInterface::GetCmdName() const {
    return _CmdName;
}

AbstractInterp4Command* LibInterface::CreateInterp() {
    if (_pCreateInterp) {
        return _pCreateInterp(); 
    }
    return nullptr;
}