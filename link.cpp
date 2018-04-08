#include "link.h"

#include <libmnl/libmnl.h> // mnl_attr_*
#include <linux/if_link.h> // IFLA_*
#include <string>          // std::string

Link::Link(const ifinfomsg *msg) {
  /*
  struct ifinfomsg {
      unsigned char  ifi_family; // AF_UNSPEC
      unsigned short ifi_type;   // Device type
      int            ifi_index;  // Interface index
      unsigned int   ifi_flags;  // Device flags
      unsigned int   ifi_change; // change mask
  };
  */
  index = msg->ifi_index;
}

int Link::mnl_attr_cb(const nlattr *attr, void *data) {
  Link *link = (Link *)data;
  // https://netfilter.org/projects/libmnl/doxygen/html/group__attr.html
  // /usr/include/linux/if_link.h
  switch (mnl_attr_get_type(attr)) {
  case IFLA_ADDRESS:
    link->address = attr;
    break;
  case IFLA_BROADCAST:
    link->broadcast = attr;
    break;
  case IFLA_IFNAME:
    assert(mnl_attr_validate(attr, MNL_TYPE_STRING) == 0);
    link->ifname = mnl_attr_get_str(attr);
    break;
  case IFLA_MTU:
    assert(mnl_attr_validate(attr, MNL_TYPE_U32) == 0);
    link->mtu = mnl_attr_get_u32(attr);
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
  // stream << indent << "index: " << index << "\n";
  stream << indent << "address: ";
  address.write_yaml(stream);
  stream << "\n";
  stream << indent << "broadcast: ";
  broadcast.write_yaml(stream);
  stream << "\n";
  stream << indent << "mtu: " << mtu << "\n";
  stream << indent << "operstate: ";
  operstate.write_yaml(stream);
}
