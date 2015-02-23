#include "framebuffer_driver.h"
#include "serial_driver.h"

void kmain()
{
    /* for some reason I wasn't able to pass the string literal into write.
     * I had to pass the array...Why is that?
     **/
    char hello[] = "kez-os Bitches!!!!";
    clear_screen();
    fb_write(hello, sizeof(hello));

    char log[] = "WARN: Serial logging like a badass!";
    serial_init();
    serial_write(log, sizeof(log));
}
