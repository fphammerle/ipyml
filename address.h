#ifndef _IPYML_ADDRESS_H
#define _IPYML_ADDRESS_H

#include "yaml.h"

#include <arpa/inet.h>
#include <cassert>
#include <cstring>
#include <libmnl/libmnl.h>
#include <linux/if_addr.h>
#include <linux/netlink.h>
#include <netinet/in.h>
#include <ostream>

class Address : public YamlObject {
public:
  unsigned char address[sizeof(in_addr_t)];

public:
  // typedef int (*mnl_attr_cb_t)(const struct nlattr *attr, void *data);
  static int mnl_attr_cb(const nlattr *attr, void *data) {
    Address *addr = (Address *)data;
    // /usr/include/linux/if_addr.h
    switch (mnl_attr_get_type(attr)) {
    case IFA_ADDRESS:
      assert(mnl_attr_validate(attr, MNL_TYPE_BINARY) == 0);
      assert(mnl_attr_get_payload_len(attr) == sizeof(address));
      memcpy(addr->address, mnl_attr_get_payload(attr), sizeof(address));
      break;
    }
  }

  void write_yaml(std::ostream &stream,
                  const yaml_indent_level_t indent_level = 0) const {
    // const std::string indent(indent_level, ' ');
    char addr_str[INET_ADDRSTRLEN];
    inet_ntop(AF_INET, address, addr_str, sizeof(addr_str));
    stream << addr_str << '\n';
  }
};

#endif
