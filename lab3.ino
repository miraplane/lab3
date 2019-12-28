
#include <Servo.h>
#include "game.h"
#include "LedControl.h"

#define INVALID_DIRECTION -1

const char keyUp = 'U';
const char keyDown = 'D';
const char keyNone = 'N';

const int boardSizeX = 24;
const int boardSizeY = 8;
const int boardSizeDisplay = 8;

Game game(boardSizeX, boardSizeY);

LedControl ledDisplay = LedControl(26, 22, 24, 3);

uint64_t lastGameUpdate;
uint64_t lastDisplayUpdate;
uint64_t lastFoodBlink;
const uint64_t gameUpdateDelayMs = 500;
const uint64_t displayUpdateDelayMs = 100;
const uint64_t foodBlinkDelayMs = 200;
bool showingFood = true;

char currentKey1 = keyNone;
char currentKey2 = keyNone;

const byte rowAmount = 4;
const byte colAmount = 4;

char keyMatrix[rowAmount][colAmount] = {
  {keyNone, keyUp,    keyNone,  keyNone},
  {keyNone, keyNone,  keyNone, keyNone},
  {keyNone, keyDown,  keyNone,  keyNone},
  {keyNone, keyNone,  keyNone,  keyNone}
};

static bool keyDownMatrix[rowAmount][colAmount];

byte rowPins1[rowAmount] = { 5, 4, 3, 2 };
byte colPins1[colAmount] = { 6, 7, 8, 9 };

byte rowPins2[rowAmount] = { 42, 44, 46, 48 };
byte colPins2[colAmount] = { 40, 38, 36, 34 };

Servo servo1;
Servo servo2;

void setup()
{
  int devices = ledDisplay.getDeviceCount();
  for (int address = 0; address < devices; address++)
  {
    ledDisplay.shutdown(address, false);
    ledDisplay.setIntensity(address, 10);
    ledDisplay.clearDisplay(address);
  }

  for (int i = 0; i < rowAmount; i++) {
    pinMode(rowPins1[i], OUTPUT);
    digitalWrite(rowPins1[i], HIGH);
  }

  for (int i = 0; i < colAmount; i++) {
    pinMode(colPins1[i], INPUT);
    digitalWrite(colPins1[i], HIGH);
  }
  
  for (int i = 0; i < rowAmount; i++) {
    pinMode(rowPins2[i], OUTPUT);
    digitalWrite(rowPins2[i], HIGH);
  }

  for (int i = 0; i < colAmount; i++) {
    pinMode(colPins2[i], INPUT);
    digitalWrite(colPins2[i], HIGH);
  }

  servo1.attach(10);
  servo2.attach(9);
}

void loop()
{
  currentKey1 = getKey(0);
  currentKey2 = getKey(1);
  
  if (game.isRunning()) {
    updateInput();
    updateGame();
    updateDisplay();
  }
  else {
    if (game.getWinner() == 0) {
      servo1.write(90);
    }
    else {
      servo2.write(90);
    }
  }
}

PaddleDirection keyToDirection(char key)
{
  switch(key) {
    case keyUp: return UP;
    case keyDown: return DOWN;
  }
  return INVALID_DIRECTION;
}

char getKey(int paddleIndex)
{  
  for (int i = 0; i < rowAmount; i++) {
    for (int j = 0; j < colAmount; j++) {
      bool keyDown;
      if (paddleIndex == 0) {
        keyDown = isKeyDown(i, j, rowPins1, colPins1);
      }
      else {
        keyDown = isKeyDown(i, j, rowPins2, colPins2);
      }
      if (keyDown) {
        return keyMatrix[i][j];
      }
    } 
  }
  return keyNone;
}

bool isKeyDown(int i, int j, byte* rowPins, byte* colPins)
{
  bool result = false;
  digitalWrite(rowPins[i], LOW);
  if (digitalRead(colPins[j]) == LOW) {
    result = true;
  }
  digitalWrite(rowPins[i], HIGH);
  return result;
}

void drawPoint(Point point)
{
  int displayAddress = int(point.x / boardSizeDisplay);
  int column = point.x - displayAddress * boardSizeDisplay;
  int row = boardSizeY - 1 - point.y;
  ledDisplay.setLed(displayAddress, row, column, true);
}

void drawPaddle(Paddle &paddle)
{
  for (int i = 0; i < paddleSize; i++) {
    drawPoint(paddle.getPosition(i));
  }
}

void updateInput() {
  char key1 = getKey(0);
  char key2 = getKey(1);
  if (key1 != keyNone) {
    currentKey1 = key1;
  }
  if (key2 != keyNone) {
    currentKey2 = key2;
  }
}

void updateGame()
{
  if (millis() - lastGameUpdate > gameUpdateDelayMs) {
    if (currentKey1 != keyNone) {
      game.paddleMove(keyToDirection(currentKey1), 0);
    }
    if (currentKey2 != keyNone) {
      game.paddleMove(keyToDirection(currentKey2), 1);
    }
    game.update();
    currentKey1 = keyNone;
    currentKey2 = keyNone;
    lastGameUpdate = millis();
  }
}

void updateDisplay()
{
  if (millis() - lastDisplayUpdate > displayUpdateDelayMs) {
    Ball ball = game.getBall();
    Paddle paddle1 = game.getPaddle(0);
    Paddle paddle2 = game.getPaddle(0);
    
    for (int address = 0; address < ledDisplay.getDeviceCount(); address++) {
      ledDisplay.clearDisplay(address);
    }

    drawPoint(ball.getPosition());
    drawPaddle(paddle1);
    drawPaddle(paddle2);
    lastDisplayUpdate = millis();
  }
}
