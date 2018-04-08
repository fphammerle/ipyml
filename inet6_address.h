#ifndef _IPYML_INET6_ADDRESS_H
#define _IPYML_INET6_ADDRESS_H

#include "yaml.h"

#include <arpa/inet.h>     // inet_ntop, socklen_t
#include <bits/socket.h>   // AF_*, socklen_t
#include <cassert>         // assert
#include <cstring>         // memcpy
#include <libmnl/libmnl.h> // mnl_attr_*
#include <linux/netlink.h> // struct nlattr
#include <netinet/in.h>    // INET6_ADDRSTRLEN, struct in6_addr
#include <ostream>         // std::ostream

/*
class may not have any virtual member due to:
> error: union member ‘Address::<anonymous union>::inet6_addr’
> with non-trivial ‘constexpr Inet6Address::Inet6Address(Inet6Address&&)’
*/

class Inet6Address { // : public YamlObject {
  unsigned char bytes[sizeof(in6_addr)];

public:
  Inet6Address &operator=(const nlattr *attr) {
    assert(mnl_attr_validate(attr, MNL_TYPE_BINARY) == 0);
    assert(mnl_attr_get_payload_len(attr) == sizeof(bytes));
    memcpy(this->bytes, mnl_attr_get_payload(attr), sizeof(bytes));
    return *this;
  }

  void format(char *dst, socklen_t size) const {
    inet_ntop(AF_INET6, bytes, dst, size);
  }

  void write_yaml(std::ostream &stream,
                  const yaml_indent_level_t indent_level = 0) const {
    char str[INET6_ADDRSTRLEN];
    format(str, sizeof(str));
    stream << str << '\n';
  }
};

#endif
