#ifndef PROGRAMINTERPRETER_HH
#define PROGRAMINTERPRETER_HH

#include "Scene.hh"
#include "Set4LibInterfaces.hh"
#include <iostream>
#include <string>

class ProgramInterpreter {
  private:
    Scene & _scn;
    Set4LibInterfaces & _libManager;

  public:
    ProgramInterpreter(Scene & scn, Set4LibInterfaces & libManager) 
        : _scn(scn), _libManager(libManager) {}

    bool ReadFile(const char* fileName);
    bool ExecProgram(const char* fileName);
};

#endif