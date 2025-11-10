#pragma once

#include <string>
#include <map>
#include <memory> 
#include "LibInterfaces.hh" 

using PluginMap = std::map<std::string, std::shared_ptr<LibInterface>>;

class Set4LibInterfaces {

private:
    PluginMap _Plugins; 

public:
    Set4LibInterfaces() = default;
    ~Set4LibInterfaces() = default;

    std::shared_ptr<LibInterface> FindPlugin(const std::string& CmdName); 
    bool AddPlugin(const std::string& LibPath); //true , if added successfully
};