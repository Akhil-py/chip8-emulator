#include <SDL2/SDL.h>
#include <iostream>

/**
 * A class to handle I/O using the SDL library
 */
class Window {
public:
	SDL_Window* window;
	SDL_Renderer* renderer;
	SDL_Texture* texture;
	int WIDTH;
	int HEIGHT;
	int SCALE;

	Window(const int width, const int height, const int scale) {
		WIDTH = width;
		HEIGHT = height;
		SCALE = scale;

		SDL_Init(SDL_INIT_VIDEO);
		
		window = SDL_CreateWindow("Chip 8", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WIDTH*SCALE, HEIGHT*SCALE, 0);

		renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

		texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STREAMING, WIDTH, HEIGHT);
	}

	~Window() {
		SDL_DestroyTexture(texture);
		SDL_DestroyRenderer(renderer);
		SDL_DestroyWindow(window);
		SDL_Quit();
	}

	/**
	 * Used to update the display with pixel data from buffer
	 */
	void update(void const* buffer) {
		SDL_UpdateTexture(texture, NULL, buffer, WIDTH*sizeof(uint32_t)); 

		SDL_Rect destRect = {0, 0, WIDTH*SCALE, HEIGHT*SCALE}; // Scaling

		SDL_RenderClear(renderer);
		SDL_RenderCopy(renderer, texture, NULL, &destRect);
		SDL_RenderPresent(renderer);
	}

	/**
	 * Keypad functionality
	 */
	bool processInput(size_t* keys) {
		SDL_Event event;
		bool quit = false;

		while (SDL_PollEvent(&event)) {
			if (event.type == SDL_QUIT || event.key.keysym.sym == SDLK_ESCAPE) {
				quit = true;
				break;
			}

			if (event.type == SDL_KEYDOWN) {
				switch (event.key.keysym.sym) {
					case SDLK_x:
						keys[0] = 1;
						break;
					case SDLK_1:
						keys[1] = 1;
						break;
					case SDLK_2:
						keys[2] = 1;
						break;
					case SDLK_3:
						keys[3] = 1;
						break;
					case SDLK_q:
						keys[4] = 1;
						break;
					case SDLK_w:
						keys[5] = 1;
						break;
					case SDLK_e:
						keys[6] = 1;
						break;
					case SDLK_a:
						keys[7] = 1;
						break;
					case SDLK_s:
						keys[8] = 1;
						break;
					case SDLK_d:
						keys[9] = 1;
						break;
					case SDLK_z:
						keys[0xA] = 1;
						break;
					case SDLK_c:
						keys[0xB] = 1;
						break;
					case SDLK_4:
						keys[0xC] = 1;
						break;
					case SDLK_r:
						keys[0xD] = 1;
						break;
					case SDLK_f:
						keys[0xE] = 1;
						break;
					case SDLK_v:
						keys[0xF] = 1;
						break;
				}
			}
			if (event.type == SDL_KEYUP) {
				switch (event.key.keysym.sym) {
					case SDLK_x:
						keys[0] = 0;
						break;
					case SDLK_1:
						keys[1] = 0;
						break;
					case SDLK_2:
						keys[2] = 0;
						break;
					case SDLK_3:
						keys[3] = 0;
						break;
					case SDLK_q:
						keys[4] = 0;
						break;
					case SDLK_w:
						keys[5] = 0;
						break;
					case SDLK_e:
						keys[6] = 0;
						break;
					case SDLK_a:
						keys[7] = 0;
						break;
					case SDLK_s:
						keys[8] = 0;
						break;
					case SDLK_d:
						keys[9] = 0;
						break;
					case SDLK_z:
						keys[0xA] = 0;
						break;
					case SDLK_c:
						keys[0xB] = 0;
						break;
					case SDLK_4:
						keys[0xC] = 0;
						break;
					case SDLK_r:
						keys[0xD] = 0;
						break;
					case SDLK_f:
						keys[0xE] = 0;
						break;
					case SDLK_v:
						keys[0xF] = 0;
						break;
				}
			}
		}

		return quit;
	}
};


/**
 * The main function is only used to test the SDL library and the window class functions
 */
int main(int argc, char *argv[])
{
	const int WIDTH = 64;
	const int HEIGHT = 32;
	const int SCALE = 25;
	uint32_t display[64 * 32]{0xFF0000FF};

	Window window = Window(WIDTH, HEIGHT, SCALE);

	display[5 * WIDTH + 10] = 0xFFFFFFFF; // Pixel at (10, 5)
    display[15 * WIDTH + 20] = 0xFFFFFFFF; // Pixel at (20, 15)
	display[31 * WIDTH + 63] = 0xFFFFFFFF; // Pixel at (63, 31)

	window.update(display);

	SDL_Event windowEvent;

	while (true) {
		if (SDL_PollEvent(&windowEvent)) {
			if (windowEvent.type == SDL_QUIT) {
				break;
			}
		}
	}

	window.~Window();

	return 0;
}
