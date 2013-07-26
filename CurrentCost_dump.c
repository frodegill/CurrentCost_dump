/*
 *  CurrentCost_dump
 *
 *  Created by Frode Roxrud Gill, 2013.
 *  This code is Public Domain.
 */

#include <signal.h>
#include <unistd.h>

#include "ctb-0.16/ctb.h"

#define DEFAULT_DEVICE     "/dev/ttyUSB0"
#define ENVIR_BAUDRATE     (57600)
#define ENVIR_PROTOCOL     "8N1"
#define ENVIR_FLOWCONTROL  (ctb::SerialPort::NoFlowControl)

#define EXIT_OK                 (0)
#define EXIT_OUT_OF_MEMORY      (1)
#define EXIT_SERIAL_OPEN_FAILED (2)
#define EXIT_READ_FAILED        (3)

#define BUFFER_SIZE        (8*1024)

bool g_terminate = false;

extern "C" void TermSignalCallbackHandler(int)
{
	g_terminate = true;
}

int main(int argc, char* argv[])
{
	signal(SIGTERM, TermSignalCallbackHandler);

	char* buf = new char[BUFFER_SIZE+1];
	if (!buf)
		return EXIT_OUT_OF_MEMORY;

	const char* device = (2<=argc) ? argv[1] : DEFAULT_DEVICE;
	ctb::SerialPort* serialPort = new ctb::SerialPort();
	int serial_fd = serialPort->Open(device, ENVIR_BAUDRATE, ENVIR_PROTOCOL, ENVIR_FLOWCONTROL);
	
	int bytes_read;
	while (!g_terminate)
	{
		bytes_read = serialPort->Read(buf, BUFFER_SIZE);
		if (0 > bytes_read) //Read failed. Abort
		{
			delete[] buf;
			printf("%s: Read failed (do you have permission, and is CurrentCost USB cable attached?)\n", device);
			return EXIT_READ_FAILED;
		}
		else if (0 == bytes_read) //Nothing to read
		{
			ctb::sleepms(1000);
		}
		else //We got content!
		{
			buf[bytes_read] = 0;
			fputs(buf, stdout);
		}
	}
	delete[] buf;
	close(serial_fd);
	return EXIT_OK;	
}
