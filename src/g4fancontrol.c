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
#include <getopt.h>
#include "IOI2C.h"
#include "g4fancontrol.h"



int main (int argc, char *argv[])
{
    
    int  min_tmin=20, max_tmin=100, new_tmin=0, interface_res;
    static int mode, set_s0=0, set_s1=0, set_s2=0;
    char *cvalue = NULL;
    int index,  c;
    static int
        mode_set_global = 0,
        mode_set_single = 0,
        mode_stats = 0,
        mode_ilstats = 0;
       
    printf("\n  G4-FanControl v0.4\n\n  Fabrizi Andrea - andrea.fabrizi@gmail.com\n  http://www.andreafabrizi.it\n\n");

    while (1)
    {
        static struct option long_options[] =
        {
            {"stats", no_argument, 0, 'f'},
            {"ilstats", no_argument, 0, 'g'},

            {"set-sensor-0", required_argument, 0, 'a'},
            {"set-sensor-1", required_argument, 0, 'b'},
            {"set-sensor-2", required_argument, 0, 'c'},
            {"set", required_argument, 0, 'd'},
            {0, 0, 0, 0}
        };
           
        int option_index = 0;
     
        c = getopt_long (argc, argv, "",long_options, &option_index);
     
        if (c == -1) break;
     
        switch (c)
        {

            case 'a':
               if ((!is_int(optarg)) || (atoi(optarg) < min_tmin) ||  (atoi(optarg) > max_tmin)) printUsage("  Error: Illegal argument for option \"set-sensor-0\"");
               set_s0 = atoi(optarg);
               mode_set_single = 1;
               mode = MODE_SET_SINGLE;
               break;
     
             case 'b':
               if ((!is_int(optarg)) || (atoi(optarg) < min_tmin) ||  (atoi(optarg) > max_tmin)) printUsage("  Error: Illegal argument for option \"set-sensor-1\"");
               set_s1 = atoi(optarg);
               mode_set_single = 1;
               mode = MODE_SET_SINGLE;
               break;
     
             case 'c':
               if ((!is_int(optarg)) || (atoi(optarg) < min_tmin) ||  (atoi(optarg) > max_tmin)) printUsage("  Error: Illegal argument for option \"set-sensor-2\"");
               set_s2 = atoi(optarg);
               mode_set_single = 1;
               mode = MODE_SET_SINGLE;
               break;
     
             case 'd':
               if ((!is_int(optarg)) || (atoi(optarg) < min_tmin) ||  (atoi(optarg) > max_tmin)) printUsage("  Error: Illegal argument for option \"set\"");
               set_s0 = atoi(optarg);
               set_s1 = atoi(optarg);
               set_s2 = atoi(optarg);
               mode_set_global = 1;
               mode = MODE_SET_GLOBAL;
               break;

             case 'f':
               mode_stats = 1;
               mode = MODE_STATS;
               break;

             case 'g':
               mode_ilstats = 1;
               mode = MODE_ILSTATS;
               break;
               
             default:
                printUsage(NULL);
             }
         }
    
    //Checking parameter's conflicts
    if (mode_set_single + mode_set_global + mode_stats + mode_ilstats == 0) printUsage("");
    if (mode_set_single + mode_set_global + mode_stats + mode_ilstats != 1) printUsage("  Error: Command line arguments conflict");
    
    interface_res = probe_PPCI2CInterface(set_s0,set_s1,set_s2,mode);
    
    if (interface_res == -1)
       interface_res = probe_IOI2CControllerPPC(set_s0,set_s1,set_s2,mode);

    if (interface_res == -1)
    printf("\n   Your MAC is not supported at this time or probably you don't run\n   this application using sudo or root privilegies!\n\n");
    
}

