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
  hwaddr: 00:00:00:00:00:00
  broadcast: 00:00:00:00:00:00
- ifname: eth0
  hwaddr: 11:22:33:44:55:66
  broadcast: ff:ff:ff:ff:ff:ff
- ifname: wlan0
  hwaddr: 77:88:99:00:aa:bb
  broadcast: ff:ff:ff:ff:ff:ff

```
