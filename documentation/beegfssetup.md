# Setting up BeeGFS parallel filesystem

<https://doc.beegfs.io/8.0/architecture/overview.html#architecture>

https://doc.beegfs.io/8.0/advanced_topics/authentication.html#connectionbasedauth

https://doc.beegfs.io/8.0/quick_start_guide/quick_start_guide.html#quickstartguide

https://doc.beegfs.io/8.0/advanced_topics/manual_installation.html

BeeGFS v8.0 for Red Hat Enterprise Linux 9

Binary packages are available for Red Hat Enterprise Linux / Rocky Linux / AlmaLinux 9.

Warning: Before upgrading to this version, make sure to read our upgrade guide very carefully to make sure all the prerequisites are met for the upgrade to proceed smoothly.
Configure Package Repository

BeeGFS repositories and packages are digitally signed. The fingerprint of our signing key is 0683 AF2B 9AC5 BC7D 59B2 D623 40CE 4E24 3CD5 0804.
To verify the package signatures, add the public BeeGFS GPG key to your package manager:
rpm --import https://www.beegfs.io/release/beegfs_8.0/gpg/GPG-KEY-beegfs

then download the repository file:
wget https://www.beegfs.io/release/beegfs_8.0/dists/beegfs-rhel9.repo \
-O /etc/yum.repos.d/beegfs.repo

You can now install BeeGFS via your package manager.
Please refer to the documentation for details on how to install and configure BeeGFS, and see also the Changelog as well as the Upgrade Notes.
Manual Download

Individual rpm packages can be downloaded by browsing the online Repository.

https://doc.beegfs.io/8.0/advanced_topics/manual_installation.html

two node setup: with eth network 172.16.1.7 and 172.16.1.8
( if using IB, one might need a proper names for nodes )
in order to ease out things, in this setup .7 is named chip and .8 dale
edit /etc/hosts :

```
nano -w /etc/hosts
```

add following lines to the end

```
172.16.1.8  dale
172.16.1.7  chip
```

test. from chip node:`ssh dale` should find a route :D

in both nodes, check that you have free space available:

```
[root@chip ~]# lsblk 
NAME                      MAJ:MIN RM   SIZE RO TYPE MOUNTPOINTS
sda                         8:0    0 111.8G  0 disk 
├─sda1                      8:1    0   100G  0 part 
└─sda9                      8:9    0  11.8G  0 part 
sdb                         8:16   0 167.7G  0 disk 
├─sdb1                      8:17   0     1G  0 part /boot
└─sdb2                      8:18   0 166.7G  0 part 
  ├─almalinux_lumi09-root 253:0    0    70G  0 lvm  /
  ├─almalinux_lumi09-swap 253:1    0   7.8G  0 lvm  [SWAP]
  └─almalinux_lumi09-home 253:2    0  88.9G  0 lvm  /home
```

we will use /dev/sda here as an example.
Partition /dev/sda1 will be our storage partition and /dev/sda9 is used for metadata ( note that one might adjust the sizing a lot, this is a small setup)

create filesystems to store the data.

```
[root@chip ~]# mkfs.xfs /dev/sda9
meta-data=/dev/sda9              isize=512    agcount=4, agsize=772637 blks
         =                       sectsz=512   attr=2, projid32bit=1
         =                       crc=1        finobt=1, sparse=1, rmapbt=0
         =                       reflink=1    bigtime=1 inobtcount=1 nrext64=0
data     =                       bsize=4096   blocks=3090545, imaxpct=25
         =                       sunit=0      swidth=0 blks
naming   =version 2              bsize=4096   ascii-ci=0, ftype=1
log      =internal log           bsize=4096   blocks=16384, version=2
         =                       sectsz=512   sunit=0 blks, lazy-count=1
realtime =none                   extsz=4096   blocks=0, rtextents=0
Discarding blocks...Done.
[root@chip ~]# mkfs.xfs /dev/sda1
meta-data=/dev/sda1              isize=512    agcount=4, agsize=6553600 blks
         =                       sectsz=512   attr=2, projid32bit=1
         =                       crc=1        finobt=1, sparse=1, rmapbt=0
         =                       reflink=1    bigtime=1 inobtcount=1 nrext64=0
data     =                       bsize=4096   blocks=26214400, imaxpct=25
         =                       sunit=0      swidth=0 blks
naming   =version 2              bsize=4096   ascii-ci=0, ftype=1
log      =internal log           bsize=4096   blocks=16384, version=2
         =                       sectsz=512   sunit=0 blks, lazy-count=1
realtime =none                   extsz=4096   blocks=0, rtextents=0
Discarding blocks...Done.
```

