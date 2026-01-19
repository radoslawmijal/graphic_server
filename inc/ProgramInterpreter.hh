#ifndef PROGRAMINTERPRETER_HH
#define PROGRAMINTERPRETER_HH

#include "Scene.hh"
#include "Set4LibInterfaces.hh"
#include "AbstractComChannel.hh"
#include <iostream>
#include <string>
#include <mutex>

class ProgramInterpreter : public AbstractComChannel {
  private:
    Scene & _scn;
    Set4LibInterfaces & _libManager;
    int _socket;
    std::mutex _mtx;

  public:
    ProgramInterpreter(Scene & scn, Set4LibInterfaces & libManager) 
        : _scn(scn), _libManager(libManager) {}

    bool ReadFile(const char* fileName);
    bool ExecProgram(const char* fileName);

    bool ConnectToServer(const char* addr, int port);

    virtual int GetSocket() const override { return _socket; }
    virtual std::mutex & UseGuard() override { return _mtx; }
};

#endif