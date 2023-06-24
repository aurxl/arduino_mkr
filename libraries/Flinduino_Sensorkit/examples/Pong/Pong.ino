// This implementation of Pong is adapted from eholks verison using an Adafruit Oled https://github.com/eholk/Arduino-Pong 
// and using GraphicsTest.ino, Universal 8bit Graphics Library (https://github.com/olikraus/u8g2/)
// Copyright (c) 2016, olikraus@gmail.com All rights reserved.


#include "Flinduino_SensorKit.h"


#define POT A0

const unsigned long PADDLE_RATE_PLAYER = 1; //controls the paddle response
const unsigned long PADDLE_RATE_CPU = 20; //controls how the cpu can react
const unsigned long BALL_RATE = 1; //controls how the ball refreshes
const uint8_t PADDLE_HEIGHT = 12;
const uint8_t POT_DIVIDER = 1024 / (63 - PADDLE_HEIGHT);

uint8_t ball_x = 64, ball_y = 32;
uint8_t ball_dir_x = 1, ball_dir_y = 1;
unsigned long ball_update;

unsigned long paddle_update_cpu;
unsigned long paddle_update_player;

const uint8_t CPU_X = 12;
uint8_t cpu_y = 16;

const uint8_t PLAYER_X = 115;
uint8_t player_y = 16;


void setup() {
  
    OledG.begin();
    OledG_prepare();

    unsigned long start = millis();

    while(millis() - start < 2000);

    ball_update = millis();
    paddle_update_player = ball_update;
    paddle_update_cpu = ball_update;

}

void loop(void) {
    OledG.firstPage();  
    
    bool update = false;
    unsigned long time = millis();

    static bool up_state = false;
    static bool down_state = false;

    do {
        OledG.drawFrame(0, 0, 128, 64);

        if(time > ball_update) {
            uint8_t new_x = ball_x + ball_dir_x;
            uint8_t new_y = ball_y + ball_dir_y;
    
            // Check if we hit the vertical walls
            if(new_x == 0 || new_x == 127) {
               ball_dir_x = -ball_dir_x;
                new_x += ball_dir_x + ball_dir_x;
            }
    
            // Check if we hit the horizontal walls.
            if(new_y == 0 || new_y == 63) {
                ball_dir_y = -ball_dir_y;
                new_y += ball_dir_y + ball_dir_y;
            }
    
            // Check if we hit the CPU paddle
            if(new_x == CPU_X && new_y >= cpu_y && new_y <= cpu_y + PADDLE_HEIGHT) {         
               ball_dir_x = -ball_dir_x;
                new_x += ball_dir_x + ball_dir_x;     
            }
    
            // Check if we hit the player paddle
            if(new_x == PLAYER_X
               && new_y >= player_y
               && new_y <= player_y + PADDLE_HEIGHT)
            {
               ball_dir_x = -ball_dir_x;
                new_x += ball_dir_x + ball_dir_x;
            }

            OledG.drawPixel(new_x, new_y);
            ball_x = new_x;
            ball_y = new_y;
    
            ball_update += BALL_RATE;
    
        }  
        if(time > paddle_update_cpu) {
            paddle_update_cpu += PADDLE_RATE_CPU;
            
            // CPU paddle
            const uint8_t half_paddle = PADDLE_HEIGHT >> 1;
            if(cpu_y + half_paddle > ball_y) {
                cpu_y -= 1;
            }
            if(cpu_y + half_paddle < ball_y) {
                cpu_y += 1;
            }
          
           if(cpu_y < 1) cpu_y = 1;
            if(cpu_y + PADDLE_HEIGHT > 63) cpu_y = 63 - PADDLE_HEIGHT;
            OledG.drawLine(CPU_X, cpu_y, CPU_X, cpu_y + PADDLE_HEIGHT);
        }
        
        if(time > paddle_update_player) {
            paddle_update_player += PADDLE_RATE_PLAYER;
        
            player_y = analogRead(POT) / POT_DIVIDER;
            
            if(player_y < 1) player_y = 1;
            if(player_y + PADDLE_HEIGHT > 63) player_y = 63 - PADDLE_HEIGHT;
            OledG.drawLine(PLAYER_X, player_y, PLAYER_X, player_y + PADDLE_HEIGHT);

        }

    } while( OledG.nextPage());
  
}

void OledG_prepare(void) {
  
  OledG.setFont(u8g2_font_6x10_tf);
  OledG.setFontRefHeightExtendedText();
  OledG.setDrawColor(1);
  OledG.setFontPosTop();
  OledG.setFontDirection(0);
  
}
