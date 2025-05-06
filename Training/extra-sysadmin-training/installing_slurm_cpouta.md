# Installing Slurm and necessary libraries on a CentOS cloud system

In this walkthrough, we will describe the process of installing necessary
components to create a working cluster login node on a cPouta-type cloud
platform.

The node we create in this walkthrough will work as a sort of "login-node", from which
we can log in to our future compute nodes. This node will also host the *Slurm controller daemon*.

## Initialize a single node in cPouta

Follow the steps in [the earlier walkthrough](cpouta-init-node.md) to create
a single computer node in cPouta.  
Initialize it with a "standard.small" flavor at this point.

This instruction assumes a **CentOS 7** operating system.

Log in to the node you created.

## Adding right package repositories (if needed)

Some of the packages that we will need might not be available by default. If you can not install some of the packages you can add EPEL repositories by running commands corresponding to right operating system from [here](https://docs.fedoraproject.org/en-US/epel/).

For example, if your image is CentOS Stream 8 the required commands are:

```
dnf config-manager --set-enabled powertools
dnf install epel-release epel-next-release
```

## User name

At some points in these instructions commands refer to the home folder of the user. Later when logged as root `$USER == root` so we can not use `$USER` in the commands, so at this point we define new variable which will be used in later commands:

```
export USER_NAME=<user_name>
```

where `<user_name>` is the user name which you used to ssh into the node (`ssh $USER_NAME@<floating_ip>`)


## (Optional) Setting up NFS exports

In cluster systems, we want to have a shared working directory between the nodes,
to enable a working Slurm & MPI.

In this example, we'll set up a shared file system that will sync a `/work` path across
nodes, enabling easy data-syncing for parallel programs.

For most steps we will need root access. Enable your sudo access with e.g. `sudo -i`.

Create a `/work` folder we will share across nodes:

```
mkdir -p /work
mkdir -p /work/tmp
```

Install NFS:

```
yum install -y nfs-utils
```

Write the following commands to your `/etc/exports` file:

```
/work 192.168.1.0/22(rw,sync,no_root_squash)
```

This file will tell the system what paths it can export over NFS.  
Here, we export the `/work` path over the local network.  

Next, we export the file system, enable the nfs-server on boot, and restart for good measure:

```
exportfs -a
systemctl enable nfs-server
systemctl restart nfs-server
```

Now we leave it be, until we're ready to mount the file system from our compute nodes at a later stage.

## SLURM

One of the major steps in cluster management is installing Slurm.  
This walkthrough will mainly follow the instructions described here: [https://southgreenplatform.github.io/trainings/hpc/slurminstallation/](https://southgreenplatform.github.io/trainings/hpc/slurminstallation/).  
Setting up Slurm will consist of roughly the following steps:

1. Installing authentication and database software
	- Munge
	- MariaDB
2. Installing Slurm
3. Creating and configuring the Slurm databases
4. Define the Slurm database configuration files
5. Starting the Slurm database
6. Defining our cluster configuration
7. Starting the Slurm controller daemon
X. Adding nodes to the Slurm configuration (and to our cluster)

Let's take this step-by-step. We start by installing the necessary authentication software.

### 1a. Creating a munge authentication key

Set up munge and slurm users on your machine:

```
export MUNGEUSER=1001
groupadd -g $MUNGEUSER munge
useradd  -m -c "MUNGE Uid 'N' Gid Emporium" -d /var/lib/munge -u $MUNGEUSER -g munge  -s /sbin/nologin munge
export SLURMUSER=1002
groupadd -g $SLURMUSER slurm
useradd  -m -c "SLURM workload manager" -d /var/lib/slurm -u $SLURMUSER -g slurm  -s /bin/bash slurm
```

Install munge:
```
yum install munge munge-libs munge-devel -y
```

Create a munge authentication key, and copy it to the work folder (for enabling later sharing across nodes):

```
/usr/sbin/create-munge-key
cp /etc/munge/munge.key /work/tmp
```

Set up correct access rights, and finally, enable and start the Munge daemon:

```
chown -R munge: /etc/munge/ /var/log/munge/ /var/lib/munge/ /run/munge/
chmod 0700 /etc/munge/ /var/log/munge/ /var/lib/munge/ /run/munge/
chmod 711 /run/munge/

systemctl enable munge
systemctl start munge
```

You can test that it's working with:

```
munge -n | unmunge
```

Now we have installed the needed authentication method and the correct munge key
for setting up the necessary Slurm backend. We continue by enabling a database
backed for Slurm.

### 1b. Installing MariaDB

First, install MariaDB and enable it:

```
yum install mariadb-server -y  
systemctl start mariadb  
systemctl enable mariadb  

```

Finish up the installation by setting up the configuration for MariaDB:

```
mysql_secure_installation
```

### 2. Installing Slurm

Start by installing the following dependencies:

```
yum install perl openssl openssl-devel pam-devel rpm-build numactl numactl-devel hwloc hwloc-devel lua lua-devel readline-devel rrdtool-devel ncurses-devel man2html libibmad libibumad mysql-devel perl-ExtUtils-MakeMaker python3 -y
```

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

### 3. Configuring the Slurm Database

Run the following command to start mysql:

```
mysql -u root -p
```

Give it the following commands:

```
mysql> grant all on slurm_acct_db.* TO 'slurm'@'localhost' identified by 'some_pass' with grant option;
mysql> create database slurm_acct_db;
mysql> exit
```


### 4. Define the Slurm database configuration files

Next, we start modifying Slurm's configuration files. All of Slurm's configuration is held in the path `/etc/slurm`.

We start with Slurm's database configuration in the file `/etc/slurm/slurmdbd.conf`. We need to make it represent our own system.
This configuration file needs to be **identical across all compute nodes**.

An example file would be the following:

```
AuthType=auth/munge
DbdAddr=<host ip>
DbdHost=<hostname>
SlurmUser=slurm
DebugLevel=4
LogFile=/var/log/slurm/slurmdbd.log
PidFile=/var/run/slurmdbd.pid
StorageType=accounting_storage/mysql
StorageHost=<hostname>
StoragePass=some_pass
StorageUser=slurm
StorageLoc=slurm_acct_db
```

Where you need to change the `DbdAddr`, `DbdHost` and `StorageHost` options to represent your own system.
Make sure to give the file (`slurmdbd.conf`) the right access rights (600) and define the owner as the `slurm` user:

```
chmod 600 /etc/slurm/slurmdbd.conf
chown slurm /etc/slurm/slurmdbd.conf
```

After modifying and saving it, copy it also to the `/work/tmp` folder, for easier sharing across compute nodes.

```
cp /etc/slurm/slurmdbd.conf /work/tmp
```

### 5. Starting the Slurm database

With the database settings in check, we can start the Slurm database daemon.

```
systemctl start slurmdbd
systemctl enable slurmdbd
systemctl status slurmdbd
```

### 6. Defining our cluster configuration

Our *master/login node* will work as the *Slurm controller*. It will host the `slurmctld` system daemon.  
The compute nodes will host the `slurmd` system daemon, meaning that they won't control Slurm's usage itself.

At this point modify the Slurm configuration file at `/etc/slurm/slurm.conf` to be the following:

```
# Example slurm.conf file. Please run configurator.html
# (in doc/html) to build a configuration file customized
# for your environment.
#
#
# slurm.conf file generated by configurator.html.
# Put this file on all nodes of your cluster.
# See the slurm.conf man page for more information.
#
ClusterName=cluster
SlurmctldHost=<hostname>
#SlurmctldHost=
#
#DisableRootJobs=NO
#EnforcePartLimits=NO
#Epilog=
#EpilogSlurmctld=
#FirstJobId=1
#MaxJobId=67043328
#GresTypes=
#GroupUpdateForce=0
#GroupUpdateTime=600
#JobFileAppend=0
#JobRequeue=1
#JobSubmitPlugins=lua
#KillOnBadExit=0
#LaunchType=launch/slurm
#Licenses=foo*4,bar
#MailProg=/bin/mail
#MaxJobCount=10000
#MaxStepCount=40000
#MaxTasksPerNode=512
MpiDefault=none
#MpiParams=ports=#-#
#PluginDir=
#PlugStackConfig=
#PrivateData=jobs
ProctrackType=proctrack/cgroup
#Prolog=
#PrologFlags=
#PrologSlurmctld=
#PropagatePrioProcess=0
#PropagateResourceLimits=
#PropagateResourceLimitsExcept=
#RebootProgram=
ReturnToService=1
SlurmctldPidFile=/var/run/slurmctld.pid
SlurmctldPort=6817
SlurmdPidFile=/var/run/slurmd.pid
SlurmdPort=6818
SlurmdSpoolDir=/var/spool/slurmd
SlurmUser=slurm
#SlurmdUser=root
#SrunEpilog=
#SrunProlog=
StateSaveLocation=/var/spool/slurmctld
SwitchType=switch/none
#TaskEpilog=
TaskPlugin=task/affinity
#TaskProlog=
#TopologyPlugin=topology/tree
#TmpFS=/tmp
#TrackWCKey=no
#TreeWidth=
#UnkillableStepProgram=
#UsePAM=0
#
#
# TIMERS
#BatchStartTimeout=10
#CompleteWait=0
#EpilogMsgTime=2000
#GetEnvTimeout=2
#HealthCheckInterval=0
#HealthCheckProgram=
InactiveLimit=0
KillWait=30
#MessageTimeout=10
#ResvOverRun=0
MinJobAge=300
#OverTimeLimit=0
SlurmctldTimeout=120
SlurmdTimeout=300
#UnkillableStepTimeout=60
#VSizeFactor=0
Waittime=0
#
#
# SCHEDULING
#DefMemPerCPU=0
#MaxMemPerCPU=0
#SchedulerTimeSlice=30
SchedulerType=sched/backfill
SelectType=select/cons_tres
SelectTypeParameters=CR_Core
#
#
# JOB PRIORITY
#PriorityFlags=
#PriorityType=priority/basic
#PriorityDecayHalfLife=
#PriorityCalcPeriod=
#PriorityFavorSmall=
#PriorityMaxAge=
#PriorityUsageResetPeriod=
#PriorityWeightAge=
#PriorityWeightFairshare=
#PriorityWeightJobSize=
#PriorityWeightPartition=
#PriorityWeightQOS=
#
#
# LOGGING AND ACCOUNTING
#AccountingStorageEnforce=0
AccountingStorageHost=localhost
AccountingStoragePass=indyscc
#AccountingStoragePort=
#AccountingStorageType=accounting_storage/none
AccountingStorageUser=slurm
#AccountingStoreFlags=
#JobCompHost=
#JobCompLoc=
#JobCompPass=
#JobCompPort=
JobCompType=jobcomp/none
#JobCompUser=
#JobContainerType=job_container/none
JobAcctGatherFrequency=30
JobAcctGatherType=jobacct_gather/none
SlurmctldDebug=info
SlurmctldLogFile=/var/log/slurmctld.log
SlurmdDebug=info
SlurmdLogFile=/var/log/slurmd.log
#SlurmSchedLogFile=
#SlurmSchedLogLevel=
#DebugFlags=
#
#
# POWER SAVE SUPPORT FOR IDLE NODES (optional)
#SuspendProgram=
#ResumeProgram=
#SuspendTimeout=
#ResumeTimeout=
#ResumeRate=
#SuspendExcNodes=
#SuspendExcParts=
#SuspendRate=
#SuspendTime=
#
#
# COMPUTE NODES

# Template line for adding node:
# NodeName=compute-node CPUs=<N_CPU> Sockets=<N_SOCKETS> CoresPerSocket=<N> ThreadsPerCore=<N_HYPERTHREADING> State=UNKNOWN

## Example with a "standard.small" flavor
PartitionName=batch Nodes=ALL Default=YES MaxTime=INFINITE State=UP
```

With the corresponding <hostname> and compute node names as in our cluster. As we don't have any compute nodes set up at this stage, you can leave the `NodeName` row untouched. When settin up compute nodes by following instruction in **adding-nodes.md** we will add NodeName rows that represent the compute nodes.

Next, we will also set up a file in `/etc/slurm/cgroup.conf` with the following information:

```
# Slurm cgroup support configuration file
#
# See man slurm.conf and man cgroup.conf for further
# information on cgroup configuration parameters
#--
CgroupAutomount=yes

ConstrainCores=no
ConstrainRAMSpace=no
```

Again, copy these configuration files to the `/work/tmp` path for sharing it across nodes later (**notice** that at this point `/etc/slurm/slurm.conf` might now be in the final form due to missing nodes):

```
cp /etc/slurm/slurm.conf /work/tmp
cp /etc/slurm/cgroup.conf /work/tmp
```

Lastly, we create folders for hosting Slurm logs:

```
mkdir /var/spool/slurmctld
chown slurm:slurm /var/spool/slurmctld
chmod 755 /var/spool/slurmctld
mkdir  /var/log/slurm
touch /var/log/slurm/slurmctld.log
touch /var/log/slurm/slurm_jobacct.log /var/log/slurm/slurm_jobcomp.log
chown -R slurm:slurm /var/log/slurm/
```


### 7. Starting the Slurm controller daemon

Finally, we can start the Slurm controller daemon:

```
systemctl enable slurmctld.service
systemctl start slurmctld.service
systemctl status slurmctld.service
```

**Notice** that at this point `slurmctld` daemon will fail to start due to `/etc/slurm/slurm.conf` not having any nodes defined. One can define new nodes by following instructions in **adding-nodes.md**

After adding atleast one compute node `slurmctld` will start succesfully.
