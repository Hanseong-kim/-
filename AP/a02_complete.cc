#include <SDL2/SDL.h>
#include <iostream>
#include <string>
#include <cmath>
#include <climits>
#include <cstdlib>
using namespace std;

// A Voice represents a single square wave oscillator with its parameters
struct Voice {
    double frequency = 0.0;      // Frequency in Hz
    double amplitude = 0.0;      // Amplitude from 0.0 (silent) to 1.0 (loudest)
    double lowpass_alpha = 0.0;  // Lowpass filter smoothing factor (0.0 to 1.0)
    double phase = 0.0;          // Current waveform phase (0.0 to 1.0)
    double prev_sample = 0.0;    // Previous filtered sample (for IIR filter)
};

// PlaybackData holds all voices used for audio synthesis
struct PlaybackData {
    Voice voices[4];  // Fixed array of 4 voices
};

// Clips a sample in the range [-1.0, 1.0] and converts it to 16-bit PCM
short clip_to_int16(double sample) {
    if (sample > 1.0) sample = 1.0;
    else if (sample < -1.0) sample = -1.0;
    return static_cast<short>(sample * 32767);
}

// SDL2 callback function that fills the audio buffer with synthesized samples
void play_audio(void* userdata, unsigned char* stream, int len) {
    PlaybackData* data = (PlaybackData*)userdata;
    short* buffer = (short*)stream;
    int num_samples = len / sizeof(short);

    for (int i = 0; i < num_samples; ++i) {
        double mixed = 0.0;  // Accumulated mixed sample from all voices

        for (int j = 0; j < 4; ++j) {
            Voice& v = data->voices[j];
            if (v.amplitude == 0.0 || v.frequency == 0.0) continue;

            double cycle_len = 44100.0 / v.frequency;  // Number of samples per cycle
            v.phase += 1.0 / cycle_len;
            if (v.phase >= 1.0) v.phase -= 1.0;

            // Generate raw square wave sample
            double raw_sample = (v.phase < 0.5 ? 1.0 : -1.0) * v.amplitude;

            // Apply IIR lowpass filter
            double filtered = v.lowpass_alpha * v.prev_sample + (1 - v.lowpass_alpha) * raw_sample;
            v.prev_sample = filtered;

            mixed += filtered;
        }

        buffer[i] = clip_to_int16(mixed);  // Store final clipped sample
    }
}

int main() {
    // Initialize SDL2 for audio
    if (SDL_Init(SDL_INIT_AUDIO) < 0) {
        cerr << "SDL_Init failed: " << SDL_GetError() << endl;
        return 1;
    }

    PlaybackData data;  // Initialize playback state

    SDL_AudioSpec spec;
    spec.freq = 44100;                   // Sample rate (CD quality)
    spec.format = AUDIO_S16SYS;         // 16-bit signed samples
    spec.channels = 1;                  // Mono output
    spec.samples = 2048;                // Buffer size
    spec.callback = play_audio;         // Callback function for audio fill
    spec.userdata = &data;              // Pointer to voice data

    // Open audio device
    if (SDL_OpenAudio(&spec, nullptr) < 0) {
        cerr << "SDL_OpenAudio failed: " << SDL_GetError() << endl;
        return 1;
    }

    SDL_PauseAudio(0);  // Start audio playback

    // Command parsing loop
    string cmd;
    while (cin >> cmd) {
        if (cmd == "freq") {
            int v; double f;
            cin >> v >> f;
            if (v >= 0 && v < 4) data.voices[v].frequency = f;
        } else if (cmd == "ampl") {
            int v; double a;
            cin >> v >> a;
            if (v >= 0 && v < 4) data.voices[v].amplitude = a;
        } else if (cmd == "lowpass") {
            int v; double a;
            cin >> v >> a;
            if (v >= 0 && v < 4) data.voices[v].lowpass_alpha = a;
        } else if (cmd == "delay") {
            int ms;
            cin >> ms;
            SDL_Delay(ms);  // Delay for the specified milliseconds
        } else if (cmd == "quit") {
            break;  // Exit the loop and terminate
        }
    }

    SDL_CloseAudio();  // Close audio device
    SDL_Quit();        // Clean up SDL
    return 0;
}