/* Author: Alexander Anderson
 * Description: Driver for IO to serial ports */

#include "io.h"

/* All the I/O ports are calculated relative to the data port. This is because
 * all serial ports (COM1, COM2, COM3, COM4) have their ports in the same
 * order, but they start at different values.
 */

#define SERIAL_COM1_BASE                0x3F8      /* COM1 base port */

#define SERIAL_DATA_PORT(base)          (base)
#define SERIAL_FIFO_COMMAND_PORT(base)  (base + 2)
#define SERIAL_LINE_COMMAND_PORT(base)  (base + 3)
#define SERIAL_MODEM_COMMAND_PORT(base) (base + 4)
#define SERIAL_LINE_STATUS_PORT(base)   (base + 5)

/* Tells the serial port to expect first the highest 8 bits on the data port,
 *   then the lowest 8 bits will follow */
#define SERIAL_LINE_ENABLE_DLAB         0x80

/* Size in bytes of the FIFO buffer of the serial port */
#define NBYTES_FIFO_BUFFER              14

/**
 * Sets the speed of the data being sent. The default speed of a serial
 *   port is 115200 bits/s. The argument is a divisor of that number, hence
 *   the resulting speed becomes (115200 / divisor) bits/s.
 *
 * @param [in] com     - The COM port to configure
 * @param [in] divisor - The divisor
 */
void serial_configure_baud_rate(unsigned short com, unsigned short divisor)
{
    /* Enable DLAB while preserving the previous configuration of the reg */
    unsigned char reg_val = inb(SERIAL_LINE_COMMAND_PORT(com));
    reg_val |= SERIAL_LINE_ENABLE_DLAB;
    outb(SERIAL_LINE_COMMAND_PORT(com), reg_val);

    /* Set the baud rate */
    outb(SERIAL_DATA_PORT(com), (divisor >> 8) & 0x00FF);
    outb(SERIAL_DATA_PORT(com), divisor & 0x00FF);

    /* Disable DLAB now that we're finished */
    reg_val &= ~SERIAL_LINE_ENABLE_DLAB;
    outb(SERIAL_LINE_COMMAND_PORT(com), reg_val);
}

/**
 * Configures the line of the given serial port. The port is set to have a
 *   data length of 8 bits, no parity bits, one stop bit and break control
 *   disabled.
 *
 * @param [in] com - The serial port to configure
 */
void serial_configure_line(unsigned short com)
{
    /* Bit:     | 7 | 6 | 5 4 3 | 2 | 1 0 |
     * Content: | d | b | prty  | s | dl  |
     * Value:   | 0 | 0 | 0 0 0 | 0 | 1 1 | = 0x03
     */
    outb(SERIAL_LINE_COMMAND_PORT(com), 0x03);
}

/**
 * Configures the FIFO buffers of a serial port. The port is set to
 *   enable FIFO, clear both receiver and transmission FIFO queues,
 *   and use 14 bytes as size of queue
 *
 * @param [in] com - The serial port to configure
 */
void serial_configure_fifo_buffers(unsigned short com)
{
    /* Bit:     | 7 6 | 5  | 4 | 3   | 2   | 1   | 0 |
     * Content: | lvl | bs | r | dma | clt | clr | e |
     * Value:   | 1 1 | 0  | 0 | 0   | 1   | 1   | 1 | = 0xC7
     */
    outb(SERIAL_FIFO_COMMAND_PORT(com), 0xC7);
}

/**
 * Configures the modem of a serial port. The port is set to flag
 *   Ready To Transmit and Data Terminal Ready
 *
 * @param [in] com - The serial port to configure
 */
void serial_configure_modem(unsigned short com)
{
    /* Bit:     | 7 | 6 | 5  | 4  | 3   | 2   | 1   | 0   |
     * Content: | r | r | af | lb | ao2 | ao1 | rts | dtr |
     * Value:   | 0 | 0 | 0  | 0  | 0   | 0   | 1   | 1   | = 0x03
     */
    outb(SERIAL_MODEM_COMMAND_PORT(com), 0x03);
}

/**
 * Checks whether the transmit FIFO queue is empty or not for the given COM
 *   port.
 *
 * @param [in] com - The COM port
 *
 * @return 0 if the transmit FIFO queue is not empty
 * @return 1 if the transmit FIFO queue is empty
 */
int serial_transmit_fifo_is_empty(unsigned int com)
{
    /* 0x20 = 0010 0000 */
    return inb(SERIAL_LINE_STATUS_PORT(com)) & 0x20;
}

void serial_init()
{
    serial_configure_baud_rate(SERIAL_COM1_BASE, 1);
    serial_configure_line(SERIAL_COM1_BASE);
    serial_configure_fifo_buffers(SERIAL_COM1_BASE);
    serial_configure_modem(SERIAL_COM1_BASE);
}

int serial_write(char* buf, unsigned int len)
{
    if (!buf)
    {
        return -1;
    }

    /* Write to the serial data port until the full buf was written */
    while (len > 0)
    {
        /* Spin till the buffers can be written to */
        while (!serial_transmit_fifo_is_empty(SERIAL_COM1_BASE)) {}

        /* Write buf to the data port, but no more bytes than the size
         *   of the FIFO queue */
        unsigned int i;
        for (i = 0; (i < len) && (i < NBYTES_FIFO_BUFFER); ++i)
        {
            outb(SERIAL_DATA_PORT(SERIAL_COM1_BASE), buf[i]);
        }

        /* Advance the buf pointer for the next burst */
        buf += i;
        len -= i;
    }

    return 0;
}

