/*=============================================================================
	launch.cpp: basic framework
	Copyright 2018 Dragonslake, LLC. All Rights Reserved.
=============================================================================*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <SDL.h>
#include <SDL_image.h>

#define DONT_MAKE_A_DLL
#include "Framework.h"


static SDL_Renderer *g_renderer;
static SDL_Window *window;

static int g_width = 800;
static int g_height = 600;
static int g_framework_initialized = false;

/*
 * structure declarations
 */

class Sprite {
public:
	Sprite():w(0), h(0), tex(nullptr) {}

	int w, h;
	SDL_Texture* tex;
};

FRAMEWORK_API Sprite* createSprite(const char* path)
{
	SDL_assert(g_framework_initialized);
	SDL_assert(g_renderer);

	SDL_Texture* texture = IMG_LoadTexture(g_renderer, path);
	if (!texture) {
		fprintf(stderr, "Couldn't load %s: %s\n", path, SDL_GetError());
		return nullptr;
	}


	Sprite* s = new Sprite();
	if (!s)
	{
		fprintf(stderr, "Not enough memory\n");
		SDL_Quit();
		exit(1);
	}

	SDL_QueryTexture(texture, NULL, NULL, &s->w, &s->h);

	s->tex = texture;

	return s;
}

FRAMEWORK_API void destroySprite(Sprite* s)
{
	SDL_assert(g_framework_initialized);
	SDL_assert(s);

	if (s->tex)
	{
		SDL_DestroyTexture(s->tex);
	}

	delete s;
}

FRAMEWORK_API void getSpriteSize(Sprite* s, int& w, int &h)
{
	SDL_assert(s);
	w = s->w;
	h = s->h;
}

FRAMEWORK_API void setWindowPos(int x, int y)
{
    SDL_assert(g_framework_initialized);
    SDL_assert(g_renderer);

    SDL_SetWindowPosition(window, x, y);
}

FRAMEWORK_API void drawSprite(Sprite* sprite, int x, int y)
{
	SDL_assert(g_framework_initialized);
	SDL_assert(g_renderer);
	SDL_assert(sprite);

	SDL_Rect r;
	r.w = sprite->w;
	r.h = sprite->h;
	r.x = x;
	r.y = y;
	SDL_RenderCopy(g_renderer, sprite->tex, NULL, &r);
}

FRAMEWORK_API void drawSpriteR(Sprite* sprite, int x, int y, float angle)
{
    SDL_assert(g_framework_initialized);
    SDL_assert(g_renderer);
    SDL_assert(sprite);

    SDL_Rect dest_rect;
    dest_rect.x = x;
    dest_rect.y = y;
    dest_rect.w = sprite->w;
    dest_rect.h = sprite->h;

    // Eeeew degrees instead of radians
    SDL_RenderCopyEx(g_renderer, sprite->tex, NULL,
                     &dest_rect, angle * 180.0 / M_PI, NULL, SDL_FLIP_NONE);
}

FRAMEWORK_API void drawSpriteRS(Sprite* sprite, int x, int y, float angle, float scale)
{
    SDL_assert(g_framework_initialized);
    SDL_assert(g_renderer);
    SDL_assert(sprite);

    // calculate new x and y
    int new_x = (x + sprite->w / 2) - ((sprite->w / 2) * scale);
    int new_y = (y + sprite->h / 2) - ((sprite->h / 2) * scale);
    // form new destination rect
    SDL_Rect dest_rect = {
            new_x,
            new_y,
            (int)(sprite->w * scale),
            (int)(sprite->h * scale)
    };

    SDL_RenderCopyEx(g_renderer, sprite->tex, NULL,
                     &dest_rect, angle * 180.0 / M_PI, NULL, SDL_FLIP_NONE);
}

FRAMEWORK_API void drawSpriteIntoArea(Sprite* sprite, int x, int y, int w, int h)
{
    SDL_assert(g_framework_initialized);
    SDL_assert(g_renderer);
    SDL_assert(sprite);

    SDL_Rect dest_rect;

    dest_rect.w = w;
    dest_rect.h = h;
    dest_rect.x = x;
    dest_rect.y = y;

    SDL_RenderCopy(g_renderer, sprite->tex, NULL, &dest_rect);
}

FRAMEWORK_API void getScreenSize(int& w, int &h)
{
	SDL_assert(g_framework_initialized);
	SDL_assert(g_renderer);

	SDL_Rect viewport;
	SDL_RenderGetViewport(g_renderer, &viewport);
	w = viewport.w;
	h = viewport.h;
}