create mountpoints:
`mkdir /beerawstorage /beemeta /mnt/beegfs`
mount:
`mount /dev/sda1 /beerawstorage`
mount place for metadata only on chip
`mount /dev/sda9 /beemeta`

import beegfs.io GPG-KEY:
`sudo rpm --import https://www.beegfs.io/release/beegfs_8.0/gpg/GPG-KEY-beegfs`
add repo to node:
`sudo curl -o /etc/yum.repos.d/beegfs.repo https://www.beegfs.io/release/beegfs_8.0/dists/beegfs-rhel9.repo`

chip will be the
mgmt+metadata+storage+client node:

`dnf install -y  beegfs-meta beegfs-mgmtd beegfs-storage beegfs-client`

dale will be just a storage+client node:
`dnf install -y beegfs-storage beegfs-client`

if used InfiniBand, Install libbeegfs-ib  for meta and storage nodes.

modify config files ( git diff results of files are in the end of the this document)

most important thing is the sysMgmtdHost address. and if no RDMA is not available set RDMA to false :D

Start Management (chip):
`# /opt/beegfs/sbin/beegfs-mgmtd --init`

strip away security layer by changing /etc/beegfs/beegfs-mgmtd.toml by altering following line and set it to true

```
# Disables TLS for gRPC communication.
 tls-disable = true # default is false
```

```
Created new database version 3 at "/var/lib/beegfs/mgmtd.sqlite".
```

Since management is there, define where to find metadata and storage:

```
[root@chip beegfs]# /opt/beegfs/sbin/beegfs-setup-meta -p /beemeta/ -s 2 -m chip
Preparing storage directory: /beemeta/
 * Creating format.conf file...
 * Creating server numeric ID file: /beemeta//nodeNumID
Updating config file: /etc/beegfs/beegfs-meta.conf
 * Setting management host: chip
 * Setting storage directory in config file...
 * Disabling usage of uninitialized storage directory in config file...
 * Fetching the underlying device...
Underlying device detected: /dev/sda9
Fetching UUID of the file system on that device...
Found UUID f81a22ae-dc07-409e-a439-509897687e1d
Writing UUID to config file...
 * Setting usage of extended attributes to: true
All done.
```

To set up multiple storage devices on the same node, use the same `-s <server-ID>` parameter,
but a different `-i <device-ID>`!

