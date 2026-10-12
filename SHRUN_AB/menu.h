#ifndef MENU_H
#define MENU_H

#include <Arduino.h>
#include "globals.h"
#include "runner.h"

byte flameid = 0;
boolean showRunner = false;

void drawCandle (byte x, byte y)
{
  if (arduboy.everyXFrames(4))
  {
    flameid = random(0, 24);
  }
  sprites.drawSelfMasked(x, y + 34, candleTip, 0);
  sprites.drawSelfMasked(x + 4, y + 18, candleFlame, flameid);

}

void stateMenuIntro()
{
  globalCounter++;
  sprites.drawSelfMasked(49, 20, T_arg, 0);
  if (globalCounter > 180)
  {
    globalCounter = 0;
    gameState = STATE_MENU_MAIN;
  }
};

void stateMenuMain()
{
  runnerY = 0;
  if (arduboy.everyXFrames(4)) runnerX += 4;
  if (runnerX > 127)
  {
    runnerX = -127;
    showRunner = !showRunner;
  }
  sprites.drawSelfMasked(16, 0, menuTitle, 0);
  sprites.drawSelfMasked(49, 26, menuItems, 0);
  sprites.drawSelfMasked(89, 42, menuYesNo, arduboy.audio.enabled());
  drawCandle(29, (menuSelection - 3) * 8);
  for (byte i = 3; i < 7; i++)
  {
    if (menuSelection != i) {
      sprites.drawErase(49, (i * 8) + 2, menuShade, 0);
      sprites.drawErase(82, (i * 8) + 2, menuShade, 0);
    }
  }
  if (showRunner)
  {
    sprites.drawSelfMasked(runnerX, -2, spotLight, 0);
    drawRunner();
  }
  if (arduboy.justPressed(UP_BUTTON) && (menuSelection > 3)) { menuSelection--; sfx(sfxMove, 1); }
  else if (arduboy.justPressed(DOWN_BUTTON) && (menuSelection < 6)) { menuSelection++; sfx(sfxMove, 1); }
  else if (arduboy.justPressed(A_BUTTON | B_BUTTON))
  {
    if (menuSelection < 5) gameState = menuSelection;
    else if (menuSelection == 6) gameState = STATE_MENU_JBOX;
    else
    {
      if (arduboy.audio.enabled()) arduboy.audio.off();
      else arduboy.audio.on();
      arduboy.audio.saveOnOff();
    }
    sfx(sfxSelect, 1);
  }

}

void stateMenuHelp()
{
  if (arduboy.justPressed(A_BUTTON | B_BUTTON)) gameState = STATE_MENU_MAIN;
}

void stateMenuInfo()
{
  sprites.drawSelfMasked(16, 0, menuTitle, 0);
  sprites.drawSelfMasked(15, 25, menuInfo, 0);
  if (arduboy.justPressed(A_BUTTON | B_BUTTON)) gameState = STATE_MENU_MAIN;
}

void stateMenuSoundfx()
{
  // placeHolder
}


void stateMenuPlay()
{
  gameState = STATE_GAME_INIT_LEVEL;
}


// JUKEBOX: UP/DOWN = MUSIC/SNDFX, LEFT = next number, RIGHT = previous number, B = play, A = stop (A again = back to the menu)
byte jbRow = 0;
byte jbNr[2];                                  // MUSIC 0..2, SNDFX 0..8

void stateMenuJbox()
{
  sprites.drawSelfMasked(16, 0, menuTitle, 0);
  for (byte i = 0; i < 2; i++)
  {
    sprites.drawSelfMasked(42, 32 + 12*i, jboxText, i);
    if (jbRow != i || (arduboy.frameCount & 16))              // the selected number blinks
    {
      sprites.drawSelfMasked(78, 32 + 12*i, numbers, (jbNr[i] + 1) / 10);
      sprites.drawSelfMasked(83, 32 + 12*i, numbers, (jbNr[i] + 1) % 10);
    }
  }
  byte n = jbRow ? JBOX_SFX : JBOX_MUSIC;
  if (arduboy.justPressed(UP_BUTTON)) jbRow = 0;
  if (arduboy.justPressed(DOWN_BUTTON)) jbRow = 1;
  if (arduboy.justPressed(LEFT_BUTTON)) jbNr[jbRow] = (jbNr[jbRow] + 1) % n;
  if (arduboy.justPressed(RIGHT_BUTTON)) jbNr[jbRow] = (jbNr[jbRow] + n - 1) % n;
  if (arduboy.justPressed(B_BUTTON) && arduboy.audio.enabled())
  {
    if (jbRow) sfx((const uint8_t *)pgm_read_word(&sfxList[jbNr[1]]), jbNr[1] > 6 ? 3 : 1);
    else musicPlay(jbNr[0]);
  }
  if (arduboy.justPressed(A_BUTTON))
  {
    if (musicId == MUSIC_OFF) gameState = STATE_MENU_MAIN;    // nothing playing: leave, the title tune restarts
    else musicPlay(MUSIC_OFF);
  }
}

#endif