FRAMEWORK_API unsigned int getTickCount()
{
	return SDL_GetTicks();
}


/* Draw a Gimpish background pattern to show transparency in the image */
static void draw_background(SDL_Renderer *renderer, int w, int h)
{
    SDL_Color col[2] = {
        { 0x66, 0x66, 0x66, 0xff },
        { 0x99, 0x99, 0x99, 0xff },
    };
    int i, x, y;
    SDL_Rect rect;

    rect.w = 8;
    rect.h = 8;
    for (y = 0; y < h; y += rect.h) {
        for (x = 0; x < w; x += rect.w) {
            /* use an 8x8 checkerboard pattern */
            i = (((x ^ y) >> 3) & 1);
            SDL_SetRenderDrawColor(renderer, col[i].r, col[i].g, col[i].b, col[i].a);

            rect.x = x;
            rect.y = y;
            SDL_RenderFillRect(renderer, &rect);
        }
    }
}


FRAMEWORK_API void drawTestBackground()
{
	SDL_assert(g_framework_initialized);
	SDL_assert(g_renderer);

	SDL_Rect viewport;
	SDL_RenderGetViewport(g_renderer, &viewport);
	return draw_background(g_renderer, viewport.w, viewport.h);
}

FRAMEWORK_API void showCursor(bool bShow)
{
	SDL_ShowCursor(bShow?1:0);
}

bool GKeyState[(int)FRKey::COUNT] = {};

static void sendKeyReleased(FRKey keyIndex, Framework* fw)
{
    if (GKeyState[(size_t)keyIndex])
    {
        fw->onKeyReleased(keyIndex);
        GKeyState[(size_t)keyIndex] = false;
    }
}

static void sendKeyPressed(FRKey keyIndex, Framework* fw)
{
    if (!GKeyState[(size_t)keyIndex])
    {
        fw->onKeyPressed(keyIndex);
        GKeyState[(size_t)keyIndex] = true;
    }
}


