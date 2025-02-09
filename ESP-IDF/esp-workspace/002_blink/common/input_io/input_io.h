#ifndef INPUT_IO_H
#define INPUT_IO_H

#include "esp_err.h"
#include "hal/gpio_types.h" 
#include <stdio.h>
#include <esp_log.h>
#include "driver/gpio.h" 
#include "freertos/FreeRTOS.h"
#include "freertos/timers.h"
#include "freertos/FreeRTOSConfig.h"
#include "freertos/event_groups.h"
#include "freertos/timers.h"

typedef void (*input_callback_t) (int);
typedef void (*timeout_btn) (int);

typedef enum{
    LO_TO_HI = 1,
    HI_TO_LO = 2,
    ANY_EDLE = 3   
}   interrupt_type_edle_t;

#define BIT_EVENT_SHORTPRESS        (1<<0)
#define BIT_EVENT_NORMALPRESS       (1<<1)
#define BIT_EVENT_LONGPRESS         (1<<2)

void input_io_create(gpio_num_t gpio_num, interrupt_type_edle_t type);
int input_io_get_level(gpio_num_t gpio_num);
void input_set_callback(void * cb);

#endif