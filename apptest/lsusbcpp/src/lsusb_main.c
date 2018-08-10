/*
 ============================================================================
 Name        : lsusb.c
 Author      : 
 Version     :
 Copyright   : Your copyright notice
 Description : Hello World in C, Ansi-style
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <stdio.h>



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

int main( int argc, char *argv[] )
{
	int result = 0;

	result = ExecCommand(argc,argv);


	//main_readlink(argc, argv);

	//libusb_printDevice();

	//main_hotplugtest(argc, argv);

//	main_udev();

//	main_ioctl(argc, argv);

//	main_hidraw(argc, argv);

	return result;

}
