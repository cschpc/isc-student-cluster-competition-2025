# System adminstration in the competition

## Adding users

- `useradd`
   - are non-default options needed for `$HOME` etc.?

## Network setup

- Cluster setup with `nmcli` and `nmtui`
- Static IPs needed for laptops?

## Firewall

- Firewall should be setup with `iptables`

## Filesystem tuning

### Kernel uring

Change uring to 0 in both nodes (https://lwn.net/Articles/898837/)

```
[root@csc-01 ~]# cat /etc/sysctl.d/10-io_uring.conf
# Activate io_uring for all processes.
# swesters, 2025-05-27: io_uring should help with
# IOPS and throughput for XFS, and possibly other applications too.
kernel.io_uring_disabled=0


[root@csc-02 ~]# cat /etc/sysctl.d/10-io_uring.conf
# Activate io_uring for all processes.
# swesters, 2025-05-27: io_uring should help with
# IOPS and throughput for XFS, and possibly other applications too.
kernel.io_uring_disabled=0
```

Activating without booth:
```
sysctl -f /etc/sysctl.d/10-io_uring.conf
```




