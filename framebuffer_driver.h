#ifndef INCLUDE_FRAMEBUFFER_DRIVER_H
#define INCLUDE_FRAMEBUFFER_DRIVER_H


/** clear_screen:
 *  Clear the buffer.  Clearing is looping through all the rows/columns and
 *  setting char to ' ' and bg and fg to black.
 **/
void clear_screen();

/** write:
 *  Write a char buffer to screen.
 *
 *  @param buf The char buffer
 *  @param len How long the buffer is
 **/
void fb_write(char *buf, unsigned int len);

#endif /* INCLUDE_FRAMEBUFFER_DRIVER_H */
