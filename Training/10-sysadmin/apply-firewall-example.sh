#!/bin/bash

set -euxo pipefail

# Reset policy to avoid getting locked out
iptables -P INPUT ACCEPT

# Flush all filter table rules
iptables -t filter -F

# Add rules here:
iptables -I INPUT 1 -p tcp -m state --state ESTABLISHED -m comment --comment "Accept established TCP traffic" -j ACCEPT
# Allow anything on local interfaces
iptables -A INPUT -i lo -j ACCEPT
iptables -A OUTPUT -o lo -j ACCEPT

# -> TODO: check what the local subnet is on your servers! In this example, it's 192.168.122.0/24

iptables -A INPUT -i enp1s0 -p tcp --dport 22 -s 192.168.122.0/24 -m state --state NEW -m comment --comment "Accept new LAN SSH" -j ACCEPT
# DNS rules (for example)
iptables -A INPUT -i enp1s0 -p udp --sport 53 -s 192.168.122.0/24 -m state --state ESTABLISHED -m comment --comment "Accept LAN DNS responses" -j ACCEPT
iptables -A OUTPUT -o enp1s0 -p udp --sport 53 -d 192.168.122.0/24 -m state --state NEW,ESTABLISHED -m comment --comment "Accept LAN DNS responses" -j ACCEPT
# DHCP rules (for example)
iptables -A INPUT -i enp1s0 -p udp --sport 67 --dport 68 -s 192.168.122.0/24 -m state --state ESTABLISHED -m comment --comment "Accept LAN DHCP responses" -j ACCEPT
iptables -A OUTPUT -o enp1s0 -p udp --sport 68 --dport 67 -d 192.168.122.0/24 -m state --state NEW,ESTABLISHED -m comment --comment "Accept LAN DHCP requests" -j ACCEPT
# NTP rules (if it should be allowed from the Internet)
iptables -A INPUT -i enp1s0 -p udp --sport 123 -m state --state ESTABLISHED -m comment --comment "Accept NTP responses" -j ACCEPT
iptables -A OUTPUT -o enp1s0 -p udp --sport 123 -m state --state NEW,ESTABLISHED -m comment --comment "Accept NTP output" -j ACCEPT
# Allow pings
iptables -A INPUT -p icmp -m comment --comment "Accept ping input" -j ACCEPT
iptables -A OUTPUT -p icmp -m comment --comment "Accept ping output" -j ACCEPT

# Allow BeeGFS access:
# - Management (this example server is the management host)
iptables -A INPUT -i enp1s0 -s 192.168.122.0/24 -p tcp --dport 8008 -m comment --comment "Accept beegfs management TCP traffic A" -j ACCEPT
iptables -A INPUT -i enp1s0 -s 192.168.122.0/24 -p udp --dport 8008 -m comment --comment "Accept beegfs management UDP traffic A" -j ACCEPT
iptables -A INPUT -i enp1s0 -s 192.168.122.0/24 -p tcp --dport 8010 -m comment --comment "Accept beegfs management TCP traffic B" -j ACCEPT
iptables -A INPUT -i enp1s0 -s 192.168.122.0/24 -p udp --dport 8010 -m comment --comment "Accept beegfs management UDP traffic B" -j ACCEPT
# - Metadata (this example server is the metadata host)
iptables -A INPUT -i enp1s0 -s 192.168.122.0/24 -p tcp --dport 8005 -m comment --comment "Accept beegfs metadata TCP traffic" -j ACCEPT
iptables -A INPUT -i enp1s0 -s 192.168.122.0/24 -p udp --dport 8005 -m comment --comment "Accept beegfs metadata UDP traffic" -j ACCEPT
# - Storage
iptables -A INPUT -i enp1s0 -s 192.168.122.0/24 -p tcp --dport 8003 -m comment --comment "Accept beegfs storage TCP traffic" -j ACCEPT
iptables -A INPUT -i enp1s0 -s 192.168.122.0/24 -p udp --dport 8003 -m comment --comment "Accept beegfs storage UDP traffic" -j ACCEPT
# - Client
iptables -A INPUT -i enp1s0 -s 192.168.122.0/24 -p tcp --dport 8004 -m comment --comment "Accept beegfs client TCP traffic" -j ACCEPT
iptables -A INPUT -i enp1s0 -s 192.168.122.0/24 -p udp --dport 8004 -m comment --comment "Accept beegfs client UDP traffic" -j ACCEPT




# Log offenders as the last rule:
iptables -A INPUT -m limit --limit 5/min -j LOG --log-prefix "iptables drop input " --log-level 7

# Set the policy back to DROP
iptables -P INPUT DROP

echo "Reapplied firewall rules"
echo "Updating /etc/sysconfig/iptables"
iptables-save > /etc/sysconfig/iptables

# IPv6: disable all input, just in case
ip6tables -P INPUT DROP
ip6tables -P FORWARD DROP
ip6tables-save > /etc/sysconfig/ip6tables

# Write a message to the kernel logs to indicate when rules were updated.
# This is easy to spot among the "iptables drop" messages in the kernel logs.
echo "$0 was executed successfully" > /dev/kmsg

