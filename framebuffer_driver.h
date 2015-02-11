#ifndef INCLUDE_FRAMEBUFFER_DRIVER_H
#define INCLUDE_FRAMEBUFFER_DRIVER_H

/**
 * write a char buffer to screen.
 *
 * @param char * buf   - the char buffer
 * @param unsigned int - how long the buffer is.
 *
 **/
int write(char *buf, unsigned int len);

/**
 * Public method to clear the buffer.  Clearing is
 * looping through all the rows/columns and setting char to ' '
 * and bg and fg to black.
 **/
int clear_screen();

#endif /* INCLUDE_IO_H */
