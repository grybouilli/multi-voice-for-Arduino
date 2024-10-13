#include "voices.hpp"
#include "notes.hpp"

#define NOTE_COUNT 120
const uint16_t tempo = 167; // one eigth is 167 ms
const uint16_t time_event = tempo/2; // one eigth is 167 ms

uint8_t overflow_counter = 0;

MultiV::Voice voices [MultiV::VOICES];

struct Note
{
    uint16_t tone;
    bool hold;
};

uint8_t event_idx = 0;
uint8_t melody_idx = 0;

// Super Mario Bros : Main Theme
// From the following sheet : https://musescore.com/user/2072681/scores/2601926
int16_t const melody[MultiV::VOICES][NOTE_COUNT][2]
{
    {
        QUAVER(E5), QUAVER(E5), QUAVER(SILENCE), QUAVER(E5), QUAVER(SILENCE), QUAVER(C5),  QUARTER(E5),     // 1
        QUARTER(G5), QUARTER(SILENCE), QUARTER(G4), QUARTER(SILENCE),
        QUARTER(C4), QUAVER(SILENCE), QUARTER(G4), QUAVER(SILENCE), QUARTER(E4), 
        QUAVER(SILENCE), QUARTER(A4), QUARTER(B4), QUAVER(As4), QUARTER(A4),
        QUAVER(G4), QUAVER(E5), QUARTER(G5), QUARTER(A5), QUAVER(F5), QUAVER(G5),
        QUAVER(SILENCE), QUARTER(E5), QUAVER(C5), QUAVER(D5), QUARTER(B4), QUAVER(SILENCE),
        QUARTER(SILENCE), QUAVER(G5), QUAVER(Fs5), QUAVER(F5), QUARTER(Ds5), QUAVER(E5),                    // 7
        QUAVER(SILENCE), QUAVER(Gs4), QUAVER(A4), QUAVER(C5), QUAVER(SILENCE), QUAVER(A4), QUAVER(C5), QUAVER(D5),
        QUARTER(SILENCE), QUAVER(G5), QUAVER(Fs5), QUAVER(F5), QUARTER(Ds5), QUAVER(E5),
        QUAVER(SILENCE), QUARTER(C6), QUAVER(C6), QUARTER(C6), QUARTER(SILENCE),
        QUARTER(SILENCE), QUAVER(G5), QUAVER(Fs5), QUAVER(F5), QUARTER(Ds5), QUAVER(E5),                    // 11
        QUAVER(SILENCE), QUAVER(Gs4), QUAVER(A4), QUAVER(C5), QUAVER(SILENCE), QUAVER(A4), QUAVER(C5), QUAVER(D5),
        QUARTER(SILENCE), QUARTER(Ds5), QUAVER(SILENCE), QUARTER(D5), QUAVER(SILENCE),
        QUARTER(C5), QUARTER(SILENCE), QUARTER(SILENCE), QUARTER(SILENCE),
        QUARTER(SILENCE), QUARTER(SILENCE), QUARTER(SILENCE), QUARTER(SILENCE)
    },
    {
        QUAVER(Fs4), QUAVER(Fs4), QUAVER(SILENCE), QUAVER(Fs4), QUAVER(SILENCE), QUAVER(Fs4), QUARTER(Fs4), // 1 
        QUARTER(B4), QUARTER(SILENCE), QUARTER(B4), QUARTER(SILENCE),
        QUARTER(E4), QUAVER(SILENCE), QUARTER(C4), QUAVER(SILENCE), QUARTER(G3), 
        QUAVER(SILENCE), QUARTER(C4), QUARTER(D4), QUAVER(Cs4), QUARTER(C4),
        QUAVER(C4), QUAVER(G4), QUARTER(B4), QUARTER(C5), QUAVER(A4), QUAVER(B4),
        QUAVER(SILENCE), QUARTER(A4), QUAVER(E4), QUAVER(F4), QUARTER(D4), QUAVER(SILENCE),
        QUARTER(SILENCE), QUAVER(E5), QUAVER(Ds5), QUAVER(D5), QUARTER(B4), QUAVER(C5),                     // 7
        QUAVER(SILENCE), QUAVER(E4), QUAVER(F4), QUAVER(G4), QUAVER(SILENCE), QUAVER(C4), QUAVER(E4), QUAVER(F4),
        QUARTER(SILENCE), QUAVER(E5), QUAVER(Ds5), QUAVER(D5), QUARTER(B4), QUAVER(C5),
        QUAVER(SILENCE), QUARTER(G5), QUAVER(G5), QUARTER(G5), QUARTER(SILENCE),
        QUARTER(SILENCE), QUAVER(E5), QUAVER(Ds5), QUAVER(D5), QUARTER(B4), QUAVER(C5),                     // 11
        QUAVER(SILENCE), QUAVER(E4), QUAVER(F4), QUAVER(G4), QUAVER(SILENCE), QUAVER(C4), QUAVER(E4), QUAVER(F4),
        QUARTER(SILENCE), QUARTER(Gs4), QUAVER(SILENCE), QUARTER(F4), QUAVER(SILENCE),
        QUARTER(E4), QUARTER(SILENCE), QUARTER(SILENCE), QUARTER(SILENCE),
        QUARTER(SILENCE), QUARTER(SILENCE), QUARTER(SILENCE), QUARTER(SILENCE)
    },
    
    {
        QUAVER(D3), QUAVER(D3), QUAVER(SILENCE), QUAVER(D3), QUAVER(SILENCE), QUAVER(D3), QUARTER(D3),  // 1
        QUARTER(G4), QUARTER(SILENCE), QUARTER(G3), QUARTER(SILENCE),
        QUARTER(G3), QUAVER(SILENCE), QUARTER(E3), QUAVER(SILENCE), QUARTER(C3), 
        QUAVER(SILENCE), QUARTER(F3), QUARTER(G3), QUAVER(Fs3), QUARTER(G3),
        QUAVER(E3), QUAVER(C4), QUARTER(E4), QUARTER(F4), QUAVER(D4), QUAVER(E4),
        QUAVER(SILENCE), QUARTER(C4), QUAVER(A3), QUAVER(B3), QUARTER(G3), QUAVER(SILENCE),
        QUARTER(C3), QUAVER(SILENCE), QUAVER(G3), QUARTER(SILENCE), QUARTER(C4),                        // 7
        QUARTER(F3), QUAVER(SILENCE), QUAVER(C4), QUARTER(C4), QUARTER(F3),
        QUARTER(C3), QUAVER(SILENCE), QUAVER(G3), QUARTER(SILENCE), QUAVER(G3), QUAVER(C4),
        QUAVER(SILENCE), QUARTER(F5), QUAVER(F5), QUARTER(F5), QUARTER(G3),
        QUARTER(C3), QUAVER(SILENCE), QUAVER(G3), QUARTER(SILENCE), QUARTER(C4),                        // 11
        QUARTER(F3), QUAVER(SILENCE), QUAVER(C4), QUARTER(C4), QUARTER(F3),
        QUARTER(C3), QUARTER(Gs3), QUAVER(SILENCE), QUARTER(As3), QUAVER(SILENCE),
        QUARTER(C4), QUAVER(SILENCE), QUAVER(G3), QUARTER(G3), QUARTER(C3),
        QUARTER(SILENCE), QUARTER(SILENCE), QUARTER(SILENCE), QUARTER(SILENCE)
    }
};

void setup()
{
    MultiV::init_multiv();

    MultiV::init_voice(voices[0], melody[0][0][0], MultiV::SQUARE50);
    MultiV::init_voice(voices[1], melody[1][0][0], MultiV::SQUARE25);
    MultiV::init_voice(voices[2], melody[2][0][0], MultiV::SAWTOOTH);
}

ISR(TIMER1_OVF_vect)
{
    // New output computed at a frequency of 16000000/127/4 = 31496.1 Hz 
    // We compute a new value every 4 overflows 
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
    if(event_idx == 2) 
    {
        melody_idx++;
        melody_idx %= NOTE_COUNT;
        event_idx = 0;
    } 

    for(uint8_t voice = 0; voice < MultiV::VOICES; ++voice)
    {
        if(!event_idx || event_idx && melody[voice][melody_idx][1])
            MultiV::set_voice(voices[voice], melody[voice][melody_idx][0]);
        else
            MultiV::set_voice(voices[voice], SILENCE);
    }

    delay(time_event);
    event_idx++;
}