/*
 ============================================================================
 Name        : lsusb.c
 Author      : 
 Version     :
 Copyright   : Your copyright notice
 Description : Hello World in C, Ansi-style
 ============================================================================
 */

#include <string>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <glob.h>
#include <limits.h>



int main_readlink(int argc, char *argv[]);
int libusb_printDevice(void);
int main_hotplugtest(int argc, char *argv[]);
int main_udev(void);
int main_ioctl(int argc, char *argv[]);
int main_hidraw(int argc, char **argv);
int main_printdevice(int argc, char *argv[]);
int main_tu_Create(int argc, char *argv[]);




static void PrintUsage()
{
	printf("usage [command] [args...] \n");
	printf("\t ioctl        /dev/input/input3 \n");
	printf("\t readlink     /dev/input/input12 \n");
	printf("\t hotplug      vendor_id product_id class_id \n");
	printf("\t udev \n");
	printf("\t hidraw        /sys/class/hidraw/hidraw0 \n");
	printf("\t printdevice   -v \n");
	printf("\t tu_create \n");


}

static  int ExecCommand( int argc, char *argv[] )
{
	int result = 0;

	if( argc > 1 )
	{
		if( strcmp(argv[1],"ioctl" ) == 0 )
		{
			result = main_ioctl(argc,argv);
		}
		else if(strcmp(argv[1],"readlink" ) == 0 )
		{
			result = main_readlink( argc , argv );
		}
		else if(strcmp(argv[1],"hotplug" ) == 0 )
		{
			result = main_hotplugtest( argc , argv );
		}
		else if(strcmp(argv[1],"udev" ) == 0 )
		{
			result = main_udev();
		}
		else if(strcmp(argv[1],"hidraw" ) == 0 )
		{
			result = main_hidraw( argc , argv );
		}
		else if(strcmp(argv[1],"printdevice" ) == 0 )
		{
			result = main_printdevice( argc , argv );
		}
		else if(strcmp(argv[1],"tu_create" ) == 0 )
		{
			result = main_tu_Create( argc , argv );
		}
		else
		{
			PrintUsage();
		}

		if( result == -1 )
		{
			PrintUsage();
		}
	}
	else
	{
		PrintUsage();
	}

	return result;

}
int getInputEventPath( )
{
#define CONFIG_PC 1

	int 		result 		= 0;
	char 		PathDevice [PATH_MAX] = "/sys/bus/hid/devices/*";
	char 		PathDeviceReal [PATH_MAX] = "/sys/bus/hid/devices/*";
	char 		PathInput[PATH_MAX];
	char 		PathInputReal[PATH_MAX];
	char 		PathEvent[PATH_MAX];
	char 		EventFilename[PATH_MAX];


	// /sys/bus/hid/device/003:cID:VID:pID:nID
	// /sys/bus/hid/device/0003:05E0:1200:0001

//	printf("getInputEventPath: FileBase = %s \n",FileBase );

    glob_t vGlobbuf[3];

    result =  glob(PathDevice , GLOB_DOOFFS, NULL, &vGlobbuf[0]);

    if(  0 != result )
    {
    	// error
    	printf("getInputEventPath: Error %s error=%d \n",PathDevice ,result);
    }
    else
    {
    	for( __size_t ii = 0; ii < vGlobbuf[0].gl_pathc; ii++ )
    	{
    		realpath(vGlobbuf[0].gl_pathv[ii],PathDeviceReal);
    		//printf("%s->%s \n",vGlobbuf[0].gl_pathv[ii], vBuffer);

#ifdef CONFIG_PC
    		sprintf(PathInput,"%s/input/input*",PathDeviceReal);
#else
    		sprintf(PathInput,"%s/../input/input*",PathDeviceReal);
#endif

    		realpath(PathInput,PathInputReal);

    	    result =  glob(PathInputReal, GLOB_DOOFFS, NULL, &vGlobbuf[1]);
//    	    printf("%s \n",vGlobbuf[1].gl_pathv[0]);

    	    if( 0 == result )
    	    {
        		sprintf(PathEvent,"%s/event*",vGlobbuf[1].gl_pathv[0]);
        	    result =  glob(PathEvent, GLOB_DOOFFS, NULL, &vGlobbuf[2]);
        	    strcpy(EventFilename,vGlobbuf[2].gl_pathv[0]);

        	    printf("\t%s \n",EventFilename);
}
    	    else
    	    {
    	    	printf("Error No diretory %s \n",PathInput);
    	    }

    	    globfree(&vGlobbuf[2]);
    	    globfree(&vGlobbuf[1]);
    	}
    }
    globfree(&vGlobbuf[0]);


	return result;
}

int main( int argc, char *argv[] )
{
	int result = 0;

	getInputEventPath( );

//	result = ExecCommand(argc,argv);


	//main_readlink(argc, argv);

	//libusb_printDevice();

	//main_hotplugtest(argc, argv);

//	main_udev();

//	main_ioctl(argc, argv);

//	main_hidraw(argc, argv);

	return result;

}
