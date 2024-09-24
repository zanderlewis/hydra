#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

#define MAX_WINDOWS 1000
#define WINDOW_WIDTH 640
#define WINDOW_HEIGHT 480
#define MIN_WINDOW_WIDTH 100
#define MAX_WINDOW_WIDTH 800
#define MIN_WINDOW_HEIGHT 100
#define MAX_WINDOW_HEIGHT 600
#define AUDIO_FREQUENCY 44100
#define AUDIO_FORMAT AUDIO_S16SYS
#define AUDIO_CHANNELS 2
#define AUDIO_SAMPLES 4096
#define BEEPS_PER_WINDOW 5

typedef enum {
    BEEP_TYPE_1,
    BEEP_TYPE_2,
    BEEP_TYPE_3,
    BEEP_TYPE_4,
    BEEP_TYPE_5,
    BEEP_TYPE_6,
    BEEP_TYPE_7,
    BEEP_TYPE_COUNT
} BeepType;

void generate_beep(BeepType type, Uint8 *stream, int len, int *phase, float volume) {
    int frequency;
    switch (type) {
        case BEEP_TYPE_1:
            frequency = 440; // A4
            break;
        case BEEP_TYPE_2:
            frequency = 523; // C5
            break;
        case BEEP_TYPE_3:
            frequency = 587; // D5
            break;
        case BEEP_TYPE_4:
            frequency = 659; // E5
            break;
        case BEEP_TYPE_5:
            frequency = 784; // G5
            break;
        case BEEP_TYPE_6:
            frequency = 880; // A5
            break;
        case BEEP_TYPE_7:
            frequency = 987; // B5
            break;
        default:
            frequency = 440;
            break;
    }

    for (int i = 0; i < len; i += 2) {
        int16_t sample = (int16_t)(32767 * volume * sin(2.0 * M_PI * frequency * (*phase) / AUDIO_FREQUENCY));
        stream[i] = sample & 0xFF;
        stream[i + 1] = (sample >> 8) & 0xFF;
        (*phase)++;
    }
}

void audio_callback(void *userdata, Uint8 *stream, int len) {
    static int phase = 0;

    for (int i = 0; i < BEEPS_PER_WINDOW; i++) {
        BeepType type = rand() % BEEP_TYPE_COUNT;
        int random_volume = rand() % 91 + 10; // Generate a random volume between 10 and 100
        float volume = random_volume / 100.0f; // Normalize to a value between 0.1 and 1.0
        generate_beep(type, stream, len / BEEPS_PER_WINDOW, &phase, volume);
        stream += len / BEEPS_PER_WINDOW;
    }
}

int main() {
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) != 0) {
        fprintf(stderr, "SDL_Init Error: %s\n", SDL_GetError());
        return 1;
    }

    SDL_Window *windows[MAX_WINDOWS];
    SDL_Renderer *renderers[MAX_WINDOWS];
    int window_count = 0;

    // Seed the random number generator
    srand(time(NULL));


    // Set up audio
    SDL_AudioSpec desired_spec;
    SDL_AudioSpec obtained_spec;
    desired_spec.freq = AUDIO_FREQUENCY;
    desired_spec.format = AUDIO_FORMAT;
    desired_spec.channels = AUDIO_CHANNELS;
    desired_spec.samples = AUDIO_SAMPLES;
    desired_spec.callback = audio_callback;

    if (SDL_OpenAudio(&desired_spec, &obtained_spec) != 0) {
        fprintf(stderr, "SDL_OpenAudio Error: %s\n", SDL_GetError());
        SDL_DestroyRenderer(renderers[window_count - 1]);
        SDL_DestroyWindow(windows[window_count - 1]);
        SDL_Quit();
        return 1;
    }

    // Create the initial window
    windows[window_count] = SDL_CreateWindow("YOU HAVE A VIRUS", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN | SDL_WINDOW_ALWAYS_ON_TOP);
    if (windows[window_count] == NULL) {
        fprintf(stderr, "SDL_CreateWindow Error: %s\n", SDL_GetError());
        SDL_Quit();
        return 1;
    }
    SDL_PauseAudio(0); // Start playing audio

    renderers[window_count] = SDL_CreateRenderer(windows[window_count], -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (renderers[window_count] == NULL) {
        fprintf(stderr, "SDL_CreateRenderer Error: %s\n", SDL_GetError());
        SDL_DestroyWindow(windows[window_count]);
        SDL_Quit();
        return 1;
    }
    window_count++;

    SDL_Event event;

    while (1) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_CLOSE) {
                // Create two new windows when any window close event is detected
                for (int i = 0; i < 100; i++) {
                    if (window_count < MAX_WINDOWS) {
                        int window_width = rand() % (MAX_WINDOW_WIDTH - MIN_WINDOW_WIDTH + 1) + MIN_WINDOW_WIDTH;
                        int window_height = rand() % (MAX_WINDOW_HEIGHT - MIN_WINDOW_HEIGHT + 1) + MIN_WINDOW_HEIGHT;
                        int x = rand() % (1920 - window_width); // Assuming screen width is 1920
                        int y = rand() % (1080 - window_height); // Assuming screen height is 1080
                        windows[window_count] = SDL_CreateWindow("YOU HAVE A VIRUS", x, y, window_width, window_height, SDL_WINDOW_SHOWN | SDL_WINDOW_ALWAYS_ON_TOP);
                        SDL_PauseAudio(0); // Start playing audio
                        if (windows[window_count] == NULL) {
                            fprintf(stderr, "SDL_CreateWindow Error: %s\n", SDL_GetError());
                        } else {
                            renderers[window_count] = SDL_CreateRenderer(windows[window_count], -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
                            if (renderers[window_count] == NULL) {
                                fprintf(stderr, "SDL_CreateRenderer Error: %s\n", SDL_GetError());
                                SDL_DestroyWindow(windows[window_count]);
                            } else {
                                window_count++;
                            }
                        }
                    } else {
                        fprintf(stderr, "Maximum number of windows reached\n");
                    }
                }
            }
        }

        // Render random colored pixels in all windows
        for (int i = 0; i < window_count; i++) {
            SDL_SetRenderDrawColor(renderers[i], rand() % 256, rand() % 256, rand() % 256, 255);
            for (int j = 0; j < 1000; j++) { // Draw 1000 random pixels
                int x = rand() % WINDOW_WIDTH;
                int y = rand() % WINDOW_HEIGHT;
                SDL_RenderDrawPoint(renderers[i], x, y);
            }
            SDL_RenderPresent(renderers[i]);
        }
    }

    // Clean up and destroy all windows
    for (int i = 0; i < window_count; i++) {
        SDL_DestroyRenderer(renderers[i]);
        SDL_DestroyWindow(windows[i]);
    }
    SDL_CloseAudio();
    SDL_Quit();

    return 0;
}