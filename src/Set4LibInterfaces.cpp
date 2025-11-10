#include "Set4LibInterfaces.hh"
#include <iostream> 

bool Set4LibInterfaces::AddPlugin(const std::string& LibPath) {

    // make a new LibInterface instance
    auto pNewPlugin = std::make_shared<LibInterface>(LibPath);
    if (!pNewPlugin->IsValid()) {
        std::cerr << "!!! Info: Nie udało się poprawnie załadować wtyczki z " << LibPath << std::endl;
        return false;
    }

    // get name
    std::string cmdName = pNewPlugin->GetCmdName();
    if (_Plugins.count(cmdName)) {
        std::cerr << "!!! Ostrzeżenie: Wtyczka dla polecenia '" << cmdName << "' jest już załadowana. Pomijam." << std::endl;
        return false;
    }

    // add to map
    _Plugins[cmdName] = pNewPlugin;
    std::cout << "--- Info: Załadowano wtyczkę dla polecenia: " << cmdName << std::endl;
    return true;
}

std::shared_ptr<LibInterface> Set4LibInterfaces::FindPlugin(const std::string& CmdName) {
    auto it = _Plugins.find(CmdName);
    if (it == _Plugins.end()) {
        return nullptr;
    }
    return it->second;
}