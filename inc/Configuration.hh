#ifndef CONFIGURATION_HH
#define CONFIGURATION_HH

#include "Set4LibInterfaces.hh"
#include "Scene.hh"

class Configuration {
  public:
  Configuration() = default;
  ~Configuration() = default;
    Set4LibInterfaces LibManager;
    Scene Scn;
};


#endif
