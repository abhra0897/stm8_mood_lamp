#include "buttons.h"
#include "timer.h"
#include <stdint.h>


void buttons_gpio_config()
{
	PORT(BUTTONS_PORT, DDR) &= ~((1 << BUTTON_LEFT_PIN_POS) & (1 << BUTTON_MIDDLE_PIN_POS) & (1 << BUTTON_RIGHT_PIN_POS)); // Button pins as input
	PORT(BUTTONS_PORT, CR1) |= (1 << BUTTON_LEFT_PIN_POS) | (1 << BUTTON_MIDDLE_PIN_POS) | (1 << BUTTON_RIGHT_PIN_POS); // Input pullup 
	PORT(BUTTONS_PORT, CR2) &= ~((1 << BUTTON_LEFT_PIN_POS) & (1 << BUTTON_MIDDLE_PIN_POS) & (1 << BUTTON_RIGHT_PIN_POS)); // No interrupt
	PORT(BUTTONS_PORT, ODR) |= (1 << BUTTON_LEFT_PIN_POS) | (1 << BUTTON_MIDDLE_PIN_POS) | (1 << BUTTON_RIGHT_PIN_POS);
}

#ifndef BTN_DEBOUNCE_PERIOD
	#define BTN_DEBOUNCE_PERIOD		200
#endif
int8_t button_process_input(button_t *button, uint64_t tick_ms)
{
	if (button == NULL)
		return -1;
	if (button->allow_long_press && (button->long_press_duration < BTN_DEBOUNCE_PERIOD))
		return -1;
	if (button->press_repeat_interval < BTN_DEBOUNCE_PERIOD)
		button->press_repeat_interval = BTN_DEBOUNCE_PERIOD;

	uint64_t diff = 0;
	uint8_t event = BTN_EVENT_NONE;
	button->state = BUTTON_READ_VALUE(button->pin_pos) == 0 ? 0 : 1;

	/**
	 * Falling edge
	 */
	if (button->_prev_state == 1 && button->state == 0)
	{
		button->_start_time_falling = tick_ms;
	}
	/**
	 * Rising edge
	 */
	else if (button->_prev_state == 0 && button->state == 1)
	{
		diff = tick_ms - button->_start_time_falling;
		
		if (diff > BTN_DEBOUNCE_PERIOD)
		{
			/* if previous long press is running (but button is actually released), don't register a single press yet. break the long press first. */
			if (button->_long_press_running)
			{
				/* We've got a rising edge of a few ms, so we now stop the previous long press */
				button->_long_press_running = 0;
			}
			else
			{
				event = BTN_EVENT_PRESSED;
			}
		}
	}
	/**
	 * Low level
	 */
	else if (button->_prev_state == 0 && button->state == 0)
	{
		diff = tick_ms - button->_start_time_falling;
		
		if (button->allow_long_press)
		{
			/* if long press is not currently running, only then we may consider it as a long press */
			if (diff >= button->long_press_duration && !button->_long_press_running)
			{
				/* Long press is now running. It will only stop after a rising edge of a few ms */
				button->_long_press_running = 1;
				event = BTN_EVENT_LONG_PRESSED;
			}
		}
		else
		{
			if (diff > button->press_repeat_interval)
			{
				event = BTN_EVENT_PRESSED;
				/* Reset the tick to get the next single press */
				button->_start_time_falling = tick_ms;
			}
		}
	}

	button->_prev_state = button->state;
	button->event = event;
	return 0;
}