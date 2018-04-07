#include "link.h"

#include <libmnl/libmnl.h>
#include <linux/if_link.h>
#include <string>

// https://netfilter.org/projects/libmnl/doxygen/html/group__attr.html

int Link::mnl_attr_cb(const nlattr *attr, void *data) {
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
  case IFLA_OPERSTATE:
    link->operstate = attr;
    break;
  }
  return MNL_CB_OK;
}

void Link::write_yaml(std::ostream &stream,
                      const yaml_indent_level_t indent_level) const {
  const std::string indent(indent_level, ' ');
  stream << "ifname: " + ifname + "\n";
  stream << indent + "hwaddr: ";
  hwaddr.write_yaml(stream);
  stream << "\n";
  stream << indent + "broadcast: ";
  broadcast.write_yaml(stream);
  stream << "\n";
  stream << indent + "operstate: ";
  operstate.write_yaml(stream);
}
