#ifndef _IPYML_ADDRESS_H
#define _IPYML_ADDRESS_H

#include "inet_address.h"
#include "yaml.h"

#include <libmnl/libmnl.h>
#include <linux/if_addr.h>
#include <linux/netlink.h>
#include <ostream>

class Address : public YamlObject {
  InetAddress inet_addr;

public:
  // typedef int (*mnl_attr_cb_t)(const struct nlattr *attr, void *data);
  static int mnl_attr_cb(const nlattr *attr, void *data) {
    Address *addr = (Address *)data;
    // /usr/include/linux/if_addr.h
    switch (mnl_attr_get_type(attr)) {
    case IFA_ADDRESS:
      addr->inet_addr = attr;
      break;
    }
  }

  void write_yaml(std::ostream &stream,
                  const yaml_indent_level_t indent_level = 0) const {
    // const std::string indent(indent_level, ' ');
    inet_addr.write_yaml(stream, indent_level + 2);
  }
};

#endif
