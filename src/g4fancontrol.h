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
#include "IOI2C.h"

#define WRITE_ERROR_CODE      1
#define WRITE_ERROR_MESSAGE 0

#define MODE_STATS        1
#define MODE_SET_GLOBAL 0
#define MODE_ILSTATS      2
#define MODE_SET_SINGLE 3

void printCFString(CFStringRef string);
void getStringFromCF(CFStringRef string, char *buf, int count);
void printUsage(char *msg);
int is_int(char *arg );
void checkRes (IOReturn res, char *msg_success, char *msg_error, int mode, int _exit);
int probe_IOI2CControllerPPC (int set_s0,int set_s1,int set_s2, int mode);
int probe_PPCI2CInterface(int set_s0,int set_s1,int set_s2, int mode);


typedef struct
{
    UInt32 i2c_controller;
    UInt8 i2c_bus;
    UInt8 i2c_device;
    I2CDeviceRef *dev;
    char  device_name[32];
    UInt8 device_registers[6]; 
} IOI2CControllerPPC_Addresses;

typedef struct
{
    UInt32 i2c_controller;
    UInt8 i2c_bus;
    I2CInterfaceRef *iface;
    UInt8 i2c_device;
    char  device_name[32];
    UInt8 device_registers[6]; 
} PPCI2CInterface_Addresses;