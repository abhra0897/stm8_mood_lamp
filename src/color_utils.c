#include "color_utils.h"



float g_rainbow_start_hue = 0.0;


void hsv_to_rgb(const CHSV_t *hsv_in, CRGB_t *rgb_out)
{
    float      hh, p, q, t, ff;
    long        i;
    float r, g, b;

    if(hsv_in->saturation <= 0.0) // < is bogus, just shuts up warnings
    {       
        rgb_out->red = hsv_in->value * 255.0;
        rgb_out->green = hsv_in->value * 255.0;
        rgb_out->blue = hsv_in->value * 255.0;
        return;
    }

    hh = hsv_in->hue;
    if(hh >= 360.0) hh = 0.0;
    hh /= 60.0;
    i = (long)hh;
    ff = hh - i;

    p = hsv_in->value * (1.0 - hsv_in->saturation);
    q = hsv_in->value * (1.0 - (hsv_in->saturation * ff));
    t = hsv_in->value * (1.0 - (hsv_in->saturation * (1.0 - ff)));

    switch(i) 
    {
        case 0:
            r = hsv_in->value;
            g = t;
            b = p;
            break;
        case 1:
            r = q;
            g = hsv_in->value;
            b = p;
            break;
        case 2:
            r = p;
            g = hsv_in->value;
            b = t;
            break;

        case 3:
            r = p;
            g = q;
            b = hsv_in->value;
            break;
        case 4:
            r = t;
            g = p;
            b = hsv_in->value;
            break;
        case 5:
        default:
            r = hsv_in->value;
            g = p;
            b = q;
            break;
    }

    rgb_out->red = r * 255.0;
    rgb_out->green = g * 255.0;
    rgb_out->blue = b * 255.0;
}


void rainbow_color_effect(CRGB_t *led_array, uint8_t led_num, hue_settings_t *hue_settings, float brightness)
{
    if (led_array == NULL)  return;

    float saturation = 1.0;
    CHSV_t hsv_color;
    hsv_color.hue = hue_settings->start_hue;
    hsv_color.saturation = saturation;
    hsv_color.value = brightness;

    for (uint8_t i = 0; i < led_num; i++)
    {
        hsv_to_rgb(&hsv_color, &led_array[i]);
        hsv_color.hue += hue_settings->hue_pix_diff;
        if (hsv_color.hue > hue_settings->hue_max)
            hsv_color.hue = hue_settings->hue_min + (hsv_color.hue - hue_settings->hue_max);
    }

    hue_settings->start_hue += hue_settings->hue_step; 
    if (hue_settings->start_hue > hue_settings->hue_max)
        hue_settings->start_hue = hue_settings->hue_min + (hue_settings->start_hue - hue_settings->hue_max);
}


void breathing_effect(CRGB_t *led_array, uint8_t led_num, uint8_t breath_delay, uint64_t ms_tick)
{
    if (led_array == NULL)  return;

    static uint8_t index = 0;
    static int8_t index_step = 1;
    static uint64_t last_update_time = 0;

    for (uint8_t i = 0; i < led_num; i++)
    {
        led_array[i].red /= breathing_effect_LUT[index] + 1;
        led_array[i].green /= breathing_effect_LUT[index] + 1;
        led_array[i].blue /= breathing_effect_LUT[index] + 1;
    }

    if (ms_tick - last_update_time > breath_delay)
    {
        index += index_step;
        last_update_time = ms_tick;

        if (index == 200)
        {
            index_step = -1;
        }
        else if (index == 0)
        {
            index_step = 1;
        }
    }
}