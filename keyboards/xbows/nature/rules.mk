# MCU name
MCU = atmega32u4

# Bootloader selection
BOOTLOADER = atmel-dfu

# Build Options
#   change yes to no to disable
#
SPACE_CADET_ENABLE = no
GRAVE_ESC_ENABLE = no 
COMBO_ENABLE = yes          # Enqble combos
CAPS_WORD_ENABLE = yes      # Enable CapsWord
AUTO_SHIFT_ENABLE = yes     # Enable AutoShift
TAP_DANCE_ENABLE = yes      # Enable Tapdace
BOOTMAGIC_ENABLE = yes      # Enable Bootmagic Lite
MOUSEKEY_ENABLE = no       # Mouse keys
EXTRAKEY_ENABLE = yes       # Audio control and System control
CONSOLE_ENABLE = no         # Console for debug
COMMAND_ENABLE = no         # Commands for debug and configuration
NKRO_ENABLE = yes           # Enable N-Key Rollover
BACKLIGHT_ENABLE = no       # Enable keyboard backlight functionality
RGBLIGHT_ENABLE = no        # Enable keyboard RGB underglow
AUDIO_ENABLE = no           # Audio output
RGB_MATRIX_ENABLE = yes
RGB_MATRIX_DRIVER = IS31FL3731
