#!/usr/bin/perl
#
# Perl ARP Extension test file
#
# Programmed by Bastian Ballmann
# Last update: 31.12.2004
#
# This program is free software; you can redistribute 
# it and/or modify it under the terms of the 
# GNU General Public License version 2 as published 
# by the Free Software Foundation.
#
# This program is distributed in the hope that it will 
# be useful, but WITHOUT ANY WARRANTY; without even 
# the implied warranty of MERCHANTABILITY or FITNESS 
# FOR A PARTICULAR PURPOSE. 
# See the GNU General Public License for more details. 

use ExtUtils::testlib;
use Net::ARP;
use Net::Pcap;

my $errbuf;
my $dev = Net::Pcap::lookupdev(\$errbuf);

#Net::ARP::send_packet($dev,                           # network interface
#		      '127.0.0.1',                    # source ip
#	              '127.0.0.1',                    # destination ip
#		      'aa:bb:cc:aa:bb:cc',            # source mac
#	              'aa:bb:cc:aa:bb:cc',            # destination mac
#	              'reply');                       # ARP operation 

Net::ARP::send_packet($dev,                           # network interface
		      '127.0.0.1',                    # source ip
	              '127.0.0.1',                    # destination ip
		      'aa:bb:cc:aa:bb:cc',            # source mac
	              'ff:ff:ff:ff:ff:ff',            # destination mac
	              'reply');                       # ARP operation 

Net::ARP::get_mac($dev,$mac);
print "MAC $mac\n";
Net::ARP::arp_lookup($dev,"192.168.1.1",$mac);
print "192.168.2.1 has got mac $mac\n";

