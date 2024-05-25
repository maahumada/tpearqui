#ifndef KEYBOARD_DRIVER_H
#define KEYBOARD_DRIVER_H
#include <stdint.h>

uint64_t read(char* buffer, uint64_t count);
void getChar(uint8_t* character);

static uint8_t const spanish_keyboard_layout[][3] = {
    {1}, // Happy smile
    {0}, // Esc
    {'1', '!'},
    {'2', '"'},
    {'3', '#'},
    {'4', '$'},
    {'5', '%'},
    {'6', '&'},
    {'7', '/'},
    {'8', '('},
    {'9', ')'},
    {'0', '='},
    {'\'', '?', '\\'},
    {'¿', '¡'},
    {'\b', '\b', '\b'},
    {'\t', '\t', '\t'},
    {'q', 'Q', '@'},
    {'w', 'W'},
    {'e', 'E'},
    {'r', 'R'},
    {'t', 'T'},
    {'y', 'Y'},
    {'u', 'U'},
    {'i', 'I'},
    {'o', 'O'},
    {'p', 'P'},
    {0xB4, '¨'},
    {'+', '*', '~'},
    {'\n', '\n'},
    {0}, // LCtrl
    {'a', 'A'},
    {'s', 'S'},
    {'d', 'D'},
    {'f', 'F'},
    {'g', 'G'},
    {'h', 'H'},
    {'j', 'J'},
    {'k', 'K'},
    {'l', 'L'},
    {'ñ', 'Ñ'},
    {'{', '[', '^'},
    {0}, // IDK
    {0}, // LShift
    {'}', ']', '`'},
    {'z', 'Z'},
    {'x', 'X'},
    {'c', 'C'},
    {'v', 'V'},
    {'b', 'B'},
    {'n', 'N'},
    {'m', 'M'},
    {',', ';'},
    {'.', ':'},
    {'-', '_'},
    {0}, // RShift
    {0}, // IDK, keypad probably
    {0}, // Alt / AltGr if E0 before
    {' ', ' '},
    // CapsLock
    // F1..10 keys
    // NumLock
    // ScrollLock
    // Keypad 7..9
    // Keypad -
    // Keypad 4..6
    // Keypad +
    // Keypad 1..3
    // Keypad 0
    // Keypad .
    // 3 dead spaces
    // F11
    // F12
};

#endif