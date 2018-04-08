#ifndef _IPYML_ADDRESS_H
#define _IPYML_ADDRESS_H

#include "inet6_address.h"
#include "inet_address.h"
#include "yaml.h"

#include <bits/socket.h>   // AF_*
#include <libmnl/libmnl.h> // mnl_attr_get_type
#include <linux/if_addr.h> // struct ifaddrmsg
#include <linux/netlink.h> // struct nlattr
#include <ostream>         // std::ostream

class Address : public YamlObject {
  unsigned char family;
  union {
    InetAddress inet_addr;
    Inet6Address inet6_addr;
  };

public:
  Address() : family(AF_UNSPEC) {}

  Address &operator=(const ifaddrmsg *msg) {
    family = msg->ifa_family;
    assert(family == AF_INET || family == AF_INET6);
    return *this;
  }

  // typedef int (*mnl_attr_cb_t)(const struct nlattr *attr, void *data);
  static int mnl_attr_cb(const nlattr *attr, void *data) {
    Address *addr = (Address *)data;
    assert(addr->family != AF_UNSPEC);
    // /usr/include/linux/if_addr.h
    switch (mnl_attr_get_type(attr)) {
    case IFA_ADDRESS:
      if (addr->family == AF_INET) {
        addr->inet_addr = attr;
      } else if (addr->family == AF_INET6) {
        addr->inet6_addr = attr;
      }
      break;
    }
  }

  void write_yaml(std::ostream &stream,
                  const yaml_indent_level_t indent_level = 0) const {
    // const std::string indent(indent_level, ' ');
    if (family == AF_INET) {
      inet_addr.write_yaml(stream, indent_level + 2);
    } else if (family == AF_INET6) {
      inet6_addr.write_yaml(stream, indent_level + 2);
    }
  }
};

#endif
