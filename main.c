#include <stdint.h>
#include <avr/io.h>
#include <avr/wdt.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#include <avr/pgmspace.h>
#include "v-usb/usbdrv/usbdrv.h"

#include "n64.h"

//#define UART_DEBUG

PROGMEM const char usbHidReportDescriptor[USB_CFG_HID_REPORT_DESCRIPTOR_LENGTH] =
{
	//N64 ID: 1
    0x05, 0x01,                    // USAGE_PAGE (Generic Desktop)
    0x09, 0x05,                    // USAGE (Game Pad)
    0xa1, 0x01,                    // COLLECTION (Application)
    0x85, 0x01,                    //   REPORT_ID (1)
    0x09, 0x01,                    //   USAGE (Pointer)
    0xa1, 0x00,                    //   COLLECTION (Physical)
    0x09, 0x30,                    //     USAGE (X)
    0x09, 0x31,                    //     USAGE (Y)
    0x15, 0x80,                    //     LOGICAL_MINIMUM (-128)
    0x25, 0x7f,                    //     LOGICAL_MAXIMUM (127)
    0x75, 0x08,                    //     REPORT_SIZE (8)
    0x95, 0x02,                    //     REPORT_COUNT (2)
    0x81, 0x02,                    //     INPUT (Data,Var,Abs)
    0xc0,                          //   END_COLLECTION
    0x05, 0x09,                    //   USAGE_PAGE (Button)
    0x19, 0x01,                    //   USAGE_MINIMUM (Button 1)
    0x29, 0x10,                    //   USAGE_MAXIMUM (Button 16)
    0x15, 0x00,                    //   LOGICAL_MINIMUM (0)
    0x25, 0x01,                    //   LOGICAL_MAXIMUM (1)
    0x75, 0x01,                    //   REPORT_SIZE (1)
    0x95, 0x10,                    //   REPORT_COUNT (16)
    0x81, 0x02,                    //   INPUT (Data,Var,Abs)
    0xc0,                          // END_COLLECTION
	//N64 ID: 2
    0x05, 0x01,                    // USAGE_PAGE (Generic Desktop)
    0x09, 0x05,                    // USAGE (Game Pad)
    0xa1, 0x01,                    // COLLECTION (Application)
    0x85, 0x02,                    //   REPORT_ID (2)
    0x09, 0x01,                    //   USAGE (Pointer)
    0xa1, 0x00,                    //   COLLECTION (Physical)
    0x09, 0x30,                    //     USAGE (X)
    0x09, 0x31,                    //     USAGE (Y)
    0x15, 0x80,                    //     LOGICAL_MINIMUM (-128)
    0x25, 0x7f,                    //     LOGICAL_MAXIMUM (127)
    0x75, 0x08,                    //     REPORT_SIZE (8)
    0x95, 0x02,                    //     REPORT_COUNT (2)
    0x81, 0x02,                    //     INPUT (Data,Var,Abs)
    0xc0,                          //   END_COLLECTION
    0x05, 0x09,                    //   USAGE_PAGE (Button)
    0x19, 0x01,                    //   USAGE_MINIMUM (Button 1)
    0x29, 0x10,                    //   USAGE_MAXIMUM (Button 16)
    0x15, 0x00,                    //   LOGICAL_MINIMUM (0)
    0x25, 0x01,                    //   LOGICAL_MAXIMUM (1)
    0x75, 0x01,                    //   REPORT_SIZE (1)
    0x95, 0x10,                    //   REPORT_COUNT (16)
    0x81, 0x02,                    //   INPUT (Data,Var,Abs)
    0xc0,                          // END_COLLECTION
	//GC ID: 3
    0x05, 0x01,                    // USAGE_PAGE (Generic Desktop)
    0x09, 0x05,                    // USAGE (Game Pad)
    0xa1, 0x01,                    // COLLECTION (Application)
    0x85, 0x03,                    //   REPORT_ID (3)
    0x09, 0x01,                    //   USAGE (Pointer)
    0xa1, 0x00,                    //   COLLECTION (Physical)
    0x09, 0x30,                    //     USAGE (X)
    0x09, 0x31,                    //     USAGE (Y)
    //0x09, 0x32,                    //     USAGE (Z)
    0x09, 0x33,                    //     USAGE (Rx)
    0x09, 0x34,                    //     USAGE (Ry)
    //0x09, 0x35,                    //     USAGE (Rz)
    0x15, 0x00,                    //     LOGICAL_MINIMUM (0)
    0x26, 0xff, 0x00,              //     LOGICAL_MAXIMUM (255)
    0x75, 0x08,                    //     REPORT_SIZE (8)
    0x95, 0x04,                    //     REPORT_COUNT (4)
    0x81, 0x02,                    //     INPUT (Data,Var,Abs)
    0xc0,                          //   END_COLLECTION
    0x05, 0x09,                    //   USAGE_PAGE (Button)
    0x19, 0x01,                    //   USAGE_MINIMUM (Button 1)
    0x29, 0x10,                    //   USAGE_MAXIMUM (Button 16)
    0x15, 0x00,                    //   LOGICAL_MINIMUM (0)
    0x25, 0x01,                    //   LOGICAL_MAXIMUM (1)
    0x75, 0x01,                    //   REPORT_SIZE (1)
    0x95, 0x10,                    //   REPORT_COUNT (16)
    0x81, 0x02,                    //   INPUT (Data,Var,Abs)
    0xc0,                          // END_COLLECTION
	//GC ID: 4
    0x05, 0x01,                    // USAGE_PAGE (Generic Desktop)
    0x09, 0x05,                    // USAGE (Game Pad)
    0xa1, 0x01,                    // COLLECTION (Application)
    0x85, 0x04,                    //   REPORT_ID (4)
    0x09, 0x01,                    //   USAGE (Pointer)
    0xa1, 0x00,                    //   COLLECTION (Physical)
    0x09, 0x30,                    //     USAGE (X)
    0x09, 0x31,                    //     USAGE (Y)
	//0x09, 0x32,                    //     USAGE (Z)
    0x09, 0x33,                    //     USAGE (Rx)
    0x09, 0x34,                    //     USAGE (Ry)
    //0x09, 0x35,                    //     USAGE (Rz)
    0x15, 0x00,                    //     LOGICAL_MINIMUM (0)
    0x26, 0xff, 0x00,              //     LOGICAL_MAXIMUM (255)
    0x75, 0x08,                    //     REPORT_SIZE (8)
    0x95, 0x04,                    //     REPORT_COUNT (4)
    0x81, 0x02,                    //     INPUT (Data,Var,Abs)
    0xc0,                          //   END_COLLECTION
    0x05, 0x09,                    //   USAGE_PAGE (Button)
    0x19, 0x01,                    //   USAGE_MINIMUM (Button 1)
    0x29, 0x10,                    //   USAGE_MAXIMUM (Button 16)
    0x15, 0x00,                    //   LOGICAL_MINIMUM (0)
    0x25, 0x01,                    //   LOGICAL_MAXIMUM (1)
    0x75, 0x01,                    //   REPORT_SIZE (1)
    0x95, 0x10,                    //   REPORT_COUNT (16)
    0x81, 0x02,                    //   INPUT (Data,Var,Abs)
    0xc0                           // END_COLLECTION
	//Length: 194
};

