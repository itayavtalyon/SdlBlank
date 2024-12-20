//
//  main.c
//  SdlTwo
//
//  Created by Itay Avtalyon on 5/12/24.
//

#define SDL_MAIN_USE_CALLBACKS
#include "SDL_main.h"
#include <SDL.h>
#include <SDL3/SDL_main.h>

static SDL_Window *window = NULL;
static SDL_Renderer *renderer = NULL;

/*
 This will be called _once_ before anything else. argc/argv work like they
 always do. If this returns SDL_APP_CONTINUE, the app runs. If it returns
 SDL_APP_FAILURE, the app calls SDL_AppQuit and terminates with an exit
 code that reports an error to the platform. If it returns SDL_APP_SUCCESS,
 the app calls SDL_AppQuit and terminates with an exit code that reports
 success to the platform. This function should not go into an infinite
 mainloop; it should do any one-time startup it requires and then return.

 If you want to, you can assign a pointer to `*appstate`, and this pointer
 will be made available to you in later functions calls in their `appstate`
 parameter. This allows you to avoid global variables, but is totally
 optional. If you don't set this, the pointer will be NULL in later function
 calls.
 */
SDL_AppResult SDL_AppInit(void **appstate, int argc, char **argv) {
    if (!SDL_Init(SDL_INIT_VIDEO)) {
        SDL_Log("SDL initialization failed: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    window = SDL_CreateWindow("SDL Game", 640, 480, 0);
    if (!window) {
        SDL_Log("Window creation failed: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    renderer = SDL_CreateRenderer(window, NULL);
    if (!renderer) {
        SDL_Log("Renderer creation failed: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    return SDL_APP_CONTINUE;
}

/*
 This is called over and over, possibly at the refresh rate of the display or
 some other metric that the platform dictates. This is where the heart of your
 app runs. It should return as quickly as reasonably possible, but it's not a
 "run one memcpy and that's all the time you have" sort of thing. The app
 should do any game updates, and render a frame of video. If it returns
 SDL_APP_FAILURE, SDL will call SDL_AppQuit and terminate the process with an
 exit code that reports an error to the platform. If it returns
 SDL_APP_SUCCESS, the app calls SDL_AppQuit and terminates with an exit code
 that reports success to the platform. If it returns SDL_APP_CONTINUE, then
 SDL_AppIterate will be called again at some regular frequency. The platform
 may choose to run this more or less (perhaps less in the background, etc),
 or it might just call this function in a loop as fast as possible. You do
 not check the  event queue in this function (SDL_AppEvent exists for that).
 */
SDL_AppResult SDL_AppIterate(void *appstate) {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
    SDL_RenderPresent(renderer);
    return SDL_APP_CONTINUE;
}

/*
 This will be called whenever an SDL event arrives. Your app should not call
 SDL_PollEvent, SDL_PumpEvent, etc, as  SDL will manage all this for you. Return
 values are the same as from SDL_AppIterate(), so you can terminate in response
 to SDL_EVENT_QUIT, etc.
 */
SDL_AppResult SDL_AppEvent(void *appstate, SDL_Event *event) {
    if (event->type == SDL_EVENT_QUIT) {
        return SDL_APP_SUCCESS;
    }
    return SDL_APP_CONTINUE;
}

/*
 
 This is called once before terminating the app--assuming the app isn't being
 forcibly killed or crashed--as a last chance to clean up. After this returns,
 SDL will call SDL_Quit so the app doesn't have to (but it's safe for the app
 to call it, too). Process termination proceeds as if the app returned normally
 from main(), so atexit handles will run, if your platform supports that.

 If you set `*appstate` during SDL_AppInit, this is where you should free that
 data, as this pointer will not be provided to your app again.

 The SDL_AppResult value that terminated the app is provided here, in case
 it's useful to know if this was a successful or failing run of the app.
 */
void SDL_AppQuit(void *appstate, SDL_AppResult result) {
    if (renderer) SDL_DestroyRenderer(renderer);
    if (window) SDL_DestroyWindow(window);
    SDL_Quit();
}

