#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Arduino_FreeRTOS.h>
#include "semphr.h"
#define rtDelay(v) \
  { vTaskDelay(v / portTICK_PERIOD_MS); }  // Macro to make things easier

LiquidCrystal_I2C lcd(0x27, 16, 2);  // Initialize LCD module
SemaphoreHandle_t xSemaphore;        // Declare Smaphore Handle
TaskHandle_t dodgeHandle;
TaskHandle_t scrollHandle;

// Declare structure for characters to be displayed
typedef struct {
  byte value[8];
  uint8_t label;
} character;

character human = { { 0b01110, 0b01110, 0b00100, 0b01110, 0b10101, 0b00100, 0b01010, 0b01010 }, 0 };   // Human
character bullet = { { 0b00000, 0b00000, 0b00000, 0b01111, 0b11111, 0b01111, 0b00000, 0b00000 }, 1 };  // Bullet

int pos = 0;            // Position to be toggled
const int upKey = 2;    // ISR pin for dodging up
const int downKey = 3;  // ISR pin for dodging down

int score = 0;  //scores
int delayPeriod = 300;

void setup() {

  // Intialize interrupt pin and attatch ISR to it
  pinMode(upKey, INPUT_PULLUP);
  pinMode(downKey, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(upKey), dodgeUp, FALLING);
  attachInterrupt(digitalPinToInterrupt(downKey), dodgeDown, FALLING);

  // Initialize the LCD so that it can display
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);

  // Create characters
  lcd.createChar(human.label, human.value);
  lcd.createChar(bullet.label, bullet.value);

  // RTOS tasks and semaphore operations
  xSemaphore = xSemaphoreCreateMutex();
  xTaskCreate(dodge, "dodge", 128, NULL, 1, &dodgeHandle);
  xTaskCreate(scrollBullet, "bullet", 128, NULL, 1, &scrollHandle);
  xSemaphoreGive(xSemaphore);
}

void loop() {}

void dodgeUp() {
  pos = 0;
}

void dodgeDown() {
  pos = 1;
}

// Task Handle for dodgine
void dodge(void*) {
  while (1) {
    xSemaphoreTake(xSemaphore, portMAX_DELAY);
    if (pos) {
      lcd.setCursor(0, 0);
      lcd.print(" ");
    } else {
      lcd.setCursor(0, 1);
      lcd.print(" ");
    }
    lcd.setCursor(0, pos);
    lcd.write(human.label);
    xSemaphoreGive(xSemaphore);
    rtDelay(30);
  }
}

// Task Handle for scrolling bullet
void scrollBullet(void*) {
  int y1 = random(2);
  int y2 = random(2);
  int y3 = random(2);
  while (1) {
    xSemaphoreTake(xSemaphore, portMAX_DELAY);
    // Position for three bullets:
    static int x1 = 15 + random(2, 4);
    static int x2 = x1 + random(2, 5);
    static int x3 = x2 + random(2, 7);

    checkStatus(&x1, &y1);
    checkStatus(&x2, &y2);
    checkStatus(&x3, &y3);

    createBullets(x1, y1, x2, y2, x3, y3);

    shiftBullets(&x1, &y1, &x2, &y2, &x3, &y3);    

    xSemaphoreGive(xSemaphore);
    rtDelay(delayPeriod);
  }
}

void checkStatus(int* x, int* y) {  // renews value of x and y if x is 0 and x isn't touching person else it calls gameover
  if (*x == 0) {
    if (pos != *y) {
      *x = 15 + random(2, 4);
      *y = random(2);
      score += 1;

      if (score > 0 && score % 11 == 0) {
        delayPeriod -= 20;
      }
    } else {
      gameOver();
    }
  }
}

void createBullet(int x, int y) {  // creates the bullet
  lcd.setCursor(x, y);
  lcd.write(bullet.label);
}

void createBullets(int x1, int y1, int x2, int y2, int x3, int y3) {  // Creates the bullets
  createBullet(x1, y1);
  createBullet(x2, y2);
  createBullet(x3, y3);
}

void shiftBullet(int* x, int* y) {  // Moves the bullet towards the person
  lcd.setCursor(*x, *y);
  lcd.print(" ");
  *x -= 1;
  lcd.setCursor(*x, *y);
  createBullet(*x, *y);
}

void shiftBullets(int* x1, int* y1, int* x2, int* y2, int* x3, int* y3) {  // Moves the bullets towards the person
  shiftBullet(x1, y1);
  shiftBullet(x2, y2);
  shiftBullet(x3, y3);
}

void gameOver() {  // Gameover
  lcd.clear();
  lcd.print("Game over");
  rtDelay(100);
  lcd.setCursor(0,1);
  lcd.print("Score: ");
  lcd.print(score);
  vTaskDelete(dodgeHandle);
  vTaskDelete(scrollHandle);
}