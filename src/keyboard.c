#include<stdio.h>
#include<stdlib.h>
#include "keyboard.h"
#include "main.h"
#include <time.h>
#include "ter/process.h"
#include "other_tools/log.h"
#include "other_tools/tool.h"

typedef struct {
	Block* up, *down, *left, *right, *upWithLeft, *upWithRight, *downWithLeft, *downWithRight;
	int isUp, isDown, isLeft, isRight, isUpWithLeft, isUpWithRight, isDownWithLeft, isDownWithRight;
} Around;

static Around getAround(Block* data, int x, int y);//后面发现可以不这么写， 可是已经晚了......
static void runAround(Around a, void (*retFunc)(Block* da), int type);//type:1 only left right down up
//lambdas
static void execOpen(Block* dat);
static void execInitNumbers(Block* dat);

inline static Block* getFromArray(Block* data, int x , int y) {return data + x*X + y;}

//defined_in_h_file

SL_EXTERN(void) sl_keyboard_cleanNumbers() {
	for (register unsigned int x = 0; x < X; ++x) {
		for (register unsigned int y = 0; y < X; ++y) {
			Block* chunk = gameData + x * X + y;
			if (chunk->who > 0) {
				chunk->who = WHO_EMPTY;
			}
		}
	}
}

SL_EXTERN(void) sl_keyboard_setFlag(Vector2d pos) {
	Block* block = getFromArray(gameData, pos.x, pos.y);
	if(block->status == STATUS_CLOSE) {
		block->status = STATUS_FLAG;
	} else if (block->status == STATUS_FLAG) {
		block->status = STATUS_CLOSE;
	}
}

SL_EXTERN(OpenFinal) sl_keyboard_open(Vector2d pos) {
	if (pos.x > (X - 1) || pos.y > (X - 1)) {
		return SUCC;
	}
	Block* da = getFromArray(gameData, pos.x, pos.y);
	if (da->status != STATUS_FLAG) {
		da->status = STATUS_OPEN;
		sl_logger_info("opened %d %d", pos.x, pos.y);
		if (da->who == WHO_MINE) {
			sl_logger_info("It's a mine!!!");
			printf("FAIL\n");
			return FAIL;
		} else if (da->who > 0) {//if it's number, it can't open around
			return SUCC;
		}
		runAround(getAround(gameData, pos.x, pos.y), execOpen, 1);
	}
	return SUCC;
}

SL_EXTERN(void) sl_keyboard_spawnMinesWithCount(size_t minesCount) {
	register unsigned int i = 0;
	while(i < minesCount) {
		register unsigned int x = sl_tools_createRandomNumber(0, X);
		register unsigned int y = sl_tools_createRandomNumber(0, X);
		Block* dat = getFromArray(gameData, x, y);
		if (dat->who == WHO_EMPTY) {
			dat->who = WHO_MINE;
			i++;

			sl_logger_info("spawned 1 mine, x:%d, y:%d", x, y);

			sl_process_showProcess(i, MINES_COUNT, "Spawing Mines");
		}
		sl_process_showProcess(i, MINES_COUNT, "Spawing Mines");
	}
}

SL_EXTERN(void) sl_keyboard_initNumbers() {
	for (register unsigned int x = 0; x < X; x++) {
		for (register unsigned int y = 0; y < X; y++) {
			Block* da = getFromArray(gameData, x, y);
			if (da->who == WHO_MINE) {
				runAround(getAround(gameData, x, y), execInitNumbers, 0);
			}
		}
	}
}

SL_EXTERN(int) sl_keyboard_checkIsWinner(Block *data) {
	for (register unsigned int x = 0; x < X; x++) {
		for (register unsigned int y = 0;y < X;y++) {
			Block dat = *(getFromArray(data, x, y));
			register int who = dat.who, status = dat.status;
			if ((who == WHO_MINE && status != STATUS_FLAG)) {
				return 0;
			}

		}
	}
	return 1;
}
//lambda_for_runAround

static void execOpen(Block* dat) {
	if (dat->status != STATUS_CLOSE)return;//if it doesn't close, it opened
	
	dat->status = STATUS_OPEN;

	if(dat->who == WHO_EMPTY) {
		sl_keyboard_open(dat->pos);
	}

}

static void execInitNumbers(Block* dat) {
	if (dat->who > -1) {//-1 is MINE_ENUM
		dat->who++;
	}
}

//about_around