From the [documentation](https://doc.beegfs.io/latest/quick_start_guide/quick_start_guide.html):

> The storage service needs to know where it can store its data and how to reach the
> management server. Typically, you will have multiple storage services running on different
> machines and/or multiple storage targets (e.g., multiple RAID volumes) per storage service.

> Optionally, you can also define a custom numeric storage service ID and numeric storage
> target ID (both in range 1..65535). As this service is running on a server with name node03
> in our example, we will pick number 3 as ID for this storage service and we will use 301
> as storage target ID to show that this is the first target (01) of storage service 3.

> ```txt
> $ ssh root@node03
> # /opt/beegfs/sbin/beegfs-setup-storage -p /mnt/myraid1/beegfs_storage -s 3 -i 301 -m node01
> 
> # /opt/beegfs/sbin/beegfs-setup-storage -p /mnt/myraid2/beegfs_storage -s 3 -i 302
> ```

```
[root@chip beegfs]# /opt/beegfs/sbin/beegfs-setup-storage -p /beerawstorage -s 2 -i 301 -m chip
Preparing storage target directory: /beerawstorage
 * Creating format.conf file...
 * Creating chunks directory...
 * Creating buddymir directory...
 * Creating target numeric ID file: /beerawstorage/targetNumID
 * Creating server numeric ID file: /beerawstorage/nodeNumID
Updating config file: /etc/beegfs/beegfs-storage.conf
 * Setting management host: chip
 * Appending to target directory list in config file...
 * Disabling usage of uninitialized storage targets in config file...
 * Fetching the underlying device...
Underlying device detected: /dev/sda1
Fetching UUID of the file system on that device...
Found UUID 5b29cfd1-e143-446d-abea-653a78c01e25
Appending UUID to config file...
All done.
```

another node needs a different server id and storage id

```
[root@dale beegfs]# /opt/beegfs/sbin/beegfs-setup-storage -p /beerawstorage -s 3 -i 302 -m dale
Preparing storage target directory: /beerawstorage
 * Creating format.conf file...
 * Creating chunks directory...
 * Creating buddymir directory...
 * Creating target numeric ID file: /beerawstorage/targetNumID
 * Creating server numeric ID file: /beerawstorage/nodeNumID
Updating config file: /etc/beegfs/beegfs-storage.conf
 * Setting management host: dale
 * Appending to target directory list in config file...
 * Disabling usage of uninitialized storage targets in config file...
 * Fetching the underlying device...
Underlying device detected: /dev/sda1
Fetching UUID of the file system on that device...
Found UUID 9a80a38f-2604-4cf1-a1f8-9fc9adb24f53
Appending UUID to config file...
All done.
```

setup a client on both nodes:

```
[root@chip beegfs]# /opt/beegfs/sbin/beegfs-setup-client -m chip
Updating config file: /etc/beegfs/beegfs-client.conf
 * Setting management host: chip
All done.
```

define client mounts:

```
[root@chip beegfs]# cat ->/etc/beegfs/beegfs-mounts.conf
/mnt/beegfs /etc/beegfs/beegfs-client.conf
```

( remember to "end" catting by pressing ctrl+d :)

Now everything should be ready to be starting chip:

```
# systemctl start beegfs-mgmtd
# systemctl start beegfs-meta
# systemctl start beegfs-storage
```

If you see errors related to the `/etc/beegfs/conn.auth` file, please
double-check the `auth-disable = true` configuration.

start storage in dale:

```
# systemctl start beegfs-storage
```

after the system is up and running, start the clients:

for testing the owerall setup before starting clients, install
` dnf install beegfs-tools`

and ease out config for the tool by setting some defaults into bashrc

```
echo "export BEEGFS_MGMTD_ADDR='chip:8010'" >> ~/.bashrc
echo "export BEEGFS_TLS_DISABLE='true'" >> ~/.bashrc`
echo "export BEEGFS_AUTH_DISABLE='true'" >> ~/.bashrc
source ~/.bashrc
```

now  it should look something like this ( if it doesn't metadata or storage targets are not starting properly. )

```
[root@dale beegfs]# beegfs health df

## Metadata Targets

ID   TYPE  ALIAS          NODE  STORAGE_POOL  CAP_POOL  SPACE    SPACE_USED        SPACE_FREE  INODES  INODES_USED    INODES_FREE  
m:2  meta  target_meta_2  m:2   (n/a)         Low       11.7GiB  150.4MiB (1.25%)  11.6GiB     6.2M    66.1k (1.07%)  6.1M

---

## Storage Targets

ID     TYPE     ALIAS                NODE  STORAGE_POOL  CAP_POOL  SPACE     SPACE_USED        SPACE_FREE  INODES  INODES_USED   INODES_FREE  
s:301  storage  target_0-682AD65E-2  s:2   s:1           Low       99.9GiB   745.8MiB (0.73%)  99.2GiB     52.4M   12.0 (0.00%)  52.4M  
s:302  storage  target_0-682AD7A2-3  s:3   s:1           Normal    893.8GiB  14.3GiB (1.60%)   879.5GiB    468.8M  17.0 (0.00%)  468.8M

[root@dale beegfs]# beegfs node list
ID    TYPE        ALIAS  
c:9   client      c2EA3-68271449-luminen  
m:2   meta        node_meta_2  
s:2   storage     node_storage_2  
s:3   storage     node_storage_3  
mg:1  management  management
```

Prepare for client installation by installing `dnf install kernel-devel`

start client in chip and dale:

# systemctl start beegfs-client

If you see errors like:
```
systemd[1]: Starting Start BeeGFS Client...
beegfs-client[1341]: Starting BeeGFS Client:
beegfs-client[1341]: - Loading BeeGFS modules
beegfs-client[1341]: - Mounting directories from /etc/beegfs/beegfs-mounts.conf
beegfs-client[1367]: + mount --internal -t beegfs --source beegfs_nodev --target /mnt/beegfs -orw,nosuid,cfgFile=/etc/beegf>
beegfs-client[1387]: mount: /mnt/beegfs: mount(2) system call failed: Operation canceled.
systemd[1]: beegfs-client.service: Main process exited, code=exited, status=32/n/a
systemd[1]: beegfs-client.service: Failed with result 'exit-code'.
systemd[1]: Failed to start Start BeeGFS Client.
```

Then double-check that the authentication is disabled in `/etc/beegfs/beegfs-client.conf`!

now the /mnt/beegfs should contain mounted shared filesystem on both nodes

monitor both nodes  ( like with dstat , dnf install dstat and the just run dstat) and write some data to see if it really works.

```
dd if=/dev/zero of=/mnt/beegfs/kamala.txt bs=512k count=100000
```

config file diffs between originals and edited ( mgmthost ip can be just a node name if node namig  ( /etc/hosts ) is set properly

```
diff --git a/beegfs-meta.conf b/beegfs-meta.conf.rpmsave.oldii
index 5c9ac0b..6bd203b 100644
\--- a/beegfs-meta.conf
+++ b/beegfs-meta.conf.rpmsave.oldii
@@ -14,11 +14,11 @@

# --- Section 1.1: [Basic Settings] ---

# 

\-sysMgmtdHost                 =
+sysMgmtdHost                 = 172.16.1.7

\-storeMetaDirectory           =
\-storeAllowFirstRunInit       = true
\-storeFsUUID                  =
+storeMetaDirectory           = /beefsmetadatadir
+storeAllowFirstRunInit       = false
+storeFsUUID                  = 5f009b09-98e9-4afa-997b-d87ffa81f240
#abowe FSUUID is taken from that dir during the metadata init

# 

@@ -26,7 +26,7 @@ storeFsUUID                  =

# 

connAuthFile                 = /etc/beegfs/conn.auth
\-connDisableAuthentication    = false
+connDisableAuthentication    = true
connBacklogTCP               = 128
connFallbackExpirationSecs   = 900
connInterfacesFile           =
```

```
git diff beegfs-storage.conf beegfs-storage.conf.rpmsave.oldii
```

```
diff --git a/beegfs-storage.conf b/beegfs-storage.conf.rpmsave.oldii
index f11f93b..ef736bf 100644
\--- a/beegfs-storage.conf
+++ b/beegfs-storage.conf.rpmsave.oldii
@@ -14,10 +14,10 @@

# --- Section 1.1: [Basic Settings] ---

# 

\-sysMgmtdHost                 =
+sysMgmtdHost                 = 172.16.1.7

\-storeStorageDirectory        =
\-storeAllowFirstRunInit       = true
+storeStorageDirectory        = /beefsrawstorage
+storeAllowFirstRunInit       = false
storeFsUUID                  =

@@ -26,7 +26,7 @@ storeFsUUID                  =

# 

connAuthFile                 = /etc/beegfs/conn.auth
\-connDisableAuthentication    = false
+connDisableAuthentication    = true
connBacklogTCP               = 128
connInterfacesFile           =
connMaxInternodeNum          = 12
```

[root@chip beegfs]# more beegfs-mounts.conf
/mnt/beegfs /etc/beegfs/beegfs-client.conf

````
diff --git a/beegfs-client.conf b/beegfs-client.conf.oldii
index 4557a26..7452be5 100644
\--- a/beegfs-client.conf
+++ b/beegfs-client.conf.oldii
@@ -9,12 +9,11 @@

# 3) Basic Settings Documentation
# 4) Advanced Settings Documentation

- 

# 
# --- Section 1.1: [Basic Settings] ---
# 

\-sysMgmtdHost                  =
+sysMgmtdHost                  = 172.16.1.7

# 

@@ -22,7 +21,7 @@ sysMgmtdHost                  =

# 

connAuthFile                  = /etc/beegfs/conn.auth
\-connDisableAuthentication     = false
+connDisableAuthentication     = true
connClientPort                = 8004
connMgmtdPort                 = 8008
connPortShift                 = 0
@@ -35,7 +34,7 @@ connMaxInternodeNum           = 12
connMaxConcurrentAttempts     = 0
connNetFilterFile             =

\-connUseRDMA                   = true
+connUseRDMA                   = false

# this old setup didin't have RDMA capability

connTCPFallbackEnabled        = true
connTCPRcvBufSize             = 0
connUDPRcvBufSize             = 0
@@ -46,7 +45,7 @@ connRDMATypeOfService         = 0
connTcpOnlyFilterFile         =

logClientID                   = false
\-logLevel                      = 3
+logLevel                      = 5

quotaEnabled                  = false

@@ -740,3 +739,4 @@ sysACLsEnabled                = false

# md4hash32
# md4hash64
# Default: md4hash64
 
```
````
