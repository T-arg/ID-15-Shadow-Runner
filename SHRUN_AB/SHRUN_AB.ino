/*
  SHADOW RUNNER: https://t-arg.github.io/ID-15-Shadow-Runner/

  Arduboy version 1.6 : https://github.com/T-arg/ID-15-Shadow-Runner

  MADE by TEAM a.r.g. : https://t-arg.github.io/

  2015 - JO3RI - Trodoss

  Special thanks to Dreamer3 for helping with the conversion from Gamby

  Game License: MIT : https://opensource.org/licenses/MIT

*/

//determine the game
#define GAME_ID 15

#include "globals.h"
#include "runner.h"
#include "menu.h"
#include "game.h"
#include "items.h"
#include "playfield.h"


typedef void (*FunctionPointer) ();

const FunctionPointer PROGMEM mainGameLoop[] = {
  stateMenuIntro,
  stateMenuMain,
  stateMenuHelp,
  stateMenuPlay,
  stateMenuInfo,
  stateMenuSoundfx,
  stateGameInitLevel,
  stateGamePlaying,
  stateGamePause,
  stateGameOver,
};

void setup () {
  arduboy.begin();
  arduboy.setFrameRate(60);
  arduboy.initRandomSeed();
}

void loop() {
  if (!(arduboy.nextFrame())) return;
  arduboy.pollButtons();
  arduboy.clear();
  ((FunctionPointer) pgm_read_word (&mainGameLoop[gameState]))();
  arduboy.display();
}

