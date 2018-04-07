#include <assert.h>
#include <cstring>
#include <iomanip>
#include <iostream>
#include <libmnl/libmnl.h>
#include <linux/if.h>
#include <linux/rtnetlink.h>
#include <sstream>
#include <string>
#include <vector>

// https://netfilter.org/projects/libmnl/doxygen/html/modules.html

typedef size_t yaml_indent_level_t;

class YamlObject {
  virtual void write_yaml(std::ostream &stream,
                          const yaml_indent_level_t indent_level = 0) const = 0;
};

struct HardwareAddress : public YamlObject {
  uint8_t bytes[IFHWADDRLEN];

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

struct Link : public YamlObject {
  std::string ifname;
  HardwareAddress hwaddr, broadcast;

  void write_yaml(std::ostream &stream,
                  const yaml_indent_level_t indent_level = 0) const {
    const std::string indent(indent_level, ' ');
    stream << "ifname: " + ifname + "\n";
    stream << indent + "hwaddr: ";
    hwaddr.write_yaml(stream);
    stream << "\n";
    stream << indent + "broadcast: ";
    broadcast.write_yaml(stream);
    stream << "\n";
  }
};

template <class T> class vector : public std::vector<T>, YamlObject {
public:
  void write_yaml(std::ostream &stream,
                  const yaml_indent_level_t indent_level = 0) const {
    if (this->empty()) {
      stream << "[]";
    } else {
      const std::string indent(indent_level, ' ');
      stream << "\n";
      for (auto it = this->begin(); it != this->end(); it++) {
        stream << indent << "- ";
        it->write_yaml(stream, indent_level + 2);
      }
    }
  }
};

static int link_attr_cb(const nlattr *attr, void *data) {
  Link *link = (Link *)data;
  // /usr/include/linux/if_link.h
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
  }
  return MNL_CB_OK;
}

static int link_cb(const nlmsghdr *nlh, void *data) {
  vector<Link> *links = (vector<Link> *)data;
  links->emplace_back();
  mnl_attr_parse(nlh, sizeof(ifinfomsg), link_attr_cb, &links->back());
  return MNL_CB_OK;
}

int main(int argc, char *argv[]) {
  mnl_socket *nl = mnl_socket_open(NETLINK_ROUTE);
  assert(nl);
  assert(mnl_socket_bind(nl, 0, MNL_SOCKET_AUTOPID) == 0);
  unsigned int nlpid = mnl_socket_get_portid(nl);

  uint8_t msgbuf[MNL_SOCKET_BUFFER_SIZE];
  nlmsghdr *nlh = mnl_nlmsg_put_header(msgbuf);
  nlh->nlmsg_type = RTM_GETLINK;
  nlh->nlmsg_flags = NLM_F_REQUEST | NLM_F_DUMP;
  nlh->nlmsg_seq = 0;
  mnl_nlmsg_put_extra_header(nlh, sizeof(rtgenmsg));
  assert(mnl_socket_sendto(nl, nlh, nlh->nlmsg_len) > 0);

  vector<Link> links;
  size_t numbytes = mnl_socket_recvfrom(nl, msgbuf, sizeof(msgbuf));
  while (numbytes > 0) {
    if (mnl_cb_run(msgbuf, numbytes, nlh->nlmsg_seq, nlpid, link_cb, &links) <= MNL_CB_STOP) {
      break;
    } else {
      numbytes = mnl_socket_recvfrom(nl, msgbuf, sizeof(msgbuf));
    }
  }
  assert(numbytes != -1);
  links.write_yaml(std::cout);
  std::cout << std::endl;

  mnl_socket_close(nl);
  return EXIT_SUCCESS;
}
