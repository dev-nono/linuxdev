/*
 * UsbEnumerator.h
 *
 *  Created on: 2 août 2018
 *      Author: bruno
 */

#ifndef SRC_USBENUMERATOR_H_
#define SRC_USBENUMERATOR_H_


#include <libusb-1.0/libusb.h>
#include <list>
#include <string>
#include <pthread.h>

#include "UsbDevice.h"



class UsbEnumerator
{
public:
	UsbEnumerator();
	virtual ~UsbEnumerator();

	int Initilize();

	int FilterDeviceAdd(
//			int a_BusNumber,
//			int a_DeviceNumber,
			int	a_DeviceDescriptor_vendorID  	 		= LIBUSB_HOTPLUG_MATCH_ANY,
			int	a_DeviceDescriptor_productID 			= LIBUSB_HOTPLUG_MATCH_ANY,
			int a_DeviceDescriptor_bDeviceClass    		= LIBUSB_HOTPLUG_MATCH_ANY,
			int a_InterfaceDescriptor_iInterface   		= LIBUSB_HOTPLUG_MATCH_ANY,
			int a_InterfaceDescriptor_bInterfaceClass 	= LIBUSB_HOTPLUG_MATCH_ANY);


	int StopAndJoin();


private:
	virtual int OnEventAddDevice(UsbDevice& a_Device);
	virtual int OnEventRemoveDevice(UsbDevice& a_Device);

	int isExitThread() {
		return m_ExitThread; }

	void setExitThread(int a_ExitThread) {
		m_ExitThread = a_ExitThread;	}

	int ListCreate();
	int ListAddDevice	(UsbDevice& a_Device);
	int ListRemoveDevice(UsbDevice& a_Device);
	int CheckDevice		(UsbDevice& a_Device);
	int getDeviceInfo	(libusb_device* a_pDevice,UsbDevice& a_Device);

	static int fnHotplugCallbackEventAddDevice (
			libusb_context 			*a_pCtx,
			libusb_device 			*a_pDevice,
			libusb_hotplug_event 	a_Event,
			void *					a_pUser_data);

	static int fnHotplugCallbackEventRemoveDevice (
			libusb_context 			*a_pCtx,
			libusb_device 			*a_pDevice,
			libusb_hotplug_event 	a_Event,
			void *					a_pUser_data);

	static void* fnTreadLoopEvent(void * a_pThis);

	int  AddHotplugCallback();


	t_ListFilterDevice 	m_ListFilter;
	t_ListDevice 		m_ListDevice;
	pthread_t 			m_PthreadID;
	int					m_ExitThread;

	libusb_hotplug_callback_handle m_LIBUSB_HOTPLUG_EVENT_DEVICE_ARRIVED;
	libusb_hotplug_callback_handle m_LIBUSB_HOTPLUG_EVENT_DEVICE_LEFT;


};

#endif /* SRC_USBENUMERATOR_H_ */
