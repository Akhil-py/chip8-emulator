#ifndef WINDOW_H
#define WINDOW_H

#include <SDL2/SDL.h>
#include <cstddef>
#include <cstdint>

/**
 * A class to handle I/O using the SDL library
 */
class Window {
public:
    SDL_Window* window;
    SDL_Renderer* renderer;
    SDL_Texture* texture;
    SDL_AudioDeviceID audioDevice;
    int WIDTH;
    int HEIGHT;
    int SCALE;

    /**
     * Constructor for the Window class
     * @param width The width of the window in pixels
     * @param height The height of the window in pixels
     * @param scale The scale factor for the window
     */
    Window(const int width, const int height, const int scale);

    /**
     * Destructor for the Window class
     */
    ~Window();

    /**
     * Update the display with pixel data from a buffer
     * @param buffer Pointer to the pixel data buffer
     */
    void update(void const* buffer);

    /**
     * Start playing a beep sound
     */
    void startBeep();

    /**
     * Stop playing the beep sound
     */
    void stopBeep();

    /**
     * Process input from the keypad
     * @param keys Pointer to an array representing key states
     * @return True if a quit event occurred, false otherwise
     */
    bool processInput(size_t* keys);

    /**
     * Audio callback function for generating square waves
     * @param userdata User-defined data (unused)
     * @param stream Pointer to the audio stream buffer
     * @param len Length of the audio stream buffer
     */
    static void audioCallback(void* userdata, Uint8* stream, int len);
};

#endif