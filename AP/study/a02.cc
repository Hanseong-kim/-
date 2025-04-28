#include <SDL2/SDL.h>
#include <cmath>
#include <iostream>
#include <string>
#include <climits>
using namespace std;

double const FREQ = 300;

static void play_audio(void* u, unsigned char* stream, int len){
    unsigned long long* position = (unsigned long long*)u;
    short* buffer = (short*)stream;
    len /= sizeof(short);
    // for (int i = 0; i < len; i+=64){
    //     for (int j = 0; j < 32; j++){
    //         buffer[i+j] = SHRT_MAX;
    //     }
    //     for (int j = 32; j < 64; j++){
    //         buffer[i+j] = SHRT_MIN;
    //     }
    // }
    for (int i = 0; i < len; i++, (*position)++){
        double cycle_length = 44100.0 / FREQ; // measured in samples
        double cycle_count;
        double phase = modf(*position / cycle_length, &cycle_count);
        if (phase < 0.5){
            buffer[i] = SHRT_MAX;
        } else {
            buffer[i] = SHRT_MIN;
        }
    }
    
    
}

int main(){
    if (SDL_Init(SDL_INIT_AUDIO) < 0){
        cerr << "SDL could not initialize SDL_error : " << SDL_GetError() << endl;
        return 1;
    }
    unsigned long long position = 0;
    SDL_AudioSpec audio_spec = {
        .freq = 44100, .format = AUDIO_S16SYS, .channels = 1,
        .samples = 2048, .callback = play_audio, .userdata = &position
    };
    
    if (SDL_OpenAudio(&audio_spec, nullptr) < 0){
        cerr << "SDL_OpenAudio failed: " << SDL_GetError();
        SDL_Quit();
        return 1;
    }
    SDL_PauseAudio(false);
    SDL_Delay(1000); // delay for 1 second
    SDL_CloseAudio();
    SDL_Quit();
    return 0;

}
