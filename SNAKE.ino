#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

Adafruit_SSD1306 oled(128, 64,&SPI, 4, 6, 12);

//-------- 4.1 -----------// 
//console buttons
int up_btn = A0;
int right_btn = A1;
int down_btn = A3;
int left_btn = A2;
int select_btn = 7;
int ok_btn = 8;
//snake directions
const int STOP = 0;
const int UP = 1;
const int RIGHT = 2;
const int DOWN = 3;
const int LEFT = 4;
int direction = STOP;
int spd = 25;
//snake position
int posX;
int posY;
//fruit position
int fruitX, fruitY;
//snake lenght
int score = 0;
int const snakeLen = 101;
int snakePosX[snakeLen];
int snakePosY[snakeLen];
bool gameOver = false;

void  snake_up(){
  int prevX = snakePosX[0];
  int prevY = snakePosY[0];
  int prev2X, prev2Y;
  snakePosX[0] = posX;
  snakePosY[0] = posY;
      
     for(int i = 1; i < score; i++){
        prev2X = snakePosX[i];
        prev2Y = snakePosY[i];
        snakePosX[i] = prevX;
        snakePosY[i] = prevY;
        prevX = prev2X;
        prevY = prev2Y;
        oled.fillCircle(snakePosX[i], snakePosY[i], 2, SSD1306_WHITE);
       if(snakePosX[i] == posX && snakePosY[i] == posY) gameOver = true;
    }
  }


void draw_fruit(){
  oled.drawCircle(fruitX, fruitY, 2, SSD1306_WHITE);
  //oled.display();
  
  if(  (posX == fruitX-1 && posY == fruitY-2) ||
       (posX == fruitX   && posY == fruitY-2) ||
       (posX == fruitX+1 && posY == fruitY-2) ||
       (posX == fruitX+2 && posY == fruitY-1) ||
       (posX == fruitX+2 && posY == fruitY)   ||
       (posX == fruitX+2 && posY == fruitY+1) ||
       (posX == fruitX+1 && posY == fruitY+2) ||
       (posX == fruitX   && posY == fruitY+2) ||
       (posX == fruitX-1 && posY == fruitY+2) ||
       (posX == fruitX-2 && posY == fruitY+1) ||
       (posX == fruitX-2 && posY == fruitY)   ||
       (posX == fruitX-2 && posY == fruitY-1)){
    fruitX = random(0, 128);
    fruitY = random(0, 64);
    //buzzer
    tone(5, 440, 100);
    delay(10);
    noTone(5);
    score++;
  }
}

void draw_point(int posX, int posY){
  oled.clearDisplay();
  oled.fillCircle(posX, posY, 2, SSD1306_WHITE);
  //oled.display();
}

void snake_control(){
  //---------- 4.2 ----------//
  pinMode(up_btn, INPUT);
  pinMode(left_btn, INPUT);
  pinMode(down_btn, INPUT);
  pinMode(right_btn, INPUT);
  pinMode(ok_btn, INPUT);
  pinMode(select_btn, INPUT);

  if(digitalRead(up_btn) == LOW){
      direction = UP;
    }
    else if(digitalRead(down_btn) == LOW){
      direction = DOWN;
    }
    else if(digitalRead(left_btn) == LOW){
      direction = LEFT;
    }
    else if(digitalRead(right_btn) == LOW){
      direction = RIGHT;
    }
    else if(digitalRead(ok_btn) == LOW){
      direction = STOP;
    }

    switch(direction){
      case UP:
            posY--;
            delay(spd);
            break;
      case DOWN:
            posY++;
            delay(spd);
            break;
      case LEFT:
            posX--;
            delay(spd);
            break;
      case RIGHT:
            posX++;
            delay(spd);
            break;
      case STOP:
            posX = posX;
            posY = posY;
            break;
    }
    //---------- 4.3 ----------//
    if( posX > 127) posX = 0;
    if(posX < 0) posX = 127;
    if(posY > 63) posY = 0;
    if(posY < 0) posY = 63;
    
    draw_point(posX, posY);
}

void setup() {
  Serial.begin(9600);
  if(!oled.begin(SSD1306_SWITCHCAPVCC)) {
    Serial.println("SSD1306 allocation failed");
    for(;;); // Don't proceed, loop forever
  }
  oled.clearDisplay();
  oled.display();
  delay(100);
  //snake random position
  randomSeed(analogRead(4));
  posX = random(0, 128);
  posY = random(0, 64);
  fruitX = random(0, 128);
  fruitY = random(0, 64);
}

void loop() {
  snake_control();
  draw_fruit();
  snake_up();
  oled.display(); // se scrie o singură dată în loop pentru o afișare corectă
  
}
