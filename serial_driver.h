/* Author: Alexander Anderson
 * Description: Driver for IO to serial ports */

#ifndef INCLUDE_SERIAL_DRIVER_H
#define INCLUDE_SERIAL_DRIVER_H

/**
 * Initializes the COM1 serial port
 */
void serial_init();

/**
 * Write a char buffer to the COM1 serial port
 *
 * @param [in] buf - The char buffer
 * @param [in] len - How long the buffer is
 *
 * @return 0 means success
 * @return -1 means failure
 */
int serial_write(char* buf, unsigned int len);

#endif /* INCLUDE_SERIAL_DRIVER_H */

