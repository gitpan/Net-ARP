/*
Perl ARP Extension
Get the MAC address of an interface
Linux code

Programmed by Bastian Ballmann
Last update: 09.02.2006

This program is free software; you can redistribute 
it and/or modify it under the terms of the 
GNU General Public License version 2 as published 
by the Free Software Foundation.

This program is distributed in the hope that it will 
be useful, but WITHOUT ANY WARRANTY; without even 
the implied warranty of MERCHANTABILITY or FITNESS 
FOR A PARTICULAR PURPOSE. 
See the GNU General Public License for more details. 
*/

#include <sys/ioctl.h>
#include <net/ethernet.h>    
#include <net/if.h>

int get_mac_linux(u_char *dev, char *mac)
{
  int sock;
  struct ifreq iface;
  struct sockaddr_in *addr;
  struct ether_addr ether;
  
  if(strlen(mac) > 0)
    strcpy(mac,"unknown");
  else
    return -1;

  if(strlen(dev) == 0)
    return -1;
  
  strcpy(iface.ifr_name,dev);
  
  // Open a socket
  if((sock = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
    {
      perror("socket");
      exit(1);
    }
  else
    {
      // Get the interface hardware address
      if((ioctl(sock, SIOCGIFHWADDR, &iface)) < 0)
	{
	  perror("ioctl SIOCGIFHWADDR");
	  exit(1);
	}
      else
	{
	  sprintf(mac,"%02x:%02x:%02x:%02x:%02x:%02x",
		  iface.ifr_hwaddr.sa_data[0] & 0xff, 
		  iface.ifr_hwaddr.sa_data[1] & 0xff,
		  iface.ifr_hwaddr.sa_data[2] & 0xff, 
		  iface.ifr_hwaddr.sa_data[3] & 0xff,
		  iface.ifr_hwaddr.sa_data[4] & 0xff, 
		  iface.ifr_hwaddr.sa_data[5] & 0xff);	
	}
    }

  return 0;
}
