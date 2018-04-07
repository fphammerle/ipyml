#ifndef _IPYML_INET_ADDRESS_H
#define _IPYML_INET_ADDRESS_H

#include "yaml.h"

#include <arpa/inet.h>
#include <cassert>
#include <cstring>
#include <libmnl/libmnl.h>
#include <linux/netlink.h>
#include <netinet/in.h>
#include <ostream>

class InetAddress : public YamlObject {
  unsigned char bytes[sizeof(in_addr_t)];

public:
  InetAddress &operator=(const nlattr *attr) {
    assert(mnl_attr_validate(attr, MNL_TYPE_BINARY) == 0);
    assert(mnl_attr_get_payload_len(attr) == sizeof(bytes));
    memcpy(this->bytes, mnl_attr_get_payload(attr), sizeof(bytes));
    return *this;
  }

  void format(char *dst, socklen_t size) const {
    inet_ntop(AF_INET, bytes, dst, size);
  }

  void write_yaml(std::ostream &stream,
                  const yaml_indent_level_t indent_level = 0) const {
    char str[INET_ADDRSTRLEN];
    format(str, sizeof(str));
    stream << str << '\n';
  }
};

#endif
