#include <stdio.h>
#include <inttypes.h>
#include "sdkconfig.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/timers.h"
#include "driver/gpio.h"
#include "input_io.h"
#include "output_io.h"

#define LED GPIO_NUM_2

TimerHandle_t xTimers[2];

void vTimerCallback(TimerHandle_t xTimer)
{
    uint32_t ulCount;
    configASSERT(xTimer);
    ulCount = (uint32_t)pvTimerGetTimerID(xTimer);
    if (ulCount == 0)
    {
        output_io_toggle(LED);
    }
    else if (ulCount == 1)
    {
        printf("hello\n");
    }
}

void app_main(void)
{
    xTimers[0] = xTimerCreate("Timer1", pdMS_TO_TICKS(500), pdTRUE, (void *)0, vTimerCallback);
    xTimers[1] = xTimerCreate("Timer2", pdMS_TO_TICKS(500), pdTRUE, (void *)1, vTimerCallback);

    output_io_create(LED);

    xTimerStart(xTimers[0], 0);
    xTimerStart(xTimers[1], 0);

    while(1)
    {

    }
}
