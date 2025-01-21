#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "driver/gpio.h"

#include "lib.h"


void app_main(void)
{
    
    while(1)
    {
        printHello();
    }
    // return 0;
}

