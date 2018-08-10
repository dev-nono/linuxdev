/*
 * TestUnitaire.cpp
 *
 *  Created on: 3 août 2018
 *      Author: bruno
 */

#include <unistd.h>
#include "UsbHid.h"


class TU_UsbEnumerator: public UsbEnumerator
{
public:
	TU_UsbEnumerator(){};
	virtual ~TU_UsbEnumerator(){};

	int OnEventAddDevice(UsbDevice& a_Device)
	{
		printf("TU_UsbEnumerator::OnEventAddDevice \n");
		return 0;
	}
	 int OnEventRemoveDevice(UsbDevice& a_Device)
	{
		printf("TU_UsbEnumerator::OnEventRemoveDevice \n");
		return 0;
	}


};

int main_tu_Create(int argc, char *argv[])
{
	int	a_DeviceDescriptor_vendorID				= LIBUSB_HOTPLUG_MATCH_ANY;
	int	a_DeviceDescriptor_productID			= LIBUSB_HOTPLUG_MATCH_ANY;
	int a_DeviceDescriptor_bDeviceClass			= LIBUSB_HOTPLUG_MATCH_ANY;
	int a_InterfaceDescriptor_iInterface		= LIBUSB_HOTPLUG_MATCH_ANY;
	int a_InterfaceDescriptor_bInterfaceClass	= LIBUSB_HOTPLUG_MATCH_ANY;

	std::string vInputEventPath;


	UsbHid vUsbHid;

//	vUsbHid.FilterDeviceAdd(0x05F9,0x220C,0,5);
//	vUsbHid.FilterDeviceAdd(0x05F9,0x220C,0,444); // error
//	vUsbHid.FilterDeviceAdd(0x0413C,0x2107,0,5);

	a_InterfaceDescriptor_bInterfaceClass	= LIBUSB_CLASS_HID;
	a_DeviceDescriptor_vendorID 	= 0x05F9;
	a_DeviceDescriptor_productID	= 0x220C;

	vUsbHid.FilterDeviceAdd(
			a_DeviceDescriptor_vendorID,
			a_DeviceDescriptor_productID,
			a_DeviceDescriptor_bDeviceClass,
			a_InterfaceDescriptor_iInterface,
			a_InterfaceDescriptor_bInterfaceClass);


	vUsbHid.Initilize();


	while(1)
	{
		vUsbHid.getInputEventPath(vInputEventPath);
		sleep(1);
	}

	return 0;
}

