/**
 * @file main.h
 * @author Avra Mitra
 * @brief Header for main.c. Modify LED relate settings here.
 * @date 2021-12-20
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#ifndef _MAIN_H
#define _MAIN_H

#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>
#include "stm8s.h"
#include "ws2812_driver.h"
#include "timer.h"
#include "buttons.h"
#include "color_utils.h"
#include "flash.h"

#define LED_COUNT               10
#define MAX_EFFECTS_CNT         2

#define MODE_SPEED              1
#define MODE_PATTERN            2
#define MODE_BRIGHTNESS         3

#define MIN_BRIGHTNESS          0
#define MAX_BRIGHTNESS          0.8

#define RAINBOW_MIN_STEP        0
#define RAINBOW_MAX_STEP        2

#define RAINBOW_MIN_PIX_DIFF    0
#define RAINBOW_MAX_PIX_DIFF    30

/**
 * @brief Configure buttons settings
 */
void buttons_config_settings();
/**
 * @brief Scan all the buttons for inputs
 */
void buttons_scan();
/**
 * @brief Handle the input and set mode related parameters
 * This is where user interacts with the hardware
 */
void handle_input();
/**
 * @brief Depending on selected mode, blink led to visually inform the user.
 * @param led pointer to the led for setting the color
 */
void handle_mode_led_blink(CRGB_t *led);

/**
 * @brief Save current led seetings in eeprom when a button event occurs
 * So we can recover the settings after power loss
 * @param address_offset EEPROM address offset to write
 */
void save_settings_in_eeprom(uint16_t address_offset);

/**
 * @brief Read led settings from eeprom and store in relevant global vars
 * @param address_offset EEPROM address offset to write
 */
void recover_from_eeprom(uint16_t address_offset);

#endif // _MAIN_H