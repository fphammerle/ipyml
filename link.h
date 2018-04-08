#ifndef _IPYML_LINK_H
#define _IPYML_LINK_H

#include "address.h"
#include "hardware_address.h"
#include "operstate.h"
#include "list.h"
#include "yaml.h"

#include <linux/netlink.h>
#include <linux/rtnetlink.h> // struct ifinfomsg
#include <ostream>
#include <string>

class Link : public YamlObject {
public:
  unsigned int index;
  list<Address> addresses;

private:
  std::string ifname;
  HardwareAddress address, broadcast;
  unsigned long int mtu;
  OperState operstate;

public:
  Link(const ifinfomsg *msg);

  // typedef int (*mnl_attr_cb_t)(const struct nlattr *attr, void *data);
  static int mnl_attr_cb(const nlattr *attr, void *data);

  void write_yaml(std::ostream &stream,
                  const yaml_indent_level_t indent_level = 0) const;
};

#endif
