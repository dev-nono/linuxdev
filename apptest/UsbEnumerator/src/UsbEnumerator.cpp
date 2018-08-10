/*
 * UsbEnumerator.cpp
 *
 *  Created on: 2 août 2018
 *      Author: bruno
 */


#include <libusb-1.0/libusb.h>
#include <iterator>
#include <string.h>
#include <time.h>
#include <algorithm>


#include "UsbEnumerator.h"




//*************************************
//*
//*************************************
UsbEnumerator::UsbEnumerator():
		m_PthreadID(0),
		m_ExitThread(0),
		m_LIBUSB_HOTPLUG_EVENT_DEVICE_ARRIVED(0),
		m_LIBUSB_HOTPLUG_EVENT_DEVICE_LEFT(0)
{
	// TODO Auto-generated constructor stub



}
//*************************************
//*
//*************************************
UsbEnumerator::~UsbEnumerator()
{
	// TODO Auto-generated destructor stub
	setExitThread( 1 != 0 );

}
//*************************************
//*
//*************************************
int UsbEnumerator::Initilize()
{
	int result = 0;

	// section create thread
	result = pthread_create(&m_PthreadID,NULL,fnTreadLoopEvent,this);

	return result;
}
//*************************************
//*
//*************************************
int UsbEnumerator::ListCreate()
{
	printf("UsbEnumerator::ListCreate_IN \n");
	int 				result 		= 0;
	int 				NbDevices	= 0;
	libusb_device **	ppDevices 	= 0;
	int 				ii			= 0;
	int 				errorCode = 0;
	UsbDevice 			lDevice;

	NbDevices = libusb_get_device_list(NULL, &ppDevices);

	if (NbDevices < 0)
	{
		result = NbDevices;
	}
	else
	{
		for (ii = 0; ii < NbDevices; ++ii)
		{
			lDevice.Clear();
			errorCode = getDeviceInfo(ppDevices[ii],lDevice);


			if( 0 == errorCode  )
			{
				errorCode 	= ListAddDevice(lDevice);
			}
		}
	}

	libusb_free_device_list(ppDevices, 1);

	ppDevices = 0;

	printf("UsbEnumerator::ListCreate_OUT result = %d size=%d \n",
			result , (int)m_ListDevice.size());

	return result;
}
//*************************************
//* return : 0 if filtered device
//*          -1 no filtered
//*************************************
int UsbEnumerator::CheckDevice(UsbDevice& a_Device)
{
	int result 		= -1;
	int retCode 	= 0;
	t_ListFilterDevice::iterator IT;

	printf("\t\t CheckDevice_IN \n");

	for(	IT = m_ListFilter.begin();
			IT != m_ListFilter.end();
			IT++)
	{
		UsbDevice & refFilter = (*IT);

		retCode = refFilter.MatchFilter(a_Device);

		if( 0 == retCode )
		{
			//  one filter ok  found return ok
			result = 0;
			break;
		}
		else
		{
			result = -1;
		}

	}// for(...)

	printf("\t\t CheckDevice_OUT result = %d\n",result);

	return result;
}
//*************************************
//*
//*************************************
int UsbEnumerator::getDeviceInfo(	libusb_device* 	a_pDevice,
									UsbDevice& 		a_Device)
{
	int 								result = 0;
	struct libusb_device_descriptor 	lDesc;
	struct libusb_config_descriptor * 	pConfig = 0;
	int vBusNumber						= 0;
	int vDeviceNumber					= 0;
	int vDeviceDescriptor_vendorID		= 0;
	int vDeviceDescriptor_productID		= 0;
	int vDeviceDescriptor_bDeviceClass	= 0;
	int vInterfaceDescriptor_iInterface	= 0;
	int vInterfaceDescriptor_bInterfaceClass = 0;



	if( a_pDevice == 0 )
	{
		result = -1;
		printf("UsbEnumerator::getDeviceInfo Error a_pDevice == 0 , result= %d \n",	result );
	}
	else
	{
		 vBusNumber 	= (int)libusb_get_bus_number (a_pDevice);
		 vDeviceNumber 	= (int)libusb_get_device_address(a_pDevice);

		//*******************
		// get vendrID and productID
		//*******************

		memset(&lDesc,0,sizeof(lDesc));
		result  = libusb_get_device_descriptor(a_pDevice, &lDesc);

		if (LIBUSB_SUCCESS == result)
		{
			vDeviceDescriptor_productID 	= lDesc.idProduct;
			vDeviceDescriptor_vendorID		= lDesc.idVendor;
			vDeviceDescriptor_bDeviceClass	= lDesc.bDeviceClass;
		}
		else
		{
			printf("UsbEnumerator::getDeviceInfo Error libusb_geUsbDevice_descriptor(), result= %d %s \n",
					result , libusb_error_name(result));
		}
	}

	//**********************
	// get Configuration Descriptor iInterface
	//**********************Info
	if( 0 == result )
	{
		result =  libusb_get_active_config_descriptor (a_pDevice,&pConfig);

		if (LIBUSB_SUCCESS == result)
		{
			vInterfaceDescriptor_iInterface 		= pConfig->interface[0].altsetting->iInterface;
			vInterfaceDescriptor_bInterfaceClass 	= pConfig->interface[0].altsetting->bInterfaceClass;
		}
		else
		{
			printf("UsbEnumerator::getDeviceInfo Error libusb_get_active_config_descriptor() , result= %d %s \n",
					result ,libusb_error_name(result));
		}
	}

	a_Device.setData(
						vBusNumber,
						vDeviceNumber,
						vDeviceDescriptor_vendorID,
						vDeviceDescriptor_productID,
						vDeviceDescriptor_bDeviceClass,
						vInterfaceDescriptor_iInterface,
						vInterfaceDescriptor_bInterfaceClass);

	if( pConfig )
	{
		libusb_free_config_descriptor(pConfig);
	}

	return result;
}

