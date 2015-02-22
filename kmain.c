#include "framebuffer_driver.h"

void kmain()
{
    /* for some reason I wasn't able to pass the string literal into write.
     * I had to pass the array...Why is that?
     **/
    char hello[] = "kez-os Bitches!!1!";
    clear_screen();
    write(hello, sizeof(hello));
}
