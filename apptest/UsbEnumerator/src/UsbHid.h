/*
 * UsbHid.h
 *
 *  Created on: 8 août 2018
 *      Author: bruno
 */

#ifndef SRC_USBHID_H_
#define SRC_USBHID_H_

#include "UsbEnumerator.h"

class UsbHid: public UsbEnumerator {
public:
	UsbHid();
	virtual ~UsbHid();

	int getInputEventPath(std::string& a_InputEventPath );

	virtual int OnEventAddDevice(UsbDevice& a_Device);
	virtual int OnEventRemoveDevice(UsbDevice& a_Device);

private:
		std::string m_InputEventPath;

};

#endif /* SRC_USBHID_H_ */