//*************************************
//*	return : 0   if add
//*			 -1  if not adding
//*************************************
int UsbEnumerator::ListAddDevice(UsbDevice& a_Device)
{
	printf("UsbEnumerator::ListAddDevice_IN  size=%d \n",(int)m_ListDevice.size());

	int result = 0;

	result = CheckDevice(a_Device);

	if( 0 == result )
	{
		a_Device.PrintDevice("UsbEnumerator::ListAddDevice ");
		m_ListDevice.push_back(a_Device);
	}

	printf("UsbEnumerator::ListAddDevice_OUT  result=%d size=%d \n",
		result, (int)m_ListDevice.size());

	return result;
}
//*************************************
//*	return : 0 ok removed
//*   		-1  nothing to remove
//*************************************
int UsbEnumerator::ListRemoveDevice(UsbDevice& a_Device)
{
	printf("ListRemoveDevice_IN size=%d \n",(int)m_ListDevice.size());

	int 			result 			= 0;
	t_ListDevice::iterator	IT;

	IT = std::find(	m_ListDevice.begin(),
						m_ListDevice.end(),
						a_Device);

	if( IT == m_ListDevice.end() )
	{
		result = -1;
		printf("ListRemoveDevice  NOTHING to remove \n");
	}
	else
	{
		m_ListDevice.remove(a_Device);

		printf("ListRemoveDevice  ITEM REMOVED \n");
	}

	printf("ListRemoveDevice_OUT  result = %d size=%d \n",result, (int)m_ListDevice.size());

	return result;
}

//******************************
//******************************
int UsbEnumerator::fnHotplugCallbackEventAddDevice (
		libusb_context 			*a_pCtx,
		libusb_device 			*a_pDevice,
		libusb_hotplug_event 	a_Event,
		void *					a_pUser_data)
{
	int 			result 	 __attribute__((unused));
	UsbEnumerator*	pThis 	= (UsbEnumerator*)a_pUser_data;
	UsbDevice 		lDevice;

	printf("fnHotplugCallbackEventAddDevice_IN \n");

	result = pThis->getDeviceInfo(a_pDevice,lDevice);
	lDevice.PrintDevice("fnHotplugCallbackEventAddDevice ADD ");

	result = pThis->ListAddDevice(lDevice);

	if( 0 == result )
	{
		result = pThis->UsbEnumerator::OnEventAddDevice(lDevice);
	}

	printf("fnHotplugCallbackEventAddDevice_OUT \n");

	return 0; // return 0 for getting callback always registerd
}
//******************************
//
//******************************
int UsbEnumerator::OnEventAddDevice(UsbDevice& a_Device)
{
	printf("UsbEnumerator::OnEventAddDevice \n");

	// call virtual heritage
	return OnEventAddDevice(a_Device);
};

