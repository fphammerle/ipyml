#ifndef _IPYML_OPERSTATE_H
#define _IPYML_OPERSTATE_H

#include "yaml.h"

#include <libmnl/libmnl.h>
#include <linux/if.h>
#include <linux/netlink.h>

class OperState : public YamlObject {
  // RFC 2863 operational status
  /* linux/if.h:
  enum {
      IF_OPER_UNKNOWN,
      IF_OPER_NOTPRESENT,
      IF_OPER_DOWN,
      IF_OPER_LOWERLAYERDOWN,
      IF_OPER_TESTING,
      IF_OPER_DORMANT,
      IF_OPER_UP,
  };
  */
  unsigned char state;

public:
  OperState &operator=(const nlattr *attr) {
    assert(mnl_attr_validate(attr, MNL_TYPE_U8) == 0);
    this->state = mnl_attr_get_u8(attr);
    return *this;
  }

  void write_yaml(std::ostream &stream,
                  const yaml_indent_level_t indent_level = 0) const {
    const char *label = NULL;
    switch (state) {
    case IF_OPER_UNKNOWN:
      label = "unknown";
      break;
    case IF_OPER_NOTPRESENT:
      label = "not present";
      break;
    case IF_OPER_DOWN:
      label = "down";
      break;
    case IF_OPER_LOWERLAYERDOWN:
      label = "lower layer down";
      break;
    case IF_OPER_TESTING:
      label = "testing";
      break;
    case IF_OPER_DORMANT:
      label = "dormant";
      break;
    case IF_OPER_UP:
      label = "up";
      break;
    }
    assert(label);
    stream << label << "\n";
  }
};

#endif