static Around getAround(Block* data, int x, int y) {
	Around around;
	
	around.isUpWithLeft = 0;
	around.isDownWithLeft = 0;  
	around.isDown = 0;
	around.isUp = 0;
	around.isUpWithRight = 0;
	around.isDownWithRight = 0;
	around.isRight = 0;
	around.isLeft = 0;

	//top (down add 1)
	if (x == 0) {
		around.isDown = 1;
		around.down = (data + x*X + X + y);
		if (y == 0) {//已确定坐标
			around.isRight = 1;
			around.isDownWithRight = 1;
			around.downWithRight= getFromArray(data, x + 1, y + 1);
			around.right = getFromArray(data, x, y + 1);
			return around;
		} else if(y == (X - 1)){
			around.isLeft = 1;
			around.isDownWithLeft = 1;
			around.left = getFromArray(data, x, y - 1);
			around.downWithLeft = getFromArray(data, x + 1, y - 1);
			return around;
		} else {
			around.isDownWithLeft = 1;
			around.isDownWithRight = 1;
			around.isLeft = 1;
			around.isRight = 1;

			around.downWithLeft = getFromArray(data, x + 1, y - 1);
			around.downWithRight = getFromArray(data, x + 1, y + 1);
			around.left = getFromArray(data, x, y - 1);
			around.right = getFromArray(data, x, y + 1);
			return around;
		}
			
	} else if(x == (X - 1)) {//down
		around.isUp = 1;
		around.up = (data + x*X + y - X);
		if(y == 0) {
			around.isUpWithRight = 1;
			around.isRight = 1;
			around.upWithRight = getFromArray(data, x - 1, y + 1 );
			around.right = getFromArray(data, x , y + 1);
			return around;
		} else if(y == (X - 1)) {
			around.isUpWithLeft = 1;
			around.isLeft = 1;
			around.upWithLeft = getFromArray(data, x - 1, y - 1);
			around.left = getFromArray(data, x, y - 1);
			return around;
		} else { 
			around.isLeft = 1;
			around.isRight = 1;
			around.isUpWithRight = 1;
			around.isUpWithLeft = 1;

			around.left = getFromArray(data, x, y - 1);
			around.right = getFromArray(data, x, y + 1);
			around.upWithRight = getFromArray(data, x - 1, y + 1);
			around.upWithLeft = getFromArray(data, x - 1, y - 1);
			return around;
		}
	}
	//down x + 1, up x - 1
	//left down up upWithRight downWithRight right
	if (y == 0) {
		around.isRight = 1;
		around.isDown = 1;
		around.isUp = 1;
		around.isUpWithRight = 1;
		around.isDownWithRight = 1;

		around.right = (data + x*X + y + 1);
		around.down = getFromArray(data, x + 1, y);
		around.up = getFromArray(data, x - 1, y);
		around.downWithRight = getFromArray(data, x + 1, y + 1);
		around.upWithRight = getFromArray(data, x - 1, y + 1);
		return around;
	} else if(y == (X - 1)) { //right 
		//left up down upWithLeft downWithLeft
		around.isLeft = 1;
		around.isUp = 1;
		around.isDown = 1;
		around.isUpWithLeft = 1;
		around.isDownWithLeft = 1;
		
		around.left = (data + x*X + y - 1);
		around.downWithLeft = getFromArray(data, x + 1, y - 1);
		around.upWithLeft = getFromArray(data, x - 1, y - 1);
		around.down = getFromArray(data, x + 1, y); 
		around.up = getFromArray(data, x - 1, y);
		return around;
	}
	//no special
	around.isUpWithLeft = 1;
	around.isDownWithLeft = 1;	
	around.isDown = 1;
	around.isUp = 1;
	around.isUpWithRight = 1;
	around.isDownWithRight = 1;
	around.isRight = 1;
	around.isLeft = 1;

	//down x + 1, up x - 1
	around.upWithLeft = getFromArray(data, x - 1, y - 1);
	around.downWithLeft = getFromArray(data, x + 1, y - 1);
	around.down = getFromArray(data, x + 1, y);
	around.up = getFromArray(data, x - 1, y);
	around.upWithRight = getFromArray(data, x - 1, y + 1);
	around.downWithRight = getFromArray(data, x + 1, y + 1);
	around.right = getFromArray(data, x, y + 1);
	around.left = getFromArray(data, x, y - 1);

	return around;
}

static void runAround(Around a, void (*retFunc)(Block* da), int type) {
	if (a.isLeft) {
		(*retFunc)(a.left);
	}
	if (a.isRight) {
		(*retFunc)(a.right);
	}
	if (a.isDown) {
		(*retFunc)(a.down);
	}
	if (a.isUp) {
		(*retFunc)(a.up);
	}
	if (type == 1) {
		return;
	}
	if (a.isDownWithRight) {
		(*retFunc)(a.downWithRight);
	}
	if (a.isDownWithLeft) {
		(*retFunc)(a.downWithLeft);
	}
	if (a.isUpWithRight) {
		(*retFunc)(a.upWithRight);
	}
	if (a.isUpWithLeft) {
		(*retFunc)(a.upWithLeft);
	}
}
