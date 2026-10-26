#include <graphx.h>
#include <ti/getcsc.h>

int main(void)
{
    gfx_Begin();
    gfx_PrintStringXY("Hello, world!", 110, 116);
    while (!os_GetCSC());
    gfx_End();
    return 0;
}
