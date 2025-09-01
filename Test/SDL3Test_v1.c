#include <stdio.h>
#include <SDL3/SDL.h>  

int main() {
    printf("Beginning the SDL3\n") ;
    
    if (!SDL_Init(SDL_INIT_JOYSTICK)){
        SDL_Log("Couldn't initialize SDL: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    int joystickCount = 0;
    SDL_JoystickID *joystickIDs = SDL_GetJoysticks(&joystickCount);
    if (joystickIDs == NULL || joystickCount == 0) {
        printf("No joysticks connected.\n");
    } 
    else {
        printf("Found %d joystick(s):\n", joystickCount);
        for (int i = 0; i < joystickCount; i++) {
            SDL_JoystickID jid = joystickIDs[i];
            printf("Joystick at index %d has ID: %d\n", i, jid);

            SDL_Joystick *joystick = SDL_OpenJoystick(jid);
            if (joystick == NULL) {
                fprintf(stderr, "Failed to open joystick with ID %d: %s\n", jid, SDL_GetError());
            } else {
                printf("Opened joystick: %s\n", SDL_GetJoystickName(joystick));
                while (true) {  
                SDL_Event event;
                while (SDL_PollEvent(&event)) { 
                if (event.type == SDL_EVENT_JOYSTICK_AXIS_MOTION) {
                    uint8_t axis = event.jaxis.axis;
                    int16_t value = event.jaxis.value;
                    printf("Value from axis : %d, is %d \n", axis, value) ;
                    }
                } 
            }
        }
    }
    SDL_Quit();
    return 0;
    }
}

