#include "util.h"
#include <ti/screen.h>
#include <ti/getcsc.h>

int main(void){
    os_ClrHome();

    char message[] = "Hello, World!";
    centerPrintText(message);

    while (!os_GetCSC());
    return 0;
}