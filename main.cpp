#include "address.h"
#include "hardware_address.h"
#include "link.h"
#include "vector.h"

#include <cassert>
#include <cstdlib>
#include <iostream>
#include <libmnl/libmnl.h>
#include <linux/if_addr.h>
#include <linux/netlink.h>
#include <linux/rtnetlink.h> // struct ifinfomsg

// https://netfilter.org/projects/libmnl/doxygen/html/modules.html

void mnl_recv_run_cb_all(const mnl_socket *nl, void *buf, size_t bufsiz,
                         mnl_cb_t cb_data, void *data) {
  unsigned int nlpid = mnl_socket_get_portid(nl);
  ssize_t numbytes = mnl_socket_recvfrom(nl, buf, bufsiz);
  while (numbytes > 0) {
    if (mnl_cb_run(buf, numbytes, 0, nlpid, cb_data, data) <= MNL_CB_STOP) {
      break;
    }
    numbytes = mnl_socket_recvfrom(nl, buf, bufsiz);
  }
  assert(numbytes != -1);
}

int link_cb(const nlmsghdr *nlh, void *data) {
  vector<Link> *links = (vector<Link> *)data;
  links->emplace_back((const ifinfomsg *)mnl_nlmsg_get_payload(nlh));
  assert(links->back().index == links->size());
  mnl_attr_parse(nlh, sizeof(ifinfomsg), Link::mnl_attr_cb, &links->back());
  return MNL_CB_OK;
}

void mnl_read_links(const mnl_socket *nl, vector<Link> *links) {
  uint8_t msgbuf[MNL_SOCKET_BUFFER_SIZE];
  nlmsghdr *nlh = mnl_nlmsg_put_header(msgbuf);
  nlh->nlmsg_type = RTM_GETLINK;
  nlh->nlmsg_flags = NLM_F_REQUEST | NLM_F_DUMP;
  mnl_nlmsg_put_extra_header(nlh, sizeof(rtgenmsg));
  assert(mnl_socket_sendto(nl, nlh, nlh->nlmsg_len) > 0);
  mnl_recv_run_cb_all(nl, msgbuf, sizeof(msgbuf), link_cb, links);
}

static int address_cb(const struct nlmsghdr *nlh, void *data) {
  vector<Link> *links = (vector<Link> *)data;
  Address addr((const ifaddrmsg *)mnl_nlmsg_get_payload(nlh));
  mnl_attr_parse(nlh, sizeof(ifaddrmsg), Address::mnl_attr_cb, &addr);
  links->at(addr.ifindex - 1).addresses.push_back(addr);
  return MNL_CB_OK;
}

void mnl_add_addresses(const mnl_socket *nl, vector<Link> *links) {
  char msgbuf[MNL_SOCKET_BUFFER_SIZE];
  nlmsghdr *nlh = mnl_nlmsg_put_header(msgbuf);
  nlh->nlmsg_type = RTM_GETADDR;
  nlh->nlmsg_flags = NLM_F_REQUEST | NLM_F_DUMP;
  rtgenmsg *rt = (rtgenmsg *)mnl_nlmsg_put_extra_header(nlh, sizeof(rtgenmsg));
  rt->rtgen_family = AF_UNSPEC;
  assert(mnl_socket_sendto(nl, nlh, nlh->nlmsg_len) > 0);
  mnl_recv_run_cb_all(nl, msgbuf, sizeof(msgbuf), address_cb, links);
}

int main(int argc, char *argv[]) {
  mnl_socket *nl = mnl_socket_open(NETLINK_ROUTE);
  assert(nl);
  assert(mnl_socket_bind(nl, 0, MNL_SOCKET_AUTOPID) == 0);

  vector<Link> links;
  mnl_read_links(nl, &links);
  mnl_add_addresses(nl, &links);
  std::cout << "links: ";
  links.write_yaml(std::cout);
  std::cout << std::endl;

  mnl_socket_close(nl);
  return EXIT_SUCCESS;
}
