# ipyml

Show network device configuration in YAML format.

## Build Dependencies

```sh
apt install libmnl-dev linux-libc-dev g++
```

## Example

```yaml
$ ipyml
links:
- ifname: lo
  mtu: 65536
  operstate: unknown
  addresses:
  - 127.0.0.1
- ifname: eth0
  address: 11:22:33:44:55:66
  broadcast: ff:ff:ff:ff:ff:ff
  mtu: 1500
  operstate: up
  addresses:
  - 192.168.2.101
  - fe80::1322:33ff:fe44:5566
- ifname: wlan0
  address: 77:88:99:00:aa:bb
  broadcast: ff:ff:ff:ff:ff:ff
  mtu: 1500
  operstate: up
  addresses:
  - 192.168.20.11
  - fe80::7588:99ff:fe00:aabb

```

## Links

[libmnl source](https://git.netfilter.org/libmnl/tree/src)

[libmnl documentation](https://netfilter.org/projects/libmnl/doxygen/html/modules.html)

[iproute2 src](https://git.kernel.org/pub/scm/network/iproute2/iproute2.git/tree/ip/ipaddress.c)
