#include "voices.hpp"

namespace MultiV
{
    const uint8_t sinewave_lu[] = { 21, 29, 35, 40, 42, 40, 35, 29, 21, 12, 6, 1, 0, 1, 6, 12 };
    const uint8_t square25_lu[] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 42, 42, 42, 42 };
    const uint8_t square50_lu[] = { 0, 0, 0, 0, 0, 0, 0, 0, 42, 42, 42, 42, 42, 42, 42, 42 };
    const uint8_t triangle_lu[] = { 0, 5, 10, 15, 21, 26, 31, 36, 42, 36, 31, 26, 21, 15, 10, 5 };
    const uint8_t sawtooth_lu[] = { 0, 2, 5, 8, 11, 14, 16, 19, 22, 25, 28, 30, 33, 36, 39, 42 };
    const uint8_t semisine_lu[] = { 0, 8, 16, 23, 29, 34, 38, 41, 42, 41, 38, 34, 29, 23, 16, 8 };

    const uint8_t* lu_tables[] = {
        sinewave_lu,
        square25_lu,
        square50_lu,
        triangle_lu,
        sawtooth_lu,
        semisine_lu,
    };

    const uint16_t tuning_words [] = { 275, 291, 308, 327, 346, 367, 388, 412, 436, 462, 489 };

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

    void init_voice(Voice& v, int16_t note, Waveform style)
    {
        v.phase_acc = INDEX_SIZE;
        v.phase_idx = 0;
        v.note = (uint8_t)(note % 12);
        v.octave = (uint8_t)(note / 12);
        v.style = style;
        v.on = true;
    }

    void set_voice(Voice& v, int16_t note)
    {
        v.phase_idx = 0;
        if (note >= 0)
        {
            v.note = (uint8_t)(note % 12);
            v.octave = (uint8_t)(note / 12);
            v.on = true;
        }
        else
        {
            v.on = false;
        }
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