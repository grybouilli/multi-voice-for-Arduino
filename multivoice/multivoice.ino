#include "voices.hpp"

uint8_t overflow_counter = 0;

MultiV::Voice voices [MultiV::VOICES];

void setup()
{
    MultiV::init_multiv();

    init_voice(voices[0], 0, MultiV::SQUARE50);
    init_voice(voices[1], 3, MultiV::SQUARE50);
    init_voice(voices[2], 8, MultiV::SQUARE50);
    init_voice(voices[3], 12, MultiV::SQUARE50);
    init_voice(voices[4], 15, MultiV::SQUARE50);
}

ISR(TIMER1_OVF_vect)
{
    // We compute a new value every 4 overflows / synthesized sample
    // New output computed at a frequency of 16000000/127/4 = 31496.1 Hz 
    // Below that, the sound is not recognizable
    // Above that, we are not generating fast enough and the sound gets de-tuned (as in, the changes made get audible because they happen at a frequency < 20kHz).
    if (overflow_counter++ & (MultiV::SKIP - 1)){ return; }

    uint8_t out = 0;

    for(uint8_t voice = 0; voice < MultiV::VOICES; ++voice)
    {
        if(voices[voice].on) 
        {
        voices[voice].phase_acc -= MultiV::get_tuning_word(voices[voice]);
        if(voices[voice].phase_acc < 0)
        {
            voices[voice].phase_acc += MultiV::INDEX_SIZE;
            voices[voice].phase_idx++;
            voices[voice].phase_idx %= MultiV::LU_SIZE;
        }
        out += MultiV::get_voice_output(voices[voice]);
        }
    }

    OCR1AL = out;
}

void loop()
{
    
}