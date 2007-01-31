/*
Perl ARP Extension
Lookup the MAC address of an ip address
Linux code

Programmed by Bastian Ballmann and Alexander Mueller
Last update: 20.09.2006

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
#include <string.h>

#define _PATH_PROCNET_ARP "/proc/net/arp"

int arp_lookup_linux(char *dev, char *ip, char *mac)
{
  FILE *fp;
  char ipaddr[100];
  char line[200];
  char hwa[100];
  char mask[100];
  char device[100];
  int num, type, flags;
  
  if(strlen(mac) > 0)
    strcpy(mac,"unknown");
  else
    return -1;

  if(strlen(ip) == 0)
    return -1;
  
  if ((fp = fopen(_PATH_PROCNET_ARP, "r")) == NULL) {
    perror(_PATH_PROCNET_ARP);
    return (-1);
  }
  
  /* Bypass header -- read until newline */
  if (fgets(line, sizeof(line), fp) != (char *) NULL)
    {
      /* Read the ARP cache entries. */
      while (fgets(line, sizeof(line), fp))
	{
	  num = sscanf(line, "%s 0x%x 0x%x %100s %100s %100s\n", ipaddr, &type, &flags, hwa, mask, device);
	  
	  if (num < 4)
	    break;

          if ((strlen(dev) == 0 || strcmp(dev, device) == 0) && strcmp(ip, ipaddr) == 0)
	    {
	      strcpy(mac, hwa);
	      break;
	    }
	  strcpy(mac, "unknown");
	}
    }

    fclose(fp);
}
