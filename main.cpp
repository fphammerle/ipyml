#include "hardware_address.h"
#include "link.h"
#include "vector.h"

#include <cassert>
#include <cstdlib>
#include <iostream>
#include <libmnl/libmnl.h>
#include <linux/netlink.h>
#include <linux/rtnetlink.h>
#include <string>

// https://netfilter.org/projects/libmnl/doxygen/html/modules.html

static int link_cb(const nlmsghdr *nlh, void *data) {
  vector<Link> *links = (vector<Link> *)data;
  links->emplace_back();
  mnl_attr_parse(nlh, sizeof(ifinfomsg), Link::mnl_attr_cb, &links->back());
  return MNL_CB_OK;
}

void mnl_read_links(const mnl_socket *nl, vector<Link> *links) {
  unsigned int nlpid = mnl_socket_get_portid(nl);

  uint8_t msgbuf[MNL_SOCKET_BUFFER_SIZE];
  nlmsghdr *nlh = mnl_nlmsg_put_header(msgbuf);
  nlh->nlmsg_type = RTM_GETLINK;
  nlh->nlmsg_flags = NLM_F_REQUEST | NLM_F_DUMP;
  nlh->nlmsg_seq = 0;
  mnl_nlmsg_put_extra_header(nlh, sizeof(rtgenmsg));
  assert(mnl_socket_sendto(nl, nlh, nlh->nlmsg_len) > 0);

  size_t numbytes = mnl_socket_recvfrom(nl, msgbuf, sizeof(msgbuf));
  while (numbytes > 0) {
    if (mnl_cb_run(msgbuf, numbytes, nlh->nlmsg_seq, nlpid, link_cb, links) <=
        MNL_CB_STOP) {
      break;
    } else {
      numbytes = mnl_socket_recvfrom(nl, msgbuf, sizeof(msgbuf));
    }
  }
  assert(numbytes != -1);
}

int main(int argc, char *argv[]) {
  mnl_socket *nl = mnl_socket_open(NETLINK_ROUTE);
  assert(nl);
  assert(mnl_socket_bind(nl, 0, MNL_SOCKET_AUTOPID) == 0);

  vector<Link> links;
  mnl_read_links(nl, &links);
  links.write_yaml(std::cout);
  std::cout << std::endl;

  mnl_socket_close(nl);
  return EXIT_SUCCESS;
}
