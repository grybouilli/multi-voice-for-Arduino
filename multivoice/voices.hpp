#pragma once

#include <Arduino.h>

namespace MultiV
{
const static uint8_t PIN = PB1;    // Can only be PB1 or PB2 (PIN 9 or 10). If PB2, adjustments need be done in the init_multiv function. Refer to the atmega328p doc.
const static uint8_t LU_SIZE = 16; // Look-up tables size
const static uint16_t INDEX_SIZE = 2047;
const static uint8_t NOTES = 12;
const static uint8_t MAX_TOP = 0x7f;
const static uint8_t VOICES = 5;
const static uint8_t SKIP = 4;

enum Waveform {
    SINEWAVE,
    SQUARE25,
    SQUARE50,
    TRIANGLE,
    SAWTOOTH,
    SEMISINE,
    TOTAL
};

struct Voice
{
    int16_t phase_acc;
    uint8_t phase_idx;
    uint8_t note;
    uint8_t octave;
    Waveform style;
    bool on;
};

void init_multiv();
void init_voice(Voice & v, uint8_t note, Waveform style);
const uint8_t get_voice_output(const struct Voice& v);
const uint16_t get_tuning_word(const struct Voice& v);

}
