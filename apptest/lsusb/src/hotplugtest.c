/*
* hotplugtest.c
*
*  Created on: 1 août 2018
*      Author: bruno
*/


/* -*- Mode: C; indent-tabs-mode:t ; c-basic-offset:8 -*- */
/*
* libusb example program for hotplug API
* Copyright © 2012-2013 Nathan Hjelm <hjelmn@mac.com>
*
* This library is free software; you can redistribute it and/or
* modify it under the terms of the GNU Lesser General Public
* License as published by the Free Software Foundation; either
* version 2.1 of the License, or (at your option) any later version.
*
* This library is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
* Lesser General Public License for more details.
*
* You should have received a copy of the GNU Lesser General Public
* License along with this library; if not, write to the Free Software
* Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
*/

#include <stdlib.h>
#include <stdio.h>

//#include "libusb.h"
#include <libusb-1.0/libusb.h>

int done = 0;
libusb_device_handle *handle = NULL;

static void printDetail(struct libusb_device_descriptor* a_pDesc)
{
	printf (" desc.idVendor  = 0x%hX \n",a_pDesc->idVendor);
	printf (" a_pDesc->idProduct = 0x%hX \n",a_pDesc->idProduct);

	printf ("a_pDesc->bLength         = 0x%X \n",a_pDesc->bLength);
	printf ("a_pDesc->bDescriptorType = 0x%X \n",a_pDesc->bDescriptorType) ;
	printf ("a_pDesc->bcdUSB		  = 0x%X \n",a_pDesc->bcdUSB);
	printf ("a_pDesc->bDeviceClass    = 0x%X \n",a_pDesc->bDeviceClass); ;
	printf ("a_pDesc->bDeviceSubClass = 0x%X \n",a_pDesc->bDeviceSubClass);
	printf ("a_pDesc->bDeviceProtocol = 0x%X \n",a_pDesc->bDeviceProtocol);
	printf ("a_pDesc->bMaxPacketSize0 = 0x%X \n",a_pDesc->bMaxPacketSize0);
	printf ("a_pDesc->idVendor        = 0x%X \n",a_pDesc->idVendor);
	printf ("a_pDesc->idProduct       = 0x%X \n",a_pDesc->idProduct);
	printf ("a_pDesc->bcdDevice       = 0x%X \n",a_pDesc->bcdDevice);
	printf ("a_pDesc->iManufacturer   = 0x%X \n",a_pDesc->iManufacturer);
	printf ("a_pDesc->iProduct        = 0x%X \n",a_pDesc->iProduct);
	printf ("a_pDesc->iSerialNumber   = 0x%X \n",a_pDesc->iSerialNumber);
	printf ("a_pDesc->bNumConfigurations = 0x%X \n",a_pDesc->bNumConfigurations);

}

static int LIBUSB_CALL hotplug_callback(libusb_context *ctx, libusb_device *dev, libusb_hotplug_event event, void *user_data)
{
	struct libusb_device_descriptor desc;
	int rc;

	(void)ctx;
	(void)dev;
	(void)event;
	(void)user_data;

	rc = libusb_get_device_descriptor(dev, &desc);
	if (LIBUSB_SUCCESS != rc) {
		fprintf (stderr, "Error getting device descriptor\n");
	}

	printf ("Device attached: %04x:%04x\n", desc.idVendor, desc.idProduct);
	printDetail(&desc);

	if (handle) {
		libusb_close (handle);
		handle = NULL;
	}

	rc = libusb_open (dev, &handle);
	if (LIBUSB_SUCCESS != rc) {
		fprintf (stderr, "Error opening device\n");
	}

	done++;

	return 0;
}

static int LIBUSB_CALL hotplug_callback_detach(
		libusb_context 			*ctx,
		libusb_device 			*dev,
		libusb_hotplug_event 	event,
		void *					user_data)
{
	(void)ctx;
	(void)dev;
	(void)event;
	(void)user_data;

	struct libusb_device_descriptor desc;
	int rc;

	rc = libusb_get_device_descriptor(dev, &desc);
	if (LIBUSB_SUCCESS != rc) {
		fprintf (stderr, "Error getting device descriptor\n");
	}

	printf ("Device dettached: \n");
	printDetail(&desc);



	//printf ("Device detached\n");

	if (handle) {
		libusb_close (handle);
		handle = NULL;
	}

	done++;

	return 0;
}

int main_hotplugtest(int argc, char *argv[])
{
	libusb_hotplug_callback_handle hp[2];
	int product_id = 0;
	int vendor_id = 0;
	int class_id = 0;
	int rc;

	vendor_id  = (argc > 2) ? (int)strtol (argv[2], NULL, 0) : LIBUSB_HOTPLUG_MATCH_ANY;
	product_id = (argc > 3) ? (int)strtol (argv[3], NULL, 0) : LIBUSB_HOTPLUG_MATCH_ANY;
	class_id   = (argc > 4) ? (int)strtol (argv[4], NULL, 0) : LIBUSB_HOTPLUG_MATCH_ANY;

	rc = libusb_init (NULL);
	if (rc < 0)
	{
		printf("failed to initialise libusb: %s\n", libusb_error_name(rc));
		return EXIT_FAILURE;
	}

	if (!libusb_has_capability (LIBUSB_CAP_HAS_HOTPLUG)) {
		printf ("Hotplug capabilites are not supported on this platform\n");
		libusb_exit (NULL);
		return EXIT_FAILURE;
	}

	rc = libusb_hotplug_register_callback (NULL, LIBUSB_HOTPLUG_EVENT_DEVICE_ARRIVED, 0,
			vendor_id,// vendor_id,
			product_id, // product_id,
			class_id,
			hotplug_callback, NULL, &hp[0]);
	if (LIBUSB_SUCCESS != rc) {
		fprintf (stderr, "Error registering callback 0\n");
		libusb_exit (NULL);
		return EXIT_FAILURE;
	}

	rc = libusb_hotplug_register_callback (NULL, LIBUSB_HOTPLUG_EVENT_DEVICE_LEFT, 0,
			vendor_id,
			product_id,
			class_id,
			hotplug_callback_detach, NULL, &hp[1]);

	if (LIBUSB_SUCCESS != rc) {
		fprintf (stderr, "Error registering callback 1\n");
		libusb_exit (NULL);
		return EXIT_FAILURE;
	}

	while (1) {
		rc = libusb_handle_events (NULL);
		if (rc < 0)
			printf("libusb_handle_events() failed: %s\n", libusb_error_name(rc));
	}

	if (handle) {
		libusb_close (handle);
	}

	libusb_exit (NULL);

	return EXIT_SUCCESS;
}
