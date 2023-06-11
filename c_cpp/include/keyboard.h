#ifndef __KEYBOARD_H__
#define __KEYBOARD_H__ 1

#include "main.h"

#define spawnMines() (sl_keyboard_spawnMinesWithCount(MINES_COUNT))
typedef enum {SUCC = 1, FAIL = 0} OpenFinal;

SL_EXTERN(int) sl_keyboard_checkIsWinner();

SL_EXTERN(void) sl_keyboard_spawnMinesWithCount(unsigned long int count);//随机生成雷
SL_EXTERN(void) sl_keyboard_initNumbers();//生成对应数
SL_EXTERN(void) sl_keyboard_setFlag(Vector2d pos);
SL_EXTERN(void)sl_keyboard_cleanNumbers();

SL_EXTERN(OpenFinal) sl_keyboard_open(Vector2d pos);

#endif
