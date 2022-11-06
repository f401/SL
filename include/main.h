#ifndef __MAIN_H__
#define __MAIN_H__ 1

#define MINE ('M')
#define EMPTY (' ')
#define FLAG ('F')
#define UNKNOW ('U')

extern unsigned int SPACES, X, MINES_COUNT;
#ifdef __cplusplus
extern "C" {
#endif

#define SL_EXTERN(type) extern type


	typedef enum {WHO_MINE = -1, WHO_EMPTY = 0} Who;
	typedef enum {STATUS_OPEN = 1, STATUS_CLOSE = 0, STATUS_FLAG = -1} Status;

	typedef enum {MODE_OPEN_MODE = 1, MODE_SET_FLAG = 2} Modes;

	typedef enum {SIMPLE_OPEN_MODE = 1, SIMPLE_NOTHING = 0, SIMPLE_MESSAGE = 2, SIMPLE_SET_FLAG_MODE = 3} Status4Input;

	typedef struct {
		unsigned int x, y;
	} Vector2d;

	typedef struct {
		Status4Input status;
		Vector2d pos;
		char message[300];
	} UserInput;

typedef struct {
	Who who;
	Status status;
	Vector2d pos;
	//who -1 mine, 0 empty, other:number
	//status 1 open, 0 close, -1, flag
} Block;

extern Block* gameData;

typedef enum {SAVE_STATUS_DONOT_SAVE_STATUS=0, SAVE_STATUS_SAVE_STATUS=1, SAVE_STATUS_DONOT_SAVE_ANYTHING=-1} SaveStatus;

SL_EXTERN(void) sl_main_exitGame(SaveStatus shouldSaveStatus);

#ifdef __cplusplus
}
#endif

#endif