struct __attribute__((__packed__)) gc_report
{
	int8_t report_id;
    struct gc_controller_state state;
};

struct __attribute__((__packed__)) n64_report
{
	int8_t report_id;
    struct n64_controller_state state;
};

static uint8_t controller_type;
static struct gc_report gc_report;
static struct n64_report n64_report;
static uchar idle_rate;

usbMsgLen_t usbFunctionSetup(uchar data[8])
{
	usbRequest_t *rq = (void *)data;

    if((rq->bmRequestType & USBRQ_TYPE_MASK) == USBRQ_TYPE_CLASS)
	{
		if(rq->bRequest == USBRQ_HID_GET_REPORT)
		{
			if(controller_type == GC_CONTROLLER)
			{
				usbMsgPtr = (void *)&gc_report;
				return sizeof(gc_report);
			}

			usbMsgPtr = (void *)&n64_report;
			return sizeof(n64_report);
        }
		else if(rq->bRequest == USBRQ_HID_GET_IDLE)
		{
			usbMsgPtr = &idle_rate;
			return 1;
        }
		else if(rq->bRequest == USBRQ_HID_SET_IDLE)
            idle_rate = rq->wValue.bytes[1];
    }

    return 0;
}

#ifdef UART_DEBUG

static void uart_send_byte(uint8_t byte)
{
	while (!(UCSR0A & (uint8_t)(1<<UDRE0)));
    UDR0 = byte;
}

static void uart_init()
{
	UCSR0C = (1<<UCSZ01)|(1<<UCSZ00);

	UBRR0L = 103;
	UBRR0H = 103 >> 8;

	UCSR0A = 0;

	UCSR0B = (1<<TXEN0);
}

#endif

static void usbSendReport(const void *report, int length)
{
	while (!usbInterruptIsReady())
		usbPoll();

	usbSetInterrupt((uchar *)report, length);
}

int main()
{
	uint8_t buffer[MAX_RESPONSE_SIZE];
	uint8_t type_request[1] = { NTD_QUERY_CONTROLLER_TYPE };
	uint8_t n64_bt_request[1] = { N64_QUERY_BUTTONS };
	uint8_t gc_bt_request[3] = { GC_QUERY_BUTTONS_0, GC_QUERY_BUTTONS_1, GC_QUERY_BUTTONS_2 };
	int length;

	wdt_disable();

#ifndef UART_DEBUG
	usbInit();
    usbDeviceDisconnect();

	_delay_ms(200);

    usbDeviceConnect();
#else
	uart_init();
	length = ntd_request(type_request, 1, buffer, 1<<PC2);

	for(int i=0; i<length; i++)
		uart_send_byte(buffer[i]);
	uart_send_byte(0xff);
#endif

    sei();

    for(;;)
	{
		#ifndef UART_DEBUG

		n64_report.report_id = 1;
		gc_report.report_id = 3;

		for(int i=PC0; i<=PC3; i++)
		{
			length = ntd_request(type_request, 1, buffer, 1<<i);
			controller_type = ntd_controller_type(buffer, length);

			if(controller_type == N64_CONTROLLER)
			{
				_delay_ms(1);
				length = ntd_request(n64_bt_request, 1, buffer, 1<<i);

				if(n64_parse_response(buffer, length, &n64_report.state))
					usbSendReport((void *)&n64_report, sizeof(n64_report));

				n64_report.report_id++;
			}
			else if(controller_type == GC_CONTROLLER)
			{
				length = ntd_request(gc_bt_request, 3, buffer, 1<<i);

				if(gc_parse_response(buffer, length, &gc_report.state))
					usbSendReport((void *)&gc_report, sizeof(gc_report));

				gc_report.report_id++;
			}
		}

		#else

		length = ntd_request(gc_bt_request, 3, buffer, 1<<PC0);

		for(int i=0; i<length; i++)
			uart_send_byte(buffer[i]);

		gc_parse_response(buffer, length, &gc_report.state);

		uart_send_byte(gc_report.state.buttons);
		uart_send_byte(gc_report.state.buttons>>8);
		uart_send_byte(gc_report.state.left_x);
		uart_send_byte(gc_report.state.left_y);
		uart_send_byte(gc_report.state.right_x);
		uart_send_byte(gc_report.state.right_y);

		uart_send_byte(length);
		uart_send_byte(0xff);

		#endif
    }
}

