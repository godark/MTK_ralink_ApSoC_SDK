#! /bin/sh
# $Id: //WIFI_SOC/release/SDK_4_1_0_0/source/user/miniupnpd-1.1/netfilter/iptables_init.sh#1 $
IPTABLES=iptables

#change this parameters :
EXTIF=eth0
EXTIP="`LC_ALL=C /sbin/ifconfig $EXTIF | grep 'inet addr' | awk '{print $2}' | sed -e 's/.*://'`"
echo "External IP = $EXTIP"

#adding the MINIUPNPD chain for nat
$IPTABLES -t nat -N MINIUPNPD
#adding the rule to MINIUPNPD
$IPTABLES -t nat -A PREROUTING -d $EXTIP -i $EXTIF -j MINIUPNPD

#adding the MINIUPNPD chain for filter
$IPTABLES -t filter -N MINIUPNPD
#adding the rule to MINIUPNPD
$IPTABLES -t filter -A FORWARD -i $EXTIF -o ! $EXTIF -j MINIUPNPD

