#include <stdint.h>
#include <stdio.h>
#include <math.h>

typedef struct _CRGB
{
    uint8_t red;        // 0 to 255
    uint8_t green;      // 0 to 255
    uint8_t blue;       // 0 to 255
    
}CRGB_t;

typedef struct _CHSV
{
    float hue;          // 0.0 to 360.0
    float saturation;   // 0.0 to 1.0
    float value;        // 0.0 to 1.0

}CHSV_t;

typedef struct 
{
    float hue_min;
    float hue_max;
    float hue_step;
    float hue_pix_diff;
    float start_hue;
}hue_settings_t;


static const uint8_t breathing_effect_LUT[] = 
{
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
    0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2, 2, 
    3, 3, 3, 3, 4, 4, 4, 4, 5, 5, 5, 5, 6, 6, 6, 7, 
    7, 7, 7, 8, 8, 9, 9, 9, 10, 10, 10, 11, 11, 12, 12, 13, 
    13, 14, 14, 15, 15, 16, 16, 17, 17, 18, 18, 19, 19, 20, 21, 21, 
    22, 22, 23, 24, 24, 25, 26, 27, 27, 28, 29, 30, 30, 31, 32, 33, 
    34, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 
    49, 50, 51, 52, 53, 54, 56, 57, 58, 59, 60, 62, 63, 64, 65, 67, 
    68, 69, 71, 72, 74, 75, 76, 78, 79, 81, 82, 84, 85, 87, 88, 90, 
    91, 93, 95, 96, 98, 100, 101, 103, 105, 106, 108, 110, 112, 113, 115, 117, 
    119, 120, 122, 124, 126, 128, 130, 132, 133, 135, 137, 139, 141, 143, 145, 147, 
    149, 151, 153, 154, 156, 158, 160, 162, 164, 166, 168, 170, 172, 174, 176, 178, 
    180, 182, 183, 185, 187, 189, 191, 193, 195, 196, 198, 200, 202, 204, 205, 207, 
    209, 210, 212, 214, 215, 217, 219, 220, 222, 223, 225, 226, 228, 229, 230, 232, 
    233, 234, 235, 237, 238, 239, 240, 241, 242, 243, 244, 245, 246, 247, 247, 248, 
    249, 250, 250, 251, 251, 252, 252, 253, 253, 253, 254, 254, 254, 254, 254, 254
};

/*
// static const uint8_t breathing_effect_LUT_2[] =
// {
//     254, 253, 253, 253, 253, 253, 253, 253, 253, 253, 253, 253, 253, 253, 253, 253, 
//     253, 253, 253, 252, 252, 252, 252, 252, 252, 252, 251, 251, 251, 251, 251, 251, 
//     250, 250, 250, 250, 249, 249, 249, 249, 248, 248, 248, 248, 247, 247, 247, 246, 
//     246, 246, 246, 245, 245, 244, 244, 244, 243, 243, 243, 242, 242, 241, 241, 240, 
//     240, 239, 239, 238, 238, 237, 237, 236, 236, 235, 235, 234, 234, 233, 232, 232, 
//     231, 231, 230, 229, 229, 228, 227, 226, 226, 225, 224, 223, 223, 222, 221, 220, 
//     219, 219, 218, 217, 216, 215, 214, 213, 212, 211, 210, 209, 208, 207, 206, 205, 
//     204, 203, 202, 201, 200, 199, 197, 196, 195, 194, 193, 191, 190, 189, 188, 186, 
//     185, 184, 182, 181, 179, 178, 177, 175, 174, 172, 171, 169, 168, 166, 165, 163, 
//     162, 160, 158, 157, 155, 153, 152, 150, 148, 147, 145, 143, 141, 140, 138, 136, 
//     134, 133, 131, 129, 127, 125, 123, 121, 120, 118, 116, 114, 112, 110, 108, 106, 
//     104, 102, 100, 99, 97, 95, 93, 91, 89, 87, 85, 83, 81, 79, 77, 75, 
//     73, 71, 70, 68, 66, 64, 62, 60, 58, 57, 55, 53, 51, 49, 48, 46, 
//     44, 43, 41, 39, 38, 36, 34, 33, 31, 30, 28, 27, 25, 24, 23, 21, 
//     20, 19, 18, 16, 15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 6, 5, 
//     4, 3, 3, 2, 2, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0
// };
*/


/**
 * @brief Convert HSV colorspace to RGB colorspace
 * @param hsv_in pointer to input hsv color
 * @param rgb_out pointer to output rgb color
 */
void hsv_to_rgb(const CHSV_t *hsv_in, CRGB_t *rgb_out);

/**
 * @brief Create moving rainbow effect and fill the led array
 * @param led_array pointer to the led array to be filled
 * @param led_num number of leds (array element count)
 * @param hue_settings pointer to hue settings struct
 * @param brightness brightness value (0 to 1)
 */
void rainbow_color_effect(CRGB_t *led_array, uint8_t led_num, hue_settings_t *hue_settings, float brightness);

/**
 * @brief Create breathing effect using a lookup table
 * TODO: Use a better LUT for smoother breathing
 * @param led_array pointer to the led array to be filled
 * @param led_num number of leds (array element count)
 * @param breath_delay speed of the breathing. Keep it from 1 to 10 for better result.
 * @param ms_tick tick in ms
 */
void breathing_effect(CRGB_t *led_array, uint8_t led_num, uint8_t breath_delay, uint64_t ms_tick);