/*
 * UsbHid.cpp
 *
 *  Created on: 8 août 2018
 *      Author: bruno
 */

#include <sys/types.h>
#include <dirent.h>
#include "errno.h"
#include "string.h"

#include "UsbHid.h"



UsbHid::UsbHid()
{
	// TODO Auto-generated constructor stub

	int	vDeviceDescriptor_vendorID				= LIBUSB_HOTPLUG_MATCH_ANY;
	int	vDeviceDescriptor_productID				= LIBUSB_HOTPLUG_MATCH_ANY;
	int vDeviceDescriptor_bDeviceClass			= LIBUSB_HOTPLUG_MATCH_ANY;
	int vInterfaceDescriptor_iInterface			= LIBUSB_HOTPLUG_MATCH_ANY;
	int vInterfaceDescriptor_bInterfaceClass	= LIBUSB_HOTPLUG_MATCH_ANY;

	vInterfaceDescriptor_bInterfaceClass	= LIBUSB_CLASS_HID;
	UsbEnumerator::FilterDeviceAdd(
			vDeviceDescriptor_vendorID,
			vDeviceDescriptor_productID,
			vDeviceDescriptor_bDeviceClass,
			vInterfaceDescriptor_iInterface,
			vInterfaceDescriptor_bInterfaceClass);

	UsbEnumerator::Initilize();

}

UsbHid::~UsbHid() {
	// TODO Auto-generated destructor stub
}

int UsbHid::OnEventAddDevice(UsbDevice& a_Device)
{
	printf("UsbHid::OnEventAddDevice_IN \n");

	int result = 0;

	printf("UsbHid::OnEventAddDevice_OUT result=%d \n",result);

	return result;
}
int UsbHid::OnEventRemoveDevice(UsbDevice& a_Device)
{
	printf("UsbHid::OnEventRemoveDevice_IN \n");

	int result = 0;

	a_Device.PrintDevice("UsbHid::OnEventRemoveDevice printdevice : ");

	printf("UsbHid::OnEventRemoveDevice_OUT result=%d \n",result);

	return result;
}


int UsbHid::getInputEventPath(std::string& a_InputEventPath )
{
	int 		result 		= 0;
//	char 		FileBase[PATH_MAX];
//	char 		FileRootDir[PATH_MAX] = "/sys/bus/hid/device";
//	UsbDevice& 	vUsbDevice 	= m_ListDevice.front() ;
//	DIR*		pRootDir 	= 0;;
//
//	// /sys/bus/hid/device/003:cID:VID:pID:nID
//	// /sys/bus/hid/device/0003:05E0:1200:0001
//	sprintf(FileBase,"%s/%.4X:%.4X:%.4X",
//			FileRootDir,
//			vUsbDevice.getInterfaceDescriptorBInterfaceClass(),
//			vUsbDevice.getDeviceDescriptorVendorId(),
//			vUsbDevice.getDeviceNumber());
//
//		// open dir
//	errno = 0;
//	pRootDir = opendir(FileBase);
//
//	if( 0 == pRootDir )
//	{
//		printf("Error %d - %s \¬",errno, strerror(errno));
//	}
//
//	//loop on all file
//	if( 0 != pRootDir )
//	{
//
//
//	}
//
//
//

	return result;
}
