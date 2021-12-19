#include "main.h"


button_t g_right_button, g_left_button, g_middle_button;
uint8_t g_mode = MODE_SPEED;    // current mode (set by middle button single press)
float g_brightness = 0.5;       // brightness (0 to 1)
uint8_t g_is_breathing = 0;     // breathing flag
uint64_t g_tick_ms = 0;         // in ms
uint8_t g_breathing_delay = 1;  // in ms
hue_settings_t g_rainbow_settings =
{
    .hue_min = 0.0,
    .hue_max = 360.0,
    .hue_step = 2,
    .hue_pix_diff = 30.0,
    .start_hue = 220.0
};


void main(void)
{
    /* Set clock to full speed (16 Mhz) */
    CLK_CKDIVR = 0;

    /* Configure required drivers */
    timer_config();
    enableInterrupts();
    buttons_gpio_config();
    ws2812_gpio_config();
    buttons_config_settings();

    /* The led array */
    CRGB_t led_array[LED_COUNT];
    
    while(1)
    {
        g_tick_ms = timer_get_ms_tick();
        buttons_scan();
        handle_input();
        
        rainbow_color_effect(led_array, LED_COUNT, &g_rainbow_settings, g_brightness);
        if (g_is_breathing)
        {
            breathing_effect(led_array, LED_COUNT, g_breathing_delay, g_tick_ms);
        }
        handle_mode_led_blink(&led_array[0]);
        for (uint8_t led_cnt = 0; led_cnt < LED_COUNT; led_cnt++)
        {
            ws2812_send_pixel_24bits(led_array[led_cnt].red, led_array[led_cnt].green, led_array[led_cnt].blue);
        }
        ws2812_send_latch();  
    }
}



void buttons_config_settings()
{
    g_right_button.pin_pos = BUTTON_RIGHT_PIN_POS;
    g_right_button.allow_long_press = false;
    g_right_button.press_repeat_interval = 300;

    g_left_button.pin_pos = BUTTON_LEFT_PIN_POS;
    g_left_button.allow_long_press = false;
    g_left_button.press_repeat_interval = 300;

    g_middle_button.pin_pos = BUTTON_MIDDLE_PIN_POS;
    g_middle_button.allow_long_press = true;
    g_middle_button.press_repeat_interval = 400;
    g_middle_button.long_press_duration = 1500;
}

void buttons_scan()
{ 
    button_process_input(&g_right_button, g_tick_ms);
    button_process_input(&g_left_button, g_tick_ms);
    button_process_input(&g_middle_button, g_tick_ms);
}

void handle_input()
{
    /**
     * Middle Button Single Press:
     * Go to next mode expect when breathing is enabled
     */
    if (g_middle_button.event == BTN_EVENT_PRESSED)
    {
        if (!g_is_breathing)
        {
            ++g_mode;
            if (g_mode > 3)
                g_mode = MODE_SPEED;
        }
    }

    /**
     * Middle Button Long Press:
     * Enable breathing effect
     */
    else if (g_middle_button.event == BTN_EVENT_LONG_PRESSED)
    {
        g_is_breathing = g_is_breathing ? 0 : 1;
    }

    /**
     * Left Button Single Press:
     * 1. if Breathing enabled:         Decrease breathing speed
     * 2. elif Mode is MODE_SPEED:      Decrease speed
     * 3. elif Mode is MODE_BRIGHTNESS: Decrease Brightness
     * 4. elif Mode is MODE_PATTERN:    Decrease pixel difference hue
     */
    else if (g_left_button.event == BTN_EVENT_PRESSED)
    {
        if (g_is_breathing)
        {
            --g_breathing_delay;
            if (g_breathing_delay < 1)
                g_breathing_delay = 1;
        }
        else if (g_mode == MODE_SPEED)
        {
            g_rainbow_settings.hue_step -= .1;
            if (g_rainbow_settings.hue_step <= RAINBOW_MIN_STEP)
                g_rainbow_settings.hue_step = RAINBOW_MIN_STEP; 
        }
        else if (g_mode == MODE_BRIGHTNESS)
        {
            g_brightness -= 0.1;
            if (g_brightness <= 0) g_brightness = 0;
        }
        else if (g_mode == MODE_PATTERN)
        {
            g_rainbow_settings.hue_pix_diff -= 2;
            if (g_rainbow_settings.hue_pix_diff < RAINBOW_MIN_PIX_DIFF)
                g_rainbow_settings.hue_pix_diff = RAINBOW_MAX_PIX_DIFF; // roll over
        }
    }

    /**
     * Right Button Single Press:
     * 1. if Breathing enabled:         Increase breathing speed
     * 2. elif Mode is MODE_SPEED:      Increase speed
     * 3. elif Mode is MODE_BRIGHTNESS: Increase Brightness
     * 4. elif Mode is MODE_PATTERN:    Increase pixel difference hue
     */
    else if (g_right_button.event == BTN_EVENT_PRESSED)
    {
        if (g_is_breathing)
        {
            ++g_breathing_delay;
            if (g_breathing_delay > 8)
                g_breathing_delay = 8;
        }
        else if (g_mode == MODE_SPEED)
        {
            g_rainbow_settings.hue_step += .1;
            if (g_rainbow_settings.hue_step >= RAINBOW_MAX_STEP)
                g_rainbow_settings.hue_step = RAINBOW_MAX_STEP;
        }
        else if (g_mode == MODE_BRIGHTNESS)
        {
            g_brightness += 0.1;
            if (g_brightness > MAX_BRIGHTNESS) g_brightness = MAX_BRIGHTNESS;
        }
        else if (g_mode == MODE_PATTERN)
        {
            g_rainbow_settings.hue_pix_diff += 2;
            if (g_rainbow_settings.hue_pix_diff > RAINBOW_MAX_PIX_DIFF)
                g_rainbow_settings.hue_pix_diff = RAINBOW_MIN_PIX_DIFF; // roll over
        }
    }
}

void handle_mode_led_blink(CRGB_t *led)
{
    // mode blinking will stop when breathing is enabled
    if (g_is_breathing) return;

    static uint64_t blink_start = 0;
    if (g_mode == MODE_BRIGHTNESS || g_mode == MODE_PATTERN)
    {
        uint8_t period = g_mode == MODE_BRIGHTNESS ? 100 : 500;
        static uint8_t blink_state;
        if (g_tick_ms - blink_start > period)
        {
            blink_start = g_tick_ms;
            blink_state = blink_state ? 0 : 1;
        }
        led->red = 20 * blink_state;
        led->green = 20 * blink_state;
        led->blue = 20 * blink_state; 
    }
}

