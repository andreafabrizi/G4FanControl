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
#include "g4fancontrol.h"

/*
 * Read 1 byte from the controller
 */
void read_PPCI2CInterface(I2CInterfaceRef *iface, UInt8 i2c_bus, UInt32 controller, UInt8 subaddress, UInt8 *readBuf, char *success_message, char *error_message)
{
    IOReturn res;
    
    res = readI2CInterface(*iface,i2c_bus,controller,subaddress,readBuf,1,kI2CMode_Combined);     
    if (res ==0)
        printf("\n # %s: %d°\n", success_message, readBuf[0]);
    else 
    {
        char *tmp_string = malloc(1024);
        error_message[1000] = 0x0;
        sprintf(tmp_string,"\n # %s", error_message);
        checkRes (res, NULL, tmp_string, WRITE_ERROR_CODE, 0);
        free(tmp_string);
    }
}


/*
 * Controller probing
 */
void find_PPCI2CInterface_Addresses(PPCI2CInterface_Addresses *address)
{
    UInt32 i2c_controller_array[1] = {0xf8001000};    //i
    UInt8 i2c_bus_array[3] = {0x0,0x1,0x2};              //j
    UInt32 device_array[2] = {0x15c,0x5c};               //k
    
    int i, j, k;
    IOReturn res_open,res_read;
    char *iface_path_chr = malloc(1024);
    CFStringRef iface_path;
    I2CInterfaceRef *iface = malloc(8);
    UInt8 *readBuf = malloc(1);
    
    for (i=0; i<1; i++)
        for (j=0; j<3; j++)
            for (k=0; k<2; k++)
            {
                   sprintf(iface_path_chr,"IOService:/MacRISC2PE/uni-n@f8000000/AppleUniN/i2c@%x/PPCI2CInterface",i2c_controller_array[i]);
                   iface_path = CFStringCreateWithCString(kCFAllocatorDefault,iface_path_chr,kCFStringEncodingUTF8);

                    //printf("%s\n", iface_path_chr);
                    res_open = openI2CInterface(iface_path,iface);
                    res_read = readI2CInterface(*iface,i2c_bus_array[j],device_array[k],0x26,readBuf,1,kI2CMode_Combined); //Provo anche una lettura

                    if ((res_open == 0) && (res_read == 0))
                    {
                        address->i2c_controller = i2c_controller_array[i];
                        address->i2c_bus = i2c_bus_array[j];
                        address->i2c_device = device_array[k];
                        address->iface = iface;
                        
                        UInt8 device_registers[6] = {0x26,0x25,0x27,0x68,0x67,0x69}; //TEMP_L, TEMP_1, TEMP_2, LIM_L, LIM_1, LIM_2
                        memcpy(address->device_registers,device_registers,sizeof(address->device_registers));
                        strncpy(address->device_name,"ADT746x",32);              
                        
                        free(iface_path_chr);
                        free(readBuf);
                        return;
                    }
            }
    
    address->iface = 0x0;
    free(iface_path_chr);
    free(readBuf);

}

int probe_PPCI2CInterface (int set_s0,int set_s1,int set_s2, int mode)
{
    I2CInterfaceRef *iface;
    UInt8 *readBuf = malloc(1);
    UInt8 *writeBuf = malloc(1);
    IOReturn res;
    UInt32 i2c_controller, i2c_device;
    UInt8 i2c_bus;
    PPCI2CInterface_Addresses *address = malloc (1024);
    char *device_name;
    
    printf (" - Probing for PPCI2CInterface interface... ");

    find_PPCI2CInterface_Addresses(address);
    
    i2c_controller = address->i2c_controller;
    i2c_bus = address->i2c_bus;
    i2c_device =  address->i2c_device;
    iface = address->iface;
    device_name = address->device_name;
    
    if (iface !=NULL)
        printf("found @ 0x%x\n - Bus I2C @ 0x%x\n - Controller (%s) @ 0x%x\n", i2c_controller,i2c_bus,device_name,i2c_device);
    else 
    {
        printf("not found\n");
        return -1;
    }

    
    //STATS MODE
    if (mode == MODE_STATS)
    {

            read_PPCI2CInterface(iface, i2c_bus, i2c_device,address->device_registers[0], readBuf, "Local Sensor temperature", "Unable to access to local Sensor temperature register");
            read_PPCI2CInterface(iface, i2c_bus, i2c_device,address->device_registers[1], readBuf, "Remote Sensor (1) temperature", "Unable to access to remote Sensor (1) temperature register");
            read_PPCI2CInterface(iface, i2c_bus, i2c_device,address->device_registers[2], readBuf, "Remote Sensor (2) temperature", "Unable to access to remote Sensor (2) temperature register");
            printf("\n");
            read_PPCI2CInterface(iface, i2c_bus, i2c_device,address->device_registers[3], readBuf, "Local Sensor temperature limit", "Unable to access to local Sensor temperature limit register");
            read_PPCI2CInterface(iface, i2c_bus, i2c_device,address->device_registers[4], readBuf, "Remote Sensor (1) temperature limit", "Unable to access to remote Sensor (1) temperature limit register");
            read_PPCI2CInterface(iface, i2c_bus, i2c_device,address->device_registers[5], readBuf, "Remote Sensor (2) temperature limit", "Unable to access to remote Sensor (2) temperature limit register");
  
    }
    else
    
    //SET MODE
    if ((mode == MODE_SET_GLOBAL) || (mode == MODE_SET_SINGLE))
    {
    
            if (set_s0 != 0)
            {
                writeBuf[0] = set_s0;
                printf ("\n > Setting local Sensor temperature limit to %d°... ", set_s0 );
                res = writeI2CInterface(*iface,i2c_bus,i2c_device,address->device_registers[3],writeBuf,1,kI2CMode_StandardSub); 
                checkRes (res, "OK", "Error", WRITE_ERROR_CODE, 0);
            }

            if (set_s1 != 0)
            {
                writeBuf[0] = set_s1;
                printf (" > Setting remote Sensor (1) temperature limit to %d°... ", set_s1 );
                res = writeI2CInterface(*iface,i2c_bus,i2c_device,address->device_registers[4],writeBuf,1,kI2CMode_StandardSub); 
                checkRes (res, "OK", "Error", WRITE_ERROR_CODE, 0);
            }

            if (set_s2 != 0)
            {
                writeBuf[0] = set_s2;
                printf (" > Setting remote Sensor (2) temperature limit to %d°... ", set_s2 );
                res = writeI2CInterface(*iface,i2c_bus,i2c_device,address->device_registers[5],writeBuf,1,kI2CMode_StandardSub); 
                checkRes (res, "OK", "Error", WRITE_ERROR_CODE, 0);
            }
            
    }
    else

    //IN LINE STATS MODE
    if (mode == MODE_ILSTATS)
    {

            // [LS TEMP] [RS1 TEMP] [RS2 TEMP] [LS LIMIT] [RS1 LIMIT] [RS2 LIMIT]

            printf ("ILSTATS ");

            res = readI2CInterface(*iface,i2c_bus,i2c_device,address->device_registers[0],readBuf,1,kI2CMode_Combined);     
            if (res ==0) printf(" %d ", readBuf[0]);
            else printf(" # ");

            res = readI2CInterface(*iface,i2c_bus,i2c_device,address->device_registers[1],readBuf,1,kI2CMode_Combined);     
            if (res ==0) printf(" %d ", readBuf[0]);
            else printf(" # ");
 
            res = readI2CInterface(*iface,i2c_bus,i2c_device,address->device_registers[2],readBuf,1,kI2CMode_Combined);     
            if (res ==0) printf(" %d ", readBuf[0]);
            else printf(" # ");
    
            res = readI2CInterface(*iface,i2c_bus,i2c_device,address->device_registers[3],readBuf,1,kI2CMode_Combined);     
            if (res ==0) printf(" %d ", readBuf[0]);
            else printf(" # ");

            res = readI2CInterface(*iface,i2c_bus,i2c_device,address->device_registers[4],readBuf,1,kI2CMode_Combined);     
            if (res ==0) printf(" %d ", readBuf[0]);
            else printf(" # ");
    
            res = readI2CInterface(*iface,i2c_bus,i2c_device,address->device_registers[5],readBuf,1,kI2CMode_Combined);     
            if (res ==0) printf(" %d ", readBuf[0]);
            else printf(" # ");
            
    }
    printf ("\n - Closing Interface... ");
    res = closeI2CInterface(*iface);
    checkRes (res, "OK\n", "Error", WRITE_ERROR_CODE, 0);
    
    free (iface);
    free(readBuf);
    free(writeBuf);
    free(address);
    
    return 0;
    
}
