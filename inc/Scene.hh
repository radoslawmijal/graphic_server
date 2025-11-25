#ifndef SCENE_HH
#define SCENE_HH

#include <map>
#include <string>
#include <iostream>
#include <memory> 
#include <utility>
#include "AbstractScene.hh"

class Scene : public AbstractScene {
  private:
    std::map<std::string, std::unique_ptr<AbstractMobileObj>> _objects;

  public:
    Scene() {};
    virtual ~Scene() {};

    AbstractMobileObj* FindMobileObj(const char *sName) override {
        std::string name(sName);
        std::map<std::string, std::unique_ptr<AbstractMobileObj>>::iterator it = _objects.find(name);
        if (it == _objects.end()) {
            return nullptr;
        }
        return it->second.get();
    }

    virtual void AddMobileObj(AbstractMobileObj *pMobObj) override {
        if (!pMobObj) return;
        std::unique_ptr<AbstractMobileObj> ptr(pMobObj);
        _objects.insert(std::make_pair(ptr->GetName(), std::move(ptr)));
    }
};

#endif