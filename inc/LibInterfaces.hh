#pragma once

#include <string>
#include <dlfcn.h> 
#include "AbstractInterp4Command.hh" 


//define function pointer types
using CreateInterpFunc = AbstractInterp4Command* (*)(void);
using GetCmdNameFunc = const char* (*)(void);


class LibInterface {

private:
    void* _LibHandler = nullptr; 
    std::string _CmdName; 
    CreateInterpFunc _pCreateInterp = nullptr;

public:
    LibInterface(const std::string& LibPath);
    ~LibInterface();

    bool IsValid() const;   //true, if library and functions loaded successfully
    const std::string& GetCmdName() const;
    AbstractInterp4Command* CreateInterp(); 

    // disable copy constructor and assignment operator
    LibInterface(const LibInterface&) = delete;
    LibInterface& operator=(const LibInterface&) = delete;
};