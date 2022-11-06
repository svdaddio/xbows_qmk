/* Copyright 2020 Shulin Huang <mumu@x-bows.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#pragma once

#include "config_common.h"

#define MATRIX_ROWS 6
#define MATRIX_COLS 15
#define MATRIX_ROW_PINS { F7, F6, F5, F4, F1, F0 }
#define MATRIX_COL_PINS { B0, B1, B2, B3, B7, D2, D3, D5, D4, D6, D7, B4, B5, B6, C6 }
#define DIODE_DIRECTION COL2ROW
#define DEBOUNCE 3
#define TAPPING_TERM 200
#define TAPPING_TERM_PER_KEY
#define HOLD_ON_OTHER_KEY_PRESS_PER_KEY
#define IGNORE_MOD_TAP_INTERRUPT
#define DOUBLE_TAP_SHIFT_TURNS_ON_CAPS_WORD
#define AUTO_SHIFT_TIMEOUT 200
#define AUTO_SHIFT_TIMEOUT_PER_KEY
#define AUTO_SHIFT_NO_AUTO_REPEAT
#define COMBO_COUNT 1
#define ONESHOT_TIMEOUT 3500
#define LAYER_STATE_8BIT

#ifdef RGB_MATRIX_ENABLE
#    define RGB_MATRIX_LED_PROCESS_LIMIT 18
#    define RGB_MATRIX_LED_FLUSH_LIMIT 16

#    define RGB_DISABLE_WHEN_USB_SUSPENDED
#    define RGB_DISABLE_TIMEOUT 90000
#    define RGB_MATRIX_KEYPRESSES
#    define RGB_MATRIX_MAXIMUM_BRIGHTNESS 200
#    define RGB_MATRIX_CENTER \
        { 92, 33 }
// RGB Matrix Animation modes. Explicitly enabled
// For full list of effects, see:
// https://docs.qmk.fm/#/feature_rgb_matrix?id=rgb-matrix-effects
#    define RGB_MATRIX_STARTUP_MODE RGB_MATRIX_SOLID_REACTIVE
#    define RGB_MATRIX_STARTUP_VAL 175
#    define ENABLE_RGB_MATRIX_SOLID_REACTIVE          // Static single hue, pulses keys hit to shifted hue then fades to current hue
#    define DRIVER_ADDR_1 0b1110100
#    define DRIVER_ADDR_2 0b1110110
#    define DRIVER_ADDR_3 0b1110101
#    define DRIVER_COUNT 3
#    define DRIVER_1_LED_TOTAL 36
#    define DRIVER_2_LED_TOTAL 36
#    define DRIVER_3_LED_TOTAL 15
#    define DRIVER_LED_TOTAL (DRIVER_1_LED_TOTAL + DRIVER_2_LED_TOTAL + DRIVER_3_LED_TOTAL)
#endif
