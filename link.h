#ifndef _IPYML_LINK_H
#define _IPYML_LINK_H

#include "hardware_address.h"
#include "operstate.h"
#include "yaml.h"

#include <linux/netlink.h>
#include <ostream>
#include <string>

class Link : public YamlObject {
  std::string ifname;
  HardwareAddress address, broadcast;
  OperState operstate;

public:
  // typedef int (*mnl_attr_cb_t)(const struct nlattr *attr, void *data);
  static int mnl_attr_cb(const nlattr *attr, void *data);

  void write_yaml(std::ostream &stream,
                  const yaml_indent_level_t indent_level = 0) const;
};

#endif
