#ifndef _IPYML_LINK_H
#define _IPYML_LINK_H

#include "hardware_address.h"
#include "yaml.h"

#include <libmnl/libmnl.h>
#include <linux/if_link.h>
#include <ostream>
#include <string>

// https://netfilter.org/projects/libmnl/doxygen/html/group__attr.html

class Link : public YamlObject {
public:
  std::string ifname;
  HardwareAddress hwaddr, broadcast;

  // typedef int (*mnl_attr_cb_t)(const struct nlattr *attr, void *data);
  static int mnl_attr_cb(const nlattr *attr, void *data) {
    Link *link = (Link *)data;
    switch (mnl_attr_get_type(attr)) {
    case IFLA_ADDRESS:
      link->hwaddr = attr;
      break;
    case IFLA_BROADCAST:
      link->broadcast = attr;
      break;
    case IFLA_IFNAME:
      link->ifname = mnl_attr_get_str(attr);
      break;
    }
    return MNL_CB_OK;
  }

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
