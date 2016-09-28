/*
	G4FanControl
 
	Copyright (c) 2007, Andrea Fabrizi <andrea.fabrizi@gmail.com>
	http://www.andreafabrizi.it
 
	This program is free software; you can redistribute it and/or
	modify it under the terms of the GNU General Public License as
	published by the Free Software Foundation; either version 2 of
	the License, or (at your option) any later version.
 
	This program is distributed in the hope that it will be
	useful, but WITHOUT ANY WARRANTY; without even the implied
	warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
	PURPOSE.  See the GNU General Public License for more details.
 
	You should have received a copy of the GNU General Public
	License along with this program; if not, write to the Free
	Software Foundation, Inc., 59 Temple Place, Suite 330, Boston,
	MA 02111-1307, USA.
*/

#include <string.h>
#include <stdio.h>
#include "g4fancontrol.h"


void printCFString(CFStringRef string)
{
    CFIndex	len;
    char *	buffer;

    len = CFStringGetLength(string) + 1;
    buffer = malloc(len);
    if (buffer && CFStringGetCString(string, buffer, len,
                                  CFStringGetSystemEncoding()) )
	printf(buffer);

    if (buffer)
	free(buffer);
}

void getStringFromCF(CFStringRef string, char *buf, int count)
{
    CFIndex	len;
    char *	buffer;

    len = CFStringGetLength(string) + 1;
    buffer = malloc(len);
    if (buffer && CFStringGetCString(string, buffer, len,
                                  CFStringGetSystemEncoding()) )
    
    strncpy(buf,buffer,count);

    if (buffer)
	free(buffer);
}

void printUsage(char *msg)
{
    printf("%s\n\n"
             "  Usage:\n"
             "   ./g4fancontrol [options]\n\n"
             "   --stats\n\tShows information about the current temperatures and limits\n\n"
             "   --ilstats\n\tLists the \"in-line\" stats (use ./g4fancontrol --ilstats | grep ILSTATS)\n\n"
             "   --set <temp>\n\tSets the temperature sensor limits to <temp>\n\n"
             "   --set-sensor-0 <temp>\n\tSets the temperature limit for the local sensor to <temp>\n\n"
             "   --set-sensor-1 <temp>\n\tSets the temperature limit for the remote sensor(1) to <temp>\n\n"
             "   --set-sensor-2 <temp>\n\tSets the temperature limit for the remote sensor(2) to <temp>\n\n"
             "   <temp>: Value must be between 20 and 100\n\n"
             ,msg);
   
    exit(0);
}

void die(char *msg)
{
    printf("%s", msg);
    exit(1);
}

int is_int(char *arg )
{
	char *s;
	for (s=arg; *s; s++) 
	{
		if (*s < '0' || *s > '9')
			return 0;
	}
	return 1;
}

/*
 * Error messages
 */
void checkRes (IOReturn res, char *msg_success, char *msg_error, int mode, int _exit)
{
    if ((res != 0) && (mode == WRITE_ERROR_CODE))
    {
        printf ("%s (error code 0x%x)\n", msg_error,res);
        if (_exit) exit(1);
    } else if ((res != 0) && (mode == WRITE_ERROR_MESSAGE))
    {
        printf ("%s\n", msg_error);
        if (_exit) exit(1);
    } 
    else printf("%s\n", msg_success);
}
