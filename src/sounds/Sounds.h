#pragma once

#include "../Arduboy2/Arduboy2.h"

const uint16_t introduction_sound[] PROGMEM = {
NOTE_A6,40, 0, 5, NOTE_C6,40,  0, 5,  NOTE_E6,40, 0, 5, NOTE_G6,40,  0, 5, 
TONES_END };

const uint16_t player_firing[] PROGMEM = {
NOTE_A4, 10, 0, 2, 
NOTE_C4, 6, 0, 2,
NOTE_B5, 7, 
TONES_END };

const uint16_t tank_firing[] PROGMEM = {
NOTE_A2, 10, 0, 2, 
NOTE_C2, 6, 0, 2,
NOTE_B3, 7, 
TONES_END };

const uint16_t miss[] PROGMEM = {
NOTE_A1, 10, 0, 2, 
NOTE_C1, 6, 
TONES_END };

const uint16_t safety[] PROGMEM = {
NOTE_A6, 10, 0, 2, 
NOTE_C6, 10, 0, 2, 
NOTE_A6, 10, 
TONES_END };

const uint16_t kill_hostage[] PROGMEM = {
NOTE_A7, 10, 0, 2, 
NOTE_B6, 10, 0, 2, 
NOTE_D6, 10, 
TONES_END };

const uint16_t collect_hostage[] PROGMEM = {
NOTE_A4, 8, 0, 2, 
NOTE_B3, 8, 0, 2, 
NOTE_D3, 8, 
TONES_END };

const uint16_t exploding[] PROGMEM = {
NOTE_A1, 10, 0, 2, 
NOTE_E3, 11, 0, 6, 
NOTE_C0, 7, 0, 3, 
NOTE_A3, 15, 0, 12, 
NOTE_B2, 9, 0, 7, 
NOTE_G2, 16, 0, 8, 
NOTE_F3, 5, 0, 8, 
NOTE_C5, 8, 0, 2, 
NOTE_F2, 13, 0, 5, 
NOTE_A0, 13, 0, 8, 
NOTE_A1, 15, 0, 2, 
NOTE_E3, 11, 0, 6, 
NOTE_C0, 7, 0, 3, 
NOTE_A3, 15, 0, 12, 
NOTE_B2, 9, 0, 7, 
NOTE_G2, 16, 0, 8, 
NOTE_F3, 5, 0, 8, 
NOTE_C2, 8, 0, 2, 
NOTE_F1, 13, 0, 5, 
NOTE_A0, 13, 0, 8, 
NOTE_C0, 6, 
TONES_END };