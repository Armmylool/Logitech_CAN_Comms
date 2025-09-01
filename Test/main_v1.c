#include <stdio.h>
#include "Port.h"
#include "G29.h"

void loop() ;

int main() {
    SDL_Begin() ;
    joyStick_Begin() ;
    loop() ;
}

void loop() {
    while (true) {
        SDL_Event event ;
        while(SDL_PollEvent(&event)) {
            if(event.type ==SDL_EVENT_JOYSTICK_AXIS_MOTION ) {
                uint8_t id = event.jaxis.axis ;
                int16_t value = event.jaxis.value ;
                printf("ID : %d\t", id) ;
                printf("Value : %d\n", value) ;
            }
        }
    }
}

// 