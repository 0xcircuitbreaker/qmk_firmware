// Copyright 2023 Anjheos (@Anjheos)
// SPDX-License-Identifier: GPL-2.0-or-later


#include QMK_KEYBOARD_H

enum {
    TD_ALT_FN1,
};

typedef enum {
    TD_NONE,
    TD_SINGLE_TAP,
    TD_SINGLE_HOLD,
    TD_DOUBLE_TAP
} td_state_t;

typedef struct {
    bool is_press_action;
    td_state_t state;
} td_tap_t;

td_state_t cur_dance(tap_dance_state_t *state);
void alt_fn1_finished(tap_dance_state_t *state, void *user_data);
void alt_fn1_reset(tap_dance_state_t *state, void *user_data);

enum layer_names {
    _QWERTY,
    _FN1,
    _FN2,
    _FN3

};

static td_tap_t alt_fn1_tap_state = {
    .is_press_action = true,
    .state = TD_NONE
};

td_state_t cur_dance(tap_dance_state_t *state) {
    if (state->count == 1) {
        if (state->interrupted || !state->pressed) return TD_SINGLE_TAP;
        else return TD_SINGLE_HOLD;
    }
    if (state->count == 2) return TD_DOUBLE_TAP;
    return TD_NONE;
}

void alt_fn1_finished(tap_dance_state_t *state, void *user_data) {
    alt_fn1_tap_state.state = cur_dance(state);
    switch (alt_fn1_tap_state.state) {
        case TD_SINGLE_TAP:
            register_code(KC_LALT);
            break;
        case TD_SINGLE_HOLD:
            layer_on(_FN1);
            break;
        case TD_DOUBLE_TAP:
            register_code(KC_LGUI);
            break;
        case TD_NONE:
            break;
    }
}

void alt_fn1_reset(tap_dance_state_t *state, void *user_data) {
    switch (alt_fn1_tap_state.state) {
        case TD_SINGLE_TAP:
            unregister_code(KC_LALT);
            break;
        case TD_SINGLE_HOLD:
            layer_off(_FN1);
            break;
        case TD_DOUBLE_TAP:
            unregister_code(KC_LGUI);
            break;
        case TD_NONE:
            break;
    }
    alt_fn1_tap_state.state = TD_NONE;
}

