# ipyml

Show network device configuration in YAML format.

## Build Dependencies

```sh
apt install libmnl-dev linux-libc-dev g++
```

## Example

```yaml
$ ipyml

- ifname: lo
  address: 00:00:00:00:00:00
  broadcast: 00:00:00:00:00:00
  mtu: 65536
  operstate: unknown
- ifname: eth0
  address: 11:22:33:44:55:66
  broadcast: ff:ff:ff:ff:ff:ff
  mtu: 1500
  operstate: down
- ifname: wlan0
  address: 77:88:99:00:aa:bb
  broadcast: ff:ff:ff:ff:ff:ff
  mtu: 1500
  operstate: up

```
