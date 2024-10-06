#include "voices.hpp"

namespace MultiV
{
const uint8_t sinewave_lu [] = { 12, 17, 21, 24, 25, 24, 21, 17, 12, 7, 3, 0, 0, 0, 3, 7 };
const uint8_t square25_lu [] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 25, 25, 25, 25 };
const uint8_t square50_lu [] = { 0, 0, 0, 0, 0, 0, 0, 0, 25, 25, 25, 25, 25, 25, 25, 25 };
const uint8_t triangle_lu [] = { 0, 3, 6, 9, 12, 15, 18, 21, 25, 21, 18, 15, 12, 9, 6, 3 };
const uint8_t sawtooth_lu [] = { 0, 1, 3, 5, 6, 8, 10, 11, 13, 15, 16, 18, 20, 21, 23, 25 };
const uint8_t semisine_lu [] = { 0, 4, 9, 13, 17, 20, 23, 24, 25, 24, 23, 20, 17, 13, 9, 4 };

const uint8_t* lu_tables[] = {
    sinewave_lu,
    square25_lu,
    square50_lu,
    triangle_lu,
    sawtooth_lu,
    semisine_lu,
};

const uint16_t tuning_words [] = { 462, 489, 518, 549, 582, 616, 653, 692, 733, 776, 823, 871 };

void init_multiv()
{
    DDRB |= (1 << PIN);
    // Mode 14 : Fast PWM, no prescaler, TOP value defined by ICR
    TCCR1A = 0x82;
    TCCR1B = 0x19;

    // Max value is 127
    ICR1H = 0;
    ICR1L = MAX_TOP;

    // Enable overflow interrupt
    TIMSK1 = 0x01;

    // Unused upper byte for compare match
    OCR1AH = 0;

    sei();
}

void init_voice(Voice& v, uint8_t note, Waveform style)
{
    v.phase_acc = INDEX_SIZE;
    v.phase_idx = 0;
    v.note = note % 12;
    v.octave = note / 12;
    v.style = style;
    v.on = true;
}

const uint8_t get_voice_output(const struct Voice& v)
{
    if (v.style < Waveform::TOTAL && v.phase_idx < LU_SIZE)
        return *(lu_tables[v.style] + v.phase_idx);
    return 0;
}

const uint16_t get_tuning_word(const struct Voice& v)
{
    if (v.note < NOTES) // there should be an extra check for the octave range
        return tuning_words[v.note] << v.octave;
    return 0;
}

}