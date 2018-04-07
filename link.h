#ifndef _IPYML_LINK_H
#define _IPYML_LINK_H

#include "hardware_address.h"
#include "yaml.h"

#include <ostream>
#include <string>

class Link : public YamlObject {
public:
  std::string ifname;
  HardwareAddress hwaddr, broadcast;

  void write_yaml(std::ostream &stream,
                  const yaml_indent_level_t indent_level = 0) const {
    const std::string indent(indent_level, ' ');
    stream << "ifname: " + ifname + "\n";
    stream << indent + "hwaddr: ";
    hwaddr.write_yaml(stream);
    stream << "\n";
    stream << indent + "broadcast: ";
    broadcast.write_yaml(stream);
    stream << "\n";
  }
};

#endif