//******************************
//
//******************************
int UsbEnumerator::fnHotplugCallbackEventRemoveDevice (
		libusb_context 			*a_pCtx,
		libusb_device 			*a_pDevice,
		libusb_hotplug_event 	a_Event,
		void *					a_pUser_data)
{
	int 			result __attribute__((unused));
	UsbEnumerator*	pThis 	= (UsbEnumerator*)a_pUser_data;
	UsbDevice 	lDevice;

	printf("UsbEnumerator::fnHotplugCallbackEventRemoveDevice_IN \n");

	result = pThis->getDeviceInfo(a_pDevice,lDevice);

	if( 0 == result )
	{
		pThis->UsbEnumerator::OnEventRemoveDevice(lDevice);
	}

	printf("UsbEnumerator::fnHotplugCallbackEventRemoveDevice_OUT result = %d\n",result);

	return 0; // return 0 for getting callback always registerd
}
//******************************
//
//******************************
int UsbEnumerator::OnEventRemoveDevice(UsbDevice& a_Device)
{
	int result  = 0;

	printf("UsbEnumerator::OnEventRemoveDevice_IN\n");

	a_Device.PrintFilterDevice("OnEventRemoveDevice ");

	result = ListRemoveDevice(a_Device);

	printf("UsbEnumerator::OnEventRemoveDevice_OUT result=%d \n",result);

	// call virtual heritage
	return OnEventRemoveDevice(a_Device);
};

//*************************************
//*
//*************************************
int UsbEnumerator::FilterDeviceAdd(
		int	a_DeviceDescriptor_vendorID,
		int	a_DeviceDescriptor_productID,
		int a_DeviceDescriptor_bDeviceClass,
		int a_InterfaceDescriptor_iInterface,
		int a_InterfaceDescriptor_bInterfaceClass)
{
	int result = 0;

	// mutex lock

	m_ListFilter.push_back(
			UsbDevice(
					-1,// not used for filter
					-1,// not used for filter
					a_DeviceDescriptor_vendorID,
					a_DeviceDescriptor_productID,
					a_DeviceDescriptor_bDeviceClass,
					a_InterfaceDescriptor_iInterface,
					a_InterfaceDescriptor_bInterfaceClass)
	);

	//	mutex unlock
	return result;
}