FRAMEWORK_API int run(Framework* framework)
{
    Uint32 flags;
    int done;
    SDL_Event event;

	for (int i = 0; i < (int)FRKey::COUNT; ++i)
	{
		GKeyState[i] = false;
	}

	Framework* GFramework = framework;

	bool fullscreen;
	GFramework->PreInit(g_width, g_height, fullscreen);

    flags = SDL_WINDOW_HIDDEN;
	if (fullscreen) {
		SDL_ShowCursor(0);
        flags |= SDL_WINDOW_FULLSCREEN;
    }

    if (SDL_Init(SDL_INIT_VIDEO) == -1) {
        fprintf(stderr, "SDL_Init(SDL_INIT_VIDEO) failed: %s\n", SDL_GetError());
        return(2);
    }

    if (SDL_CreateWindowAndRenderer(0, 0, flags, &window, &g_renderer) < 0) {
        fprintf(stderr, "SDL_CreateWindowAndRenderer() failed: %s\n", SDL_GetError());
        return(2);
    }

	{
        /* Show the window */
        SDL_SetWindowTitle(window, GFramework->GetTitle());
        SDL_SetWindowSize(window, g_width, g_height);
        SDL_ShowWindow(window);

		g_framework_initialized = true;


		if (!GFramework->Init())
		{
			fprintf(stderr, "Framework::Init failed\n");
			SDL_Quit();
			exit(1);
		}

        done = 0;
		int keyPressed = -1;
        while ( ! done ) {
            while ( SDL_PollEvent(&event) ) {
                switch (event.type) {
                    case SDL_KEYUP:
                        switch (event.key.keysym.sym) {
                            // Had to rewrite it 'cuz it didn't work and segfaulted
                            // once I expanded on the number of supported keys.
                            // Who thought that it's a good idea to just substract first keycode
                            // and expect it to work smooth I dunno
                            case SDLK_RIGHT:
                                sendKeyReleased(FRKey::RIGHT, GFramework);
                                break;
                            case SDLK_LEFT:
                                sendKeyReleased(FRKey::LEFT, GFramework);
                                break;
                            case SDLK_DOWN:
                                sendKeyReleased(FRKey::DOWN, GFramework);
                                break;
                            case SDLK_UP:
                                sendKeyReleased(FRKey::UP, GFramework);
                                break;
                            case SDLK_SPACE:
                                sendKeyReleased(FRKey::SPACE, GFramework);
                                break;
                            case SDLK_LCTRL:
                                sendKeyReleased(FRKey::LCTRL, GFramework);
                                break;
                            case SDLK_LSHIFT:
                                sendKeyReleased(FRKey::LSHIFT, GFramework);
                                break;
                            case SDLK_w:
                                sendKeyReleased(FRKey::W, GFramework);
                                break;
                            case SDLK_a:
                                sendKeyReleased(FRKey::A, GFramework);
                                break;
                            case SDLK_s:
                                sendKeyReleased(FRKey::S, GFramework);
                                break;
                            case SDLK_d:
                                sendKeyReleased(FRKey::D, GFramework);
                                break;
                            case SDLK_z:
                                sendKeyReleased(FRKey::Z, GFramework);
                                break;
                            case SDLK_x:
                                sendKeyReleased(FRKey::X, GFramework);
                                break;
                            case SDLK_c:
                                sendKeyReleased(FRKey::C, GFramework);
                                break;
                            case SDLK_v:
                                sendKeyReleased(FRKey::V, GFramework);
                                break;
                            case SDLK_q:
                                sendKeyReleased(FRKey::Q, GFramework);
                                break;
                            case SDLK_e:
                                sendKeyReleased(FRKey::E, GFramework);
                                break;
                            case SDLK_ESCAPE:
								done = 1;
                            break;
	                        default:
		                        break;
                        }
                        break;
					case SDL_KEYDOWN:
						switch (event.key.keysym.sym) {
                            case SDLK_RIGHT:
                                sendKeyPressed(FRKey::RIGHT, GFramework);
                                break;
                            case SDLK_LEFT:
                                sendKeyPressed(FRKey::LEFT, GFramework);
                                break;
                            case SDLK_DOWN:
                                sendKeyPressed(FRKey::DOWN, GFramework);
                                break;
                            case SDLK_UP:
                                sendKeyPressed(FRKey::UP, GFramework);
                                break;
                            case SDLK_SPACE:
                                sendKeyPressed(FRKey::SPACE, GFramework);
                                break;
                            case SDLK_LCTRL:
                                sendKeyPressed(FRKey::LCTRL, GFramework);
                                break;
                            case SDLK_LSHIFT:
                                sendKeyPressed(FRKey::LSHIFT, GFramework);
                                break;
                            case SDLK_w:
                                sendKeyPressed(FRKey::W, GFramework);
                                break;
                            case SDLK_a:
                                sendKeyPressed(FRKey::A, GFramework);
                                break;
                            case SDLK_s:
                                sendKeyPressed(FRKey::S, GFramework);
                                break;
                            case SDLK_d:
                                sendKeyPressed(FRKey::D, GFramework);
                                break;
                            case SDLK_z:
                                sendKeyPressed(FRKey::Z, GFramework);
                                break;
                            case SDLK_x:
                                sendKeyPressed(FRKey::X, GFramework);
                                break;
                            case SDLK_c:
                                sendKeyPressed(FRKey::C, GFramework);
                                break;
                            case SDLK_v:
                                sendKeyPressed(FRKey::V, GFramework);
                                break;
                            case SDLK_q:
                                sendKeyPressed(FRKey::Q, GFramework);
                                break;
                            case SDLK_e:
                                sendKeyPressed(FRKey::E, GFramework);
                                break;
						default:
							break;
						}
						break;
                    case SDL_MOUSEBUTTONDOWN:
						if (event.button.button <= SDL_BUTTON_RIGHT) {
							GFramework->onMouseButtonClick((FRMouseButton)(event.button.button - SDL_BUTTON_LEFT), false);
						}
						break;
                    case SDL_MOUSEBUTTONUP:
						if (event.button.button <= SDL_BUTTON_RIGHT) {
							GFramework->onMouseButtonClick((FRMouseButton)(event.button.button - SDL_BUTTON_LEFT), true);
						}
                        break;
					case SDL_MOUSEMOTION:
						GFramework->onMouseMove(event.motion.x, event.motion.y, event.motion.xrel, event.motion.yrel);
                        break;
                    case SDL_QUIT:
                        done = 1;
                        break;
                    default:
                        break;
                }
            }

			SDL_RenderClear(g_renderer);

			SDL_Rect viewport;
			SDL_RenderGetViewport(g_renderer, &viewport);

			/* Draw a gray background */
			SDL_SetRenderDrawColor(g_renderer, 0xA0, 0xA0, 0xA0, 0xFF);
			SDL_RenderClear(g_renderer);

			done |= GFramework->Tick() ? 1 : 0;

            SDL_RenderPresent(g_renderer);

            //SDL_Delay(1);
        }
    }

	GFramework->Close();

    SDL_DestroyRenderer(g_renderer);
    SDL_DestroyWindow(window);

	g_framework_initialized = false;

    /* We're done! */
    SDL_Quit();
    return(0);
}
