# Adding compute nodes to your cluster in cPouta

After [initializing a login node](cpouta-init-node.md) and [setting up Slurm (and NFS)](installing_slurm.md), you're ready to add compute nodes to your cluster.

## Create a compute node in cPouta

First, we create a node in cPouta similarly as to your login node:    

0. Generate ssh key on on the login node and create key-pair using it
1. Choose a fitting name (in this tutorial we'll use the name "compute-node")
2. Use the "standard.small" image  
3. Use the previously created key-pair, so we can ssh to the compute node from the login node
4. Use the same security groups as your login node  

**Do not assign a floating IP for your compute node.** You can access your compute nodes through the local network from your login node.  

Wait for your node to initialize.

## Logging in to the compute node, setting up NFS

Next, we will log in to the compute node through our login node. We will also sync the `/work` path from our login node to the compute node through NFS.  

First, let's modify the file `/etc/hosts` to contain information about our compute node. Modify the file with to contain your compute node's IP address similarly to the following:  

```
127.0.0.1   localhost localhost.localdomain localhost4 localhost4.localdomain4
::1         localhost localhost.localdomain localhost6 localhost6.localdomain6
<login-node ip> login
<compute-node ip> compute-node
```

From your *login node*, we can ssh into the compute node. Notice that if you created ssh key on login node while logged in as root you must ssh as root so ssh can find right private key or give the private key with `-i <path_to_key>`

```
ssh cloud-user@compute-node
```

On the compute node, enable sharing of the `/work` path between nodes automatically on reboot (replace the name "login" with the name of your login node):

```
echo "login:/work /work nfs defaults 0 0" >> /etc/fstab
```

Install the following dependencies and mount the `/work` path onto the compute node.

```
mkdir -p /work
yum install -y nfs-utils
systemctl enable nfs-server
systemctl restart nfs-server
mount -a
```

Now you should see the `/work` path on your compute node as identical to the login node. Try adding a file somewhere in `/work` on the login node and make sure it synchronizes between nodes.

Make an identical `/etc/hosts` file on the compute node as the one in the login node.

## Including the compute node in the Slurm configuration file

At this point, you should already add the information of the compute node soon to be added to the login node's `/etc/slurm/slurm.conf` file.

Add the line:
```
NodeName=compute-node CPUs=<N_CPU> Sockets=<N_SOCKETS> CoresPerSocket=<N> ThreadsPerCore=<N_HYPERTHREADING> State=UNKNOWN
```

to the file and copy it to the `/work/tmp` folder for later sharing purposes.

## Installing Slurm on a compute node

Next, we install Slurm on the compute node. This process will follow largely identical steps to installing slurm on the head node, with a few slight differences.

Log in to the login node.
Define your username again with:

```
export USER_NAME=<user_name>
```

Install the following dependencies:

```
yum install perl openssl openssl-devel pam-devel rpm-build numactl numactl-devel hwloc hwloc-devel lua lua-devel readline-devel rrdtool-devel ncurses-devel man2html libibmad libibumad mysql-devel perl-ExtUtils-MakeMaker python3 munge munge-libs munge-devel mariadb-server -y
```

### Slurm backend

Run the following commands with your setup to ensure coherent Slurm backend between the login and compute nodes (identical Slurm and Munge users):

```
export MUNGEUSER=1001
groupadd -g $MUNGEUSER munge
useradd  -m -c "MUNGE Uid 'N' Gid Emporium" -d /var/lib/munge -u $MUNGEUSER -g munge  -s /sbin/nologin munge
export SLURMUSER=1002
groupadd -g $SLURMUSER slurm
useradd  -m -c "SLURM workload manager" -d /var/lib/slurm -u $SLURMUSER -g slurm  -s /bin/bash slurm

cp /work/tmp/munge.key /etc/munge

chown -R munge: /etc/munge/ /var/log/munge/ /var/lib/munge/ /run/munge/
chmod 0700 /etc/munge/ /var/log/munge/ /var/lib/munge/ /run/munge/

systemctl enable munge
systemctl start munge
```

### Installing Slurm

Make an install directory and download the latest Slurm tarball there:

```
mkdir -p /home/$USER_NAME/slurm_setup  
cd /home/$USER_NAME/slurm_setup/
wget https://download.schedmd.com/slurm/slurm-22.05.3.tar.bz2
```

(`wget` might not be installed. It can be installed with `yum install wget`)

Build it with:

```
rpmbuild -ta slurm-22.05.3.tar.bz2
```

(this might fail due to missing C compiler. To add one run `yum install gcc`)

This takes about *10 minutes*, after it's done, finish the installation with:

```
cd /root/rpmbuild/RPMS/x86_64/

yum --nogpgcheck localinstall slurm-* -y
```


### Starting the Slurm client


Make sure that you have identical Slurm configuration files between the head node and the compute node. Ensure this by copying the files from the `/work/tmp` folder to the right paths:

```
cp /work/tmp/slurmdbd.conf /etc/slurm/slurmdbd.conf
cp /work/tmp/slurm.conf /etc/slurm/slurm.conf
cp /work/tmp/cgroup.conf /etc/slurm/cgroup.conf
```

Now, you can start the Slurm *client* daemon with:

```
systemctl enable slurmd.service
systemctl start slurmd.service
systemctl status slurmd.service
```

### Using SLURM

Go back to your login node and type in:

```
scontrol update NodeName=compute-node State=RESUME
```

Check that your node is up with the command:

```
sinfo --partition batch
```

(Assuming the node is in a partition named "batch")