void* UsbEnumerator::fnTreadLoopEvent(void * a_pThis)
{
	int 			result 		= 0;
	int 			retCode 		= 0;
	void* 			pRetCode 	= 0;
	UsbEnumerator* 	pThis 		= (UsbEnumerator*)a_pThis;

	//******************************************
	// initilize libusb library
	//******************************************
	result = libusb_init(NULL);

	if( LIBUSB_SUCCESS != result )
	{
		printf("UsbEnumerator::fnTreadLoopEvent_1 Error libusb_init() = %d %s\n",
				result,libusb_error_name(result) );
	}

	//******************************************
	// check hotplug capacity
	//******************************************
	if( LIBUSB_SUCCESS == result )
	{
		retCode = libusb_has_capability (LIBUSB_CAP_HAS_HOTPLUG);

		if( 0 == retCode )
		{
			printf("UsbEnumerator::fnTreadLoopEvent_2 Error libusb_has_capability (LIBUSB_CAP_HAS_HOTPLUG) = %d %s \n",
					result,libusb_error_name(result) );
			result = LIBUSB_ERROR_NOT_SUPPORTED;
		}
	}

	//******************************************
	// create list of installed devices
	//******************************************
	if (LIBUSB_SUCCESS ==  result )
	{
		result = pThis->ListCreate();
		if (LIBUSB_SUCCESS !=  result )
		{
			printf("UsbEnumerator::fnTreadLoopEvent_4 Error ListCreate result = %d \n",result);
		}
	}

	//******************************************
	// install callback for add or remove devices
	//******************************************
	if( LIBUSB_SUCCESS == result )
	{
		result = pThis->AddHotplugCallback();
		if (LIBUSB_SUCCESS !=  result )
		{
			printf("UsbEnumerator::fnTreadLoopEvent_3 Error AddHotplugCallback result = %d \n",result);
		}
	}

	//******************************************
	// loop on event
	//******************************************
	if (LIBUSB_SUCCESS ==  result )
	{
		do
		{
//			printf("UsbEnumerator::fnTreadLoopEvent_5 start DO while {} \n");

			result = libusb_handle_events (NULL);

			if (LIBUSB_SUCCESS !=  result )
			{
				printf("UsbEnumerator::fnTreadLoopEvent_6 Error libusb_handle_events_completed result = %d \n",result);
			}
			else
			{
//				printf("UsbEnumerator::fnTreadLoopEvent_7 result = %d \n",result);
			}
		}while( pThis->isExitThread() == 0 );
	}

	printf("UsbEnumerator::fnTreadLoopEvent_8 EXIT result = %d \n",result);

	libusb_exit(NULL);

	pRetCode = (void*)result;

	return pRetCode;
}
//******************************************
//
//******************************************
int  UsbEnumerator::AddHotplugCallback()
{
	int result = 0;

	result = libusb_hotplug_register_callback (
			NULL,// libusb_context * 	ctx,
			LIBUSB_HOTPLUG_EVENT_DEVICE_ARRIVED,	// LIBUSB_HOTPLUG_EVENUsbDevice_LEFT
			LIBUSB_HOTPLUG_NO_FLAGS,				// LIBUSB_HOTPLUG_NO_FLAGS LIBUSB_HOTPLUG_ENUMERATE
			LIBUSB_HOTPLUG_MATCH_ANY,				// vendor_id,
			LIBUSB_HOTPLUG_MATCH_ANY, 				// product_id,
			LIBUSB_HOTPLUG_MATCH_ANY,
			fnHotplugCallbackEventAddDevice,
			(void*)this,
			&m_LIBUSB_HOTPLUG_EVENT_DEVICE_ARRIVED);

	if( LIBUSB_SUCCESS != result )
	{
		printf("AddHotplugCallback LIBUSB_HOTPLUG_EVENUsbDevice_ARRIVED result=%d",result);
	}

	if( 0 == result )
	{
		result = libusb_hotplug_register_callback (
				NULL,									// libusb_context * 	ctx,
				LIBUSB_HOTPLUG_EVENT_DEVICE_LEFT, 		// LIBUSB_HOTPLUG_EVENUsbDevice_ARRIVED,// LIBUSB_HOTPLUG_EVENUsbDevice_LEFT
				LIBUSB_HOTPLUG_NO_FLAGS,				// LIBUSB_HOTPLUG_NO_FLAGS LIBUSB_HOTPLUG_ENUMERATE
				LIBUSB_HOTPLUG_MATCH_ANY,				// vendor_id,
				LIBUSB_HOTPLUG_MATCH_ANY,				// product_id,
				LIBUSB_HOTPLUG_MATCH_ANY,
				fnHotplugCallbackEventRemoveDevice,
				(void*)this,
				&m_LIBUSB_HOTPLUG_EVENT_DEVICE_LEFT);

		if( LIBUSB_SUCCESS != result )
		{
			printf("AddHotplugCallback m_LIBUSB_HOTPLUG_EVENUsbDevice_LEFT result=%d",result);
		}
	}

	return result;
}
//******************************************
//
//******************************************
int UsbEnumerator::StopAndJoin()
{
	int 	result 	= 0;
	void* 	pStatus = 0;

	setExitThread(1);

	pthread_cancel(m_PthreadID);

	result = pthread_join(m_PthreadID,&pStatus);

	m_PthreadID = 0;

	return result;
}

