#ifndef _BUTTONS_H
#define _BUTTONS_H

#include "stm8s.h"
#include <stdint.h> 

#define BUTTONS_PORT                PC
#define BUTTON_LEFT_PIN_POS         5
#define BUTTON_MIDDLE_PIN_POS       6
#define BUTTON_RIGHT_PIN_POS        7

#define BTN_EVENT_NONE				0
#define BTN_EVENT_PRESSED			1
#define BTN_EVENT_LONG_PRESSED		2

#define BTN_DEBOUNCE_PERIOD		200

#define BUTTON_READ_VALUE(gpio_pos)         (PORT(BUTTONS_PORT, IDR) & (1 << (gpio_pos)))


typedef struct 
{
    /**
     * ![NOTE]: variables start with underscore are for private use only
     */
    uint64_t _start_time_falling;
    uint16_t press_repeat_interval;
    uint16_t long_press_duration;
    uint8_t _prev_state;
    uint8_t _long_press_running;
    uint8_t allow_long_press;
    uint8_t pin_pos;
    uint8_t event;
    uint8_t state;
} button_t;

/**
 * @brief Configure the gpios that are used for button inputs
 */
void buttons_gpio_config();

/**
 * @brief Process raw inputs and sets state and events.
 * TODO: Optimize, and add support for double-click
 * @param button pointer to the button struct
 * @param tick_ms tick in ms
 * @return int8_t returns 0 on success, -1 otherwise
 */
int8_t button_process_input(button_t *button, uint64_t tick_ms);

#endif