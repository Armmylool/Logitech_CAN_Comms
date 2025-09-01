#include <stdio.h>
#include "Port.h"
#include "G29.h"

void loop() ;

int main(int argc, char *argv[]) {
    SDL_Begin() ;
    joyStick_Begin() ;
    loop() ;
}

void loop() {
    while (TRUE) {
        SDL_JoystickUpdate() ;
        int16_t current_steering = SDL_JoystickGetAxis(joystick, 0); 
        int16_t current_throttle = SDL_JoystickGetAxis(joystick, 1); 
        int16_t current_brake = SDL_JoystickGetAxis(joystick, 2); 
        SDL_Log("Steering: %d, Accel: %d, Brake: %d",
              current_steering, current_throttle, current_brake);
    }
}
