/*
Perl ARP Extension
Get the MAC address of an interface
BSD code

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

#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <ifaddrs.h>

int get_mac_bsd(u_char *dev, char *mac)
{
  struct ifaddrs *iface;

  if(strlen(mac) > 0)
    strcpy(mac,"unknown");
  else
    return -1;

  if(strlen(dev) == 0)
    return -1;

  strcpy(mac,"unkown");
  getifaddrs(&iface);
  
  while(iface->ifa_next != NULL)
    { 
      if(!strcmp(iface->ifa_name,dev))
	{
	  if(iface->ifa_addr->sa_family != AF_INET)
	    {
	      sprintf(mac,"%02x:%02x:%02x:%02x:%02x:%02x",	
		      iface->ifa_addr->sa_data[0] & 0xff, 
		      iface->ifa_addr->sa_data[1] & 0xff,
		      iface->ifa_addr->sa_data[2] & 0xff, 
		      iface->ifa_addr->sa_data[3] & 0xff,
		      iface->ifa_addr->sa_data[4] & 0xff, 
		      iface->ifa_addr->sa_data[5] & 0xff);
	      break;
	    }
	}
      
      iface = iface->ifa_next;	
    }

  return 0;
}
