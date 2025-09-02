#include <stdio.h>
#include "Port.h"
#include "G29.h"
#include "steering.h"

#define CAN_RATE_1M   1000000
boolean joystickReady = FALSE;
typedef struct {
    Sint16 steering;
    Sint16 accelerator;
    Sint16 brake;
} val ;

val G29_val = {0, 0, 0};
SDL_mutex* state_mutex = NULL;
SDL_bool app_is_running = SDL_TRUE ;

void loop() ;
int getData_G29(void* data)  ;

int main(int argc, char *argv[]) {
//----------------------------------------- Initialize --------------------------------------//
    Serial_Begin("\\\\.\\COM11", 115200, 8, NOPARITY, 1) ;
    CAN_Begin(CAN_RATE_1M) ;
    enableWheel() ;
    SDL_Begin() ;
    joyStick_Begin() ;
//---------------------------------------------------------------------------------------------//
    state_mutex = SDL_CreateMutex();
    SDL_Log("Main: Starting worker threads...");
    SDL_Thread* producer_thread = SDL_CreateThread(getData_G29, "G29_Publish", NULL);
     while (app_is_running) {
        SDL_Event event;
        if (SDL_WaitEvent(&event)) {
            if (event.type == SDL_QUIT) {
                app_is_running = SDL_FALSE;
            }
        }
    }
}

int getData_G29(void* data) {
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
            SDL_LockMutex(state_mutex);
            G29_val.steering = SDL_JoystickGetAxis(joystick, 0); 
            G29_val.accelerator = SDL_JoystickGetAxis(joystick, 1); 
            G29_val.brake = SDL_JoystickGetAxis(joystick, 2); 
            receiveDataFromG29(G29_val.steering) ;
            SDL_UnlockMutex(state_mutex);
            SDL_Delay(1) ;
        }
    }
}

Uint32 SDLCALL can_sender_callback(Uint32 interval, void *param) {
    const unsigned int CAN_ID_G29 = 0x100;
    val local_state_to_send;

    if (!app_is_running) {
        return 0; 
    }

    SDL_LockMutex(state_mutex);
    local_state_to_send = G29_val;
    SDL_UnlockMutex(state_mutex);

    unsigned char can_data[8] = {0} ;
    can_data[0] = (local_state_to_send.steering >> 8) & 0xFF;
    can_data[1] = local_state_to_send.steering & 0xFF;
    can_data[2] = (local_state_to_send.accelerator >> 8) & 0xFF;
    can_data[3] = local_state_to_send.accelerator & 0xFF;
    can_data[4] = (local_state_to_send.brake >> 8) & 0xFF;
    can_data[5] = local_state_to_send.brake & 0xFF;

    return interval;
}