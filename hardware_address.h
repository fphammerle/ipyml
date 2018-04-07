#ifndef _IPYML_HARDWARE_ADDRESS_H
#define _IPYML_HARDWARE_ADDRESS_H

#include "yaml.h"

#include <cassert>
#include <cstring>
#include <iomanip>
#include <libmnl/libmnl.h>
#include <linux/if.h>
#include <linux/netlink.h>
#include <ostream>
#include <sstream>
#include <string>

class HardwareAddress : public YamlObject {
  uint8_t bytes[IFHWADDRLEN];

public:
  HardwareAddress &operator=(const nlattr *attr) {
    assert(mnl_attr_get_payload_len(attr) == IFHWADDRLEN);
    memcpy(this->bytes, mnl_attr_get_payload(attr), IFHWADDRLEN);
    return *this;
  }

  std::string format() const {
    std::stringstream ss;
    ss << std::hex << std::setfill('0');
    for (int i = 0; i < IFHWADDRLEN; i++) {
      if (i != 0) {
        ss << ':';
      }
      ss << std::setw(2) << (int)bytes[i];
    }
    return ss.str();
  }

  void write_yaml(std::ostream &stream,
                  const yaml_indent_level_t indent_level = 0) const {
    stream << format();
  }
};

#endif
