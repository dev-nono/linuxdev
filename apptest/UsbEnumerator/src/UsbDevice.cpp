/*
 * UsbDevice.cpp
 *
 *  Created on: 7 août 2018
 *      Author: bruno
 */

#include "UsbDevice.h"


//**********************************************
//*
//**********************************************
UsbDevice::~UsbDevice()
{
	// TODO Auto-generated destructor stub
}
//**********************************************
//*
//**********************************************
bool UsbDevice:: operator == (const UsbDevice &a_Item)
{
	bool result = false;

	if( 	(a_Item.m_DeviceDescriptor_bDeviceClass 		== m_DeviceDescriptor_bDeviceClass)
		&& 	(a_Item.m_DeviceDescriptor_productID 			== m_DeviceDescriptor_productID )
		&&	(a_Item.m_DeviceDescriptor_vendorID				== m_DeviceDescriptor_vendorID)
		&&	(a_Item.m_BusNumber								== m_BusNumber)
		&&	(a_Item.m_DeviceNumber							== m_DeviceNumber)
	)
		result = true;
	else
		result = false;

	return result;
}
//**********************************************
//*
//**********************************************
int UsbDevice::setData(
			int a_BusNumber,
			int a_DeviceNumber,
			int a_DeviceDescriptor_vendorID,
			int a_DeviceDescriptor_productID,
			int a_DeviceDescriptor_bDeviceClass,
			int a_InterfaceDescriptor_iInterface,
			int a_InterfaceDescriptor_bInterfaceClass)
{
	m_BusNumber								= a_BusNumber;
	m_DeviceNumber							= a_DeviceNumber;
	m_DeviceDescriptor_vendorID			 	= a_DeviceDescriptor_vendorID;
	m_DeviceDescriptor_productID			= a_DeviceDescriptor_productID;
	m_DeviceDescriptor_bDeviceClass		 	= a_DeviceDescriptor_bDeviceClass;
	m_InterfaceDescriptor_iInterface		= a_InterfaceDescriptor_iInterface;
	m_InterfaceDescriptor_bInterfaceClass	= a_InterfaceDescriptor_bInterfaceClass;

		return 0;
}
int UsbDevice::Clear()
{
	m_BusNumber								= LIBUSB_HOTPLUG_MATCH_ANY;
	m_DeviceNumber							= LIBUSB_HOTPLUG_MATCH_ANY;
	m_DeviceDescriptor_vendorID			 	= LIBUSB_HOTPLUG_MATCH_ANY;
	m_DeviceDescriptor_productID			= LIBUSB_HOTPLUG_MATCH_ANY;
	m_DeviceDescriptor_bDeviceClass		 	= LIBUSB_HOTPLUG_MATCH_ANY;
	m_InterfaceDescriptor_iInterface		= LIBUSB_HOTPLUG_MATCH_ANY;
	m_InterfaceDescriptor_bInterfaceClass	= LIBUSB_HOTPLUG_MATCH_ANY;

	return 0;
}
//************************************************
//	return 0  if match
//		   -1  not match
//************************************************
int UsbDevice::MatchFilter(UsbDevice& a_UsbDevice)
{
	printf("\t\t UsbDevice::MatchFilter_IN \n");

	a_UsbDevice.PrintDevice("MatchFilter ");

	int result = 0;

	if( m_DeviceDescriptor_vendorID  > 0 )
	{
		if( a_UsbDevice.m_DeviceDescriptor_vendorID == m_DeviceDescriptor_vendorID)
		{
			result = 0;
		}
		else
			result = -1;

	}
	if(		( 0 == result  )
		&&	( m_DeviceDescriptor_productID  > 0 ))
	{
		if( a_UsbDevice.m_DeviceDescriptor_productID == m_DeviceDescriptor_productID)
			result = 0;
		else
			result = -1;
	}

	if(		( 0 == result  )
		&&	(m_DeviceDescriptor_bDeviceClass  > 0 ) )
	{
		if( a_UsbDevice.m_DeviceDescriptor_bDeviceClass == m_DeviceDescriptor_bDeviceClass)
			result = 0;
		else
			result = -1;
	}

	if(		( 0 == result  )
		&&	( m_InterfaceDescriptor_iInterface  > 0 ))
	{
		if( a_UsbDevice.m_InterfaceDescriptor_iInterface == m_InterfaceDescriptor_iInterface )
			result = 0;
		else
			result = -1;
	}

	if(		( 0 == result  )
		&&	( m_InterfaceDescriptor_bInterfaceClass > 0 ))
	{
		if( a_UsbDevice.m_InterfaceDescriptor_bInterfaceClass == m_InterfaceDescriptor_bInterfaceClass )
			result = 0;
		else
			result = -1;
	}

	printf("\t\t UsbDevice::MatchFilter_OUT resut = %d \n" , result);

	return result;
}
//**********************************************
//*
//**********************************************

void UsbDevice::PrintDevice( std::string a_Title	)
{
	printf("\t _%s %.4X %.4X %.4X %.4X %.4X \n",
			a_Title.c_str(),
			m_BusNumber,
			m_DeviceNumber,
			m_DeviceDescriptor_vendorID,
			m_DeviceDescriptor_productID,
			m_DeviceDescriptor_bDeviceClass);

}
//**********************************************
//*
//**********************************************
void UsbDevice::PrintFilterDevice(	std::string 		a_Title)
{
	printf("\t _ %s %.4X %.4X %.4X %.4X %.4X \n",
			a_Title.c_str(),
			m_DeviceDescriptor_vendorID,
			m_DeviceDescriptor_productID,
			m_DeviceDescriptor_bDeviceClass,
			m_InterfaceDescriptor_iInterface,
			m_InterfaceDescriptor_bInterfaceClass);

}
