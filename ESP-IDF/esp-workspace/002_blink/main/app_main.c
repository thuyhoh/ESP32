#include <stdio.h>
#include <inttypes.h>
#include "sdkconfig.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "input_io.h"
#include "output_io.h"

void app_main(void)
{
    output_io_create(GPIO_NUM_0);
    while(1)
    {
        output_io_set_level(GPIO_NUM_0, 1);
        vTaskDelay(100);
        output_io_set_level(GPIO_NUM_0, 0);
        vTaskDelay(100);
    }
}