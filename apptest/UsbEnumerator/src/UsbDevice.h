/*
 * UsbDevice.h
 *
 *  Created on: 7 août 2018
 *      Author: bruno
 */

#ifndef SRC_USBDEVICE_H_
#define SRC_USBDEVICE_H_

#include <libusb-1.0/libusb.h>
#include <list>
#include <string>

class UsbDevice
{
public:
	UsbDevice() :
				m_BusNumber								(LIBUSB_HOTPLUG_MATCH_ANY),
				m_DeviceNumber							(LIBUSB_HOTPLUG_MATCH_ANY),
				m_DeviceDescriptor_vendorID				(LIBUSB_HOTPLUG_MATCH_ANY),
				m_DeviceDescriptor_productID			(LIBUSB_HOTPLUG_MATCH_ANY),
				m_DeviceDescriptor_bDeviceClass			(LIBUSB_HOTPLUG_MATCH_ANY),
				m_InterfaceDescriptor_iInterface		(LIBUSB_HOTPLUG_MATCH_ANY),
				m_InterfaceDescriptor_bInterfaceClass	(LIBUSB_HOTPLUG_MATCH_ANY)
				{}

	UsbDevice(
				int a_BusNumber,
				int a_DeviceNumber,
				int a_DeviceDescriptor_vendorID,
				int a_DeviceDescriptor_productID,
				int a_DeviceDescriptor_bDeviceClass,
				int a_InterfaceDescriptor_iInterface,
				int a_InterfaceDescriptor_bInterfaceClass)	:
					m_BusNumber								(a_BusNumber),
					m_DeviceNumber							(a_DeviceNumber),

					m_DeviceDescriptor_vendorID			(a_DeviceDescriptor_vendorID),
					m_DeviceDescriptor_productID			(a_DeviceDescriptor_productID),
					m_DeviceDescriptor_bDeviceClass		(a_DeviceDescriptor_bDeviceClass),
					m_InterfaceDescriptor_iInterface		(a_InterfaceDescriptor_iInterface),
					m_InterfaceDescriptor_bInterfaceClass	(a_InterfaceDescriptor_bInterfaceClass)
	     {

	     }

	int setData(
				int a_BusNumber,
				int a_DeviceNumber,
				int a_DeviceDescriptor_vendorID,
				int a_DeviceDescriptor_productID,
				int a_DeviceDescriptor_bDeviceClass,
				int a_InterfaceDescriptor_iInterface,
				int a_InterfaceDescriptor_bInterfaceClass);

	bool operator == (const UsbDevice &a_Item);

	virtual ~UsbDevice();

	int MatchFilter(UsbDevice& a_UsbDevice);
	int Clear();
	void PrintDevice(		std::string a_Title);
	void PrintFilterDevice(	std::string a_Title);

	int getBusNumber() const {
		return m_BusNumber;
	}

	int getDeviceDescriptorBDeviceClass() const {
		return m_DeviceDescriptor_bDeviceClass;
	}

	int getDeviceDescriptorProductId() const {
		return m_DeviceDescriptor_productID;
	}

	int getDeviceDescriptorVendorId() const {
		return m_DeviceDescriptor_vendorID;
	}

	int getDeviceNumber() const {
		return m_DeviceNumber;
	}

	int getInterfaceDescriptorBInterfaceClass() const {
		return m_InterfaceDescriptor_bInterfaceClass;
	}

	int getInterfaceDescriptorIInterface() const {
		return m_InterfaceDescriptor_iInterface;
	}

private:
		int m_BusNumber;
		int m_DeviceNumber;

		int m_DeviceDescriptor_vendorID;
		int m_DeviceDescriptor_productID;
		int m_DeviceDescriptor_bDeviceClass;

		int m_InterfaceDescriptor_iInterface;
		int m_InterfaceDescriptor_bInterfaceClass;

};
typedef std::list< UsbDevice > 	t_ListDevice;
typedef std::list< UsbDevice > 	t_ListFilterDevice;


#endif /* SRC_USBDEVICE_H_ */
