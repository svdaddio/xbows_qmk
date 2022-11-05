/* Copyright 2021 Shulin Huang <mumu@x-bows.com>
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
#include QMK_KEYBOARD_H
#define _BASE 0
#define _FUNC 1
#define _NAV 2

typedef enum {
    TD_NONE,
    TD_UNKNOWN,
    TD_SINGLE_TAP,
    TD_SINGLE_HOLD,
    TD_DOUBLE_TAP,
    TD_DOUBLE_HOLD,
    TD_DOUBLE_SINGLE_TAP, // Send two single taps
    TD_TRIPLE_TAP,
    TD_TRIPLE_HOLD
} td_state_t;

typedef struct {
    bool is_press_action;
    td_state_t state;
} td_tap_t;

enum {
    TD_LAY_CAPS,
    TD_SPC,
    TD_HM,
    TD_CMD,
};

enum combos {
  LK_CMB,
};

bool get_permissive_hold(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
        case TD(TD_SPC):
            return true;
        case TD(TD_CMD):
            return true;
        case CTL_T(KC_BSPC):
            return true;
        default: 
            return false;
    }
}

const uint16_t PROGMEM lock_combo[] = {KC_BSPC, KC_END, COMBO_END};

td_state_t cur_dance(qk_tap_dance_state_t *state);

// For the x tap dance. Put it here so it can be used in any keymap
void spc_finished(qk_tap_dance_state_t *state, void *user_data);
void spc_reset(qk_tap_dance_state_t *state, void *user_data);

td_state_t cur_dance(qk_tap_dance_state_t *state) {
    if (state->count == 1) {
        if (state->interrupted || !state->pressed) return TD_SINGLE_TAP;
        // Key has not been interrupted, but the key is still held. Means you want to send a 'HOLD'.
        else return TD_SINGLE_HOLD;
    } else if (state->count == 2) {
        return TD_DOUBLE_TAP;
    } else return TD_SINGLE_TAP;
};

// Create an instance of 'td_tap_t' for the 'x' tap dance.
static td_tap_t spctap_state = {
    .is_press_action = true,
    .state = TD_NONE
};

void spc_finished(qk_tap_dance_state_t *state, void *user_data) {
    spctap_state.state = cur_dance(state);
    switch (spctap_state.state) {
        case TD_SINGLE_TAP: register_code(KC_SPC); break;
        case TD_SINGLE_HOLD: layer_on(2); break;
        case TD_DOUBLE_TAP: register_code(KC_ENT); break;
        default: break;
    }
};

void spc_reset(qk_tap_dance_state_t *state, void *user_data) {
    switch (spctap_state.state) {
        case TD_SINGLE_TAP: unregister_code(KC_SPC); break;
        case TD_SINGLE_HOLD: layer_off(2); break;
        case TD_DOUBLE_TAP: unregister_code(KC_ENT); break;
        default: break;
    }
    spctap_state.state = TD_NONE;
};

void home_tap(qk_tap_dance_state_t *state, void *user_data) {
    if (state->count >= 2) {
        layer_move(0);
        reset_tap_dance(state);
    } else
    tap_code(KC_HOME);
};

void cmd_tap(qk_tap_dance_state_t *state, void *user_data) {
    clear_oneshot_mods();
    if (state->count >= 2) {
        if (state->interrupted || state->pressed) {
            add_oneshot_mods((MOD_BIT(KC_LEFT_CTRL) | MOD_BIT(KC_LEFT_ALT) | MOD_BIT(KC_LEFT_GUI)));
        } else {
            register_code(KC_LGUI);
            tap_code(KC_SPC);
            unregister_code(KC_LGUI);
        }
    } else if (state->interrupted || state->pressed) {
        register_mods(MOD_BIT(KC_LEFT_GUI));
    } else 
        add_oneshot_mods(MOD_BIT(KC_LEFT_GUI));
};

void cmd_reset(qk_tap_dance_state_t *state, void *user_data) {
    if (get_mods() & MOD_BIT(KC_LEFT_GUI)) {
        unregister_mods(MOD_BIT(KC_LEFT_GUI));
    }
};

qk_tap_dance_action_t tap_dance_actions[] = {
    // Tap once for Escape, twice for Caps Lock
    [TD_LAY_CAPS] = ACTION_TAP_DANCE_LAYER_TOGGLE(KC_CAPS, 2),
    [TD_SPC] = ACTION_TAP_DANCE_FN_ADVANCED(NULL, spc_finished, spc_reset),
    [TD_HM] = ACTION_TAP_DANCE_FN(home_tap),
    [TD_CMD] = ACTION_TAP_DANCE_FN_ADVANCED(NULL, cmd_tap, cmd_reset)
};
combo_t key_combos[COMBO_COUNT] = {
    [LK_CMB] = COMBO(lock_combo, LCTL(LGUI(KC_Q))),
};

uint16_t get_autoshift_timeout(uint16_t keycode, keyrecord_t *record) {
    switch(keycode) {
        case KC_A || KC_Q || KC_Z || KC_X || KC_C || KC_T || KC_G || KC_Y || KC_H|| KC_N || KC_M:
            return get_generic_autoshift_timeout() + 65;
    }
    switch(keycode) {
        case AUTO_SHIFT_NUMERIC:
            return get_generic_autoshift_timeout() + 50;
        case AUTO_SHIFT_SPECIAL:
            return get_generic_autoshift_timeout() + 50;
        case AUTO_SHIFT_ALPHA:
        default:
            return get_generic_autoshift_timeout();
    }
};
uint16_t get_tapping_term(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
        case TD(TD_SPC):
            return TAPPING_TERM + 35;
        case TD(TD_LAY_CAPS):
            return TAPPING_TERM + 35;
        default:
            return TAPPING_TERM;
    }
};
layer_state_t layer_state_set_user(layer_state_t state) {
    switch (get_highest_layer(state)) {
    case _BASE:
        rgb_matrix_sethsv(HSV_RED);
        break;
    case _FUNC:
        rgb_matrix_sethsv(HSV_GOLD);
        break;
    case _NAV:
        rgb_matrix_sethsv(HSV_BLUE);
        break;
    default: //  for any other layers, or the default layer
        rgb_matrix_sethsv(HSV_RED);
        break;
    }
  return state;
};
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
  /* Keymap VANILLA: (Base Layer) Default Layer
   *
   * |---------------------------------------------------------------------------------------------------------------------------------|
   * | Esc |  F1  |  F2  |  F3  |  F4  |      F5  |  F6  |  F7  |  F8  |      F9  |  F10 |  F11 |  F12 |   Home   |    End    |
   * |---------------------------------------------------------------------------------------------------------------------------------|
   * |  ~  |     1   |   2   |   3   |   4   |    5      |       6    |    7    |    8   |   9  |   0  |   -  |  =  |  Delete  |
   * |---------------------------------------------------------------------------------------------------------------------------------|
   * | Tab |   Q    |    W   |   E  |   R  |   T  |            |    Y   |    U   |    I  |   O  |   P  |   [  |  ]  |   \  | PgUp |
   * |---------------------------------------------------------------------------------------------------------------------------------|
   * | Ctl |   A   |   S   |   D  |   F  |   G  |      Bksp      |    H  |    J   |   K  |   L  |   ;  |  '"  |    Enter   | PgDn |
   * |---------------------------------------------------------------------------------------------------------------------------------|
   * |Shift|   Z  |   X  |   C  |   V  |   B  |       Enter       |    N  |    M   |  ,  |   .  |  /?  | Shift|      |  Up |
   * |---------------------------------------------------------------------------------------------------------------------------------|
   * |Ctrl | Option |     GUI   |    Space   |   Ctrl   |   Shift   |     Space     |    GUI   |  FN  | Ctrl | Lft  |  Dn |  Rig |
   * |---------------------------------------------------------------------------------------------------------------------------------|
   */
  [_BASE] = LAYOUT(
		KC_ESC,  KC_F1,   KC_F2, KC_F3,  LSG(KC_4),  MEH(KC_F5),   KC_F6,   KC_F7,  KC_F8,  KC_F9,   KC_F10, KC_F11,  KC_F12,  TD(TD_HM),  KC_END,
		KC_GRV,  KC_1,    KC_2,  KC_3,   KC_4,   KC_5,             KC_6,   KC_7,   KC_8,    KC_9,   KC_0,    KC_MINS, KC_EQL,  KC_BSPC,
		KC_TAB,  KC_Q,    KC_W,  KC_E,   KC_R,   KC_T,    KC_Y,    KC_U,   KC_I,   KC_O,    KC_P,   KC_LBRC, KC_RBRC, KC_BSLS, KC_PGUP,
		TD(TD_LAY_CAPS), KC_A,    KC_S,  KC_D,   KC_F,   KC_G,    KC_BSPC, KC_H,   KC_J,   KC_K,    KC_L,   KC_SCLN, KC_QUOT, KC_ENT,  KC_PGDN,
		OSM(MOD_LSFT), KC_Z,    KC_X,  KC_C,   KC_V,   KC_B,    KC_ENT,  KC_N,   KC_M,   KC_COMM, KC_DOT, KC_SLSH, OSM(MOD_RSFT), KC_UP,
		KC_LCTL, KC_LALT, TD(TD_CMD),       TD(TD_SPC), CTL_T(KC_BSPC), OSM(MOD_LSFT), KC_SPC,        KC_RGUI, MO(1),  KC_RCTL, KC_LEFT, KC_DOWN, KC_RGHT),
  [_FUNC] = LAYOUT(
    QK_BOOT,     KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,   KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_CALC,   KC_MYCM,  KC_MSEL,   KC_MAIL,   NK_TOGG,   EEP_RST,
    KC_TRNS,   KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,             KC_TRNS,  KC_TRNS,  KC_TRNS,   KC_TRNS,  KC_TRNS,   KC_TRNS,   KC_TRNS,   KC_NLCK,
    RGB_TOG,   RGB_MOD,  RGB_VAI,  RGB_HUI,  KC_TRNS,  KC_TRNS,   KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,   KC_TRNS,  KC_TRNS,   KC_TRNS,   KC_TRNS,   KC_HOME,
    KC_TRNS,   RGB_SPD,  RGB_VAD,  RGB_SPI,  KC_TRNS,  KC_TRNS,   KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,   KC_TRNS,  KC_TRNS,   KC_TRNS,   KC_TRNS,   KC_END,
    KC_TRNS,   KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,   KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,   KC_TRNS,  KC_TRNS,   KC_MUTE,   KC_VOLU,
    KC_TRNS,   KC_TRNS,  KC_TRNS,            KC_TRNS,  KC_TRNS,   KC_TRNS,  KC_TRNS,            KC_TRNS,   KC_TRNS,  KC_MPLY,   KC_MPRV,   KC_VOLD,   KC_MNXT),
  [_NAV] = LAYOUT(
    KC_TRNS,   KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,   KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,   KC_TRNS,  KC_TRNS,   KC_TRNS,   KC_TRNS,   KC_TRNS,
    KC_TRNS,   KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,             KC_TRNS,  KC_TRNS,  KC_TRNS,   KC_TRNS,  KC_TRNS,   KC_TRNS,   KC_TRNS,   KC_TRNS,
    KC_TRNS,   KC_1,  KC_2,  KC_3,  KC_4,  KC_5,   KC_6,  KC_7,  KC_8,  KC_9,   KC_0,  KC_TRNS,   KC_TRNS,   KC_TRNS,   KC_TRNS,
    KC_ESC,   LSG(KC_LEFT),  LSG(KC_RIGHT),  LSA(KC_LEFT),  LSA(KC_RIGHT),  KC_TRNS,   KC_DEL,  KC_LEFT,  KC_UP,  KC_DOWN,   KC_RIGHT,  KC_TRNS,   KC_TRNS,   KC_TRNS,   KC_TRNS,
    KC_TRNS,   LGUI(KC_Z),  LGUI(KC_X),  LGUI(KC_C),  LGUI(KC_V),  KC_TRNS,   KC_TRNS,  LGUI(KC_N),  LALT(KC_LEFT),   LALT(KC_RIGHT),  LGUI(KC_LEFT),   LGUI(KC_RIGHT),  KC_TRNS,   KC_TRNS,
    KC_TRNS,   KC_TRNS,  KC_TRNS,            KC_TRNS,  KC_TRNS,   KC_TRNS,  KC_TRNS,            KC_TRNS,   KC_TRNS,  KC_TRNS,   KC_TRNS,   KC_TRNS,   KC_TRNS)
};