/**
* helloworld API generated from helloworld.yang
*
* NOTE: This file is auto generated by polycube-codegen
* https://github.com/polycube-network/polycube-codegen
*/


/* Do not edit this file manually */


#include "HelloworldBase.h"

HelloworldBase::HelloworldBase(const std::string name) {
  logger()->set_pattern("[%Y-%m-%d %H:%M:%S.%e] [Helloworld] [%n] [%l] %v");
}



HelloworldBase::~HelloworldBase() {}

void HelloworldBase::update(const HelloworldJsonObject &conf) {
  set_conf(conf.getBase());

  if (conf.portsIsSet()) {
    for (auto &i : conf.getPorts()) {
      auto name = i.getName();
      auto m = getPorts(name);
      m->update(i);
    }
  }
  if (conf.actionIsSet()) {
    setAction(conf.getAction());
  }
}

HelloworldJsonObject HelloworldBase::toJsonObject() {
  HelloworldJsonObject conf;
  conf.setBase(to_json());

  conf.setName(getName());
  for (auto &i : getPortsList()) {
    conf.addPorts(i->toJsonObject());
  }
  conf.setAction(getAction());

  return conf;
}
void HelloworldBase::addPortsList(const std::vector<PortsJsonObject> &conf) {
  for (auto &i : conf) {
    std::string name_ = i.getName();
    addPorts(name_, i);
  }
}

void HelloworldBase::replacePorts(const std::string &name, const PortsJsonObject &conf) {
  delPorts(name);
  std::string name_ = conf.getName();
  addPorts(name_, conf);
}

void HelloworldBase::delPortsList() {
  auto elements = getPortsList();
  for (auto &i : elements) {
    std::string name_ = i->getName();
    delPorts(name_);
  }
}

void HelloworldBase::addPorts(const std::string &name, const PortsJsonObject &conf) {
  add_port<PortsJsonObject>(name, conf);
}

void HelloworldBase::delPorts(const std::string &name) {
  remove_port(name);
}

std::shared_ptr<Ports> HelloworldBase::getPorts(const std::string &name) {
  return get_port(name);
}

std::vector<std::shared_ptr<Ports>> HelloworldBase::getPortsList() {
  return get_ports();
}