tap_dance_action_t tap_dance_actions[] = {
    [TD_ALT_FN1] = ACTION_TAP_DANCE_FN_ADVANCED(NULL, alt_fn1_finished, alt_fn1_reset)
};

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
  [0] = LAYOUT_625(
    QK_GESC,   KC_Q,     KC_W,      KC_E,      KC_R,      KC_T,      KC_Y,      KC_U,      KC_I,      KC_O,      KC_P,      KC_BSPC,
    KC_TAB,    KC_A,     KC_S,      KC_D,      KC_F,      KC_G,      KC_H,      KC_J,      KC_K,      KC_L,      KC_SCLN,   KC_ENT,
    KC_LSFT,   KC_Z,     KC_X,      KC_C,      KC_V,      KC_B,      KC_N,      KC_M,      KC_COMMA,  KC_DOT,    KC_SLSH,   KC_UP,     UG_NEXT,
    KC_LCTL,   KC_LGUI,  TD(TD_ALT_FN1),                KC_SPC,                            MO(_FN3),             KC_LEFT,   KC_DOWN,   KC_RIGHT
  ),
  [1] = LAYOUT_625(
    KC_F1,     KC_F2,    KC_F3,     KC_F4,     KC_F5,     KC_F6,     KC_F7,     KC_F8,     KC_F9,     KC_F10,    KC_F11,    KC_F12,
    KC_CAPS,   KC_1,     KC_2,      KC_3,      KC_4,      KC_5,      KC_6,      KC_7,      KC_8,      KC_9,      KC_0,      KC_ENT,
    KC_SCRL,   KC_Z,     KC_X,      KC_C,      KC_V,      KC_B,      KC_N,      KC_M,      KC_COMMA,  KC_DOT,    CK_TOGG,   UG_SATU,   UG_TOGG,
    KC_NUM,    MO(_FN2), _______,                       KC_SPC,                            KC_RCTL,              UG_HUED,   UG_SATD,   UG_HUEU
  ),
  [2] = LAYOUT_625(
    // Row 0: Common symbols
    KC_EXLM,   KC_AT,    KC_HASH,   KC_DLR,    KC_PERC,   KC_CIRC,   KC_AMPR,   KC_ASTR,   KC_LPRN,   KC_RPRN,   KC_UNDS,   KC_MINS,
    // !         @         #          $          %          ^          &          *          (          )          _          -

    // Row 1: Brackets, quotes, and punctuation
    KC_LCBR,   KC_RCBR,  KC_PIPE,   KC_DQUO,   KC_COLN,   KC_SCLN,   KC_QUES,   KC_SLSH,   KC_LABK,   KC_RABK,   KC_COMM,   KC_DOT,
    // {         }         |          "          :          ;          ?          /          <          >          ,          .

    // Row 2: Additional symbols
    KC_MINS,   KC_PLUS,  KC_EQL,    KC_QUOT,   KC_GRV,    KC_TILD,   KC_BSLS,   _______,   _______,   _______,   _______,   _______,   _______,
    // -          +         =          '          `          ~      backslash

    _______,   _______,  _______,                         _______,                         _______,              _______,   _______,   _______
  ),
  [3] = LAYOUT_625(
    // Row 0: Common app shortcuts
    QK_GESC,   LGUI(KC_Q), LGUI(KC_W), LGUI(KC_E), LGUI(KC_R), LGUI(KC_T), LGUI(KC_Y), LGUI(KC_U), LGUI(KC_I), LGUI(KC_O), LGUI(KC_P), KC_BSPC,
    // Q: Quit app    W: Close window   E: Explorer   R: Refresh    T: New tab    Y: Redo      U: -          I: -         O: -        P: Print

    // Row 1: Text edit and navigation
    KC_TAB,    LGUI(KC_A), LGUI(KC_S), LGUI(KC_D), LGUI(KC_F), LGUI(KC_G), LGUI(KC_H), LGUI(KC_J), LGUI(KC_K), LGUI(KC_L), KC_SCLN,   KC_ENT,
    // A: Select all  S: Save        D: Duplicate   F: Find      G: Find next H: -         J: -          K: -         L: Lock

    // Row 2: Common
    KC_LSFT,   LGUI(KC_Z), LGUI(KC_X), LGUI(KC_C), LGUI(KC_V), LGUI(KC_B), LGUI(KC_N), LGUI(KC_M), LGUI(KC_COMM), LGUI(KC_DOT), KC_SLSH, KC_UP, KC_TRNS,
    // Z: Undo       X: Cut         C: Copy        V: Paste     B: Bold      N: New win   M: Minimize   ,: -           .: -
    _______,   _______,  _______,              _______,                                     _______,              _______,   _______,   _______
  )                   
};

#if defined(ENCODER_MAP_ENABLE)
const uint16_t PROGMEM encoder_map[][NUM_ENCODERS][NUM_DIRECTIONS] = {
    [_QWERTY] = { ENCODER_CCW_CW(KC_VOLD, KC_VOLU)},
    [_FN1] = { ENCODER_CCW_CW(UG_VALD, UG_VALU)},
    [_FN2] = { ENCODER_CCW_CW(KC_MS_WH_DOWN, KC_MS_WH_UP)},
    [_FN3] = { ENCODER_CCW_CW(KC_LEFT, KC_RIGHT)}
};
#endif

const rgblight_segment_t PROGMEM _caps_layer[] = RGBLIGHT_LAYER_SEGMENTS(
    {0, 3, HSV_CYAN}
);

const rgblight_segment_t PROGMEM _fn1_layer[] = RGBLIGHT_LAYER_SEGMENTS(
    {0,1, HSV_RED}
);

const rgblight_segment_t PROGMEM _fn2_layer[] = RGBLIGHT_LAYER_SEGMENTS(
    {1, 1, HSV_PURPLE}
);

const rgblight_segment_t PROGMEM _fn3_layer[] = RGBLIGHT_LAYER_SEGMENTS(
    {2, 1, HSV_YELLOW}
);

const rgblight_segment_t* const PROGMEM _rgb_layers[] = RGBLIGHT_LAYERS_LIST(
    _caps_layer,
    _fn1_layer,
    _fn2_layer,
    _fn3_layer
);

void keyboard_post_init_user (void) {
    rgblight_layers = _rgb_layers;
    rgblight_sethsv_noeeprom(HSV_BLACK);
}

bool led_update_user(led_t led_state) {
    rgblight_set_layer_state(0, led_state.caps_lock);
    return true;
}

layer_state_t layer_state_set_user(layer_state_t state) {
    rgblight_set_layer_state(1, layer_state_cmp(state, _FN1));
    rgblight_set_layer_state(2, layer_state_cmp(state, _FN2));
    rgblight_set_layer_state(3, layer_state_cmp(state, _FN3));
    return state;
}