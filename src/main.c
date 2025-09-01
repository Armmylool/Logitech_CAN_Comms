#include <stdio.h>
#include "Port.h"
#include "G29.h"

boolean joystickReady = FALSE;

void loop() ;

int main(int argc, char *argv[]) {
    SDL_Begin() ;
    joyStick_Begin() ;
    loop() ;
}

void loop() {
    SDL_Event event; 
    while (TRUE) {
        while (SDL_PollEvent(&event)) {                                                                                                                                     // Initalize Joystick to control with button 23.
            if (event.type == SDL_JOYBUTTONDOWN){
                SDL_Log("Button %d pressed.", event.jbutton.button);
                if (event.jbutton.button == 23) {
                    joystickReady = TRUE ;                                                                                                                                             // Enter pin on logitech to start a program
                }   
            }
        }
        if (joystickReady == TRUE) {
        SDL_JoystickUpdate() ;
        int16_t current_steering = SDL_JoystickGetAxis(joystick, 0); 
        int16_t current_throttle = SDL_JoystickGetAxis(joystick, 1); 
        int16_t current_brake = SDL_JoystickGetAxis(joystick, 2); 
        SDL_Log("Steering: %d, Accel: %d, Brake: %d",
              current_steering, current_throttle, current_brake);
        }
    }
}