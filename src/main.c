//system
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <ncurses.h>
#include <getopt.h>
//ter
#include "ter/process.h"
#include "keyboard.h"
#include "ter/consle.h"
#include "ter/theme.h"
#include "ter/input.h"
#include "ter/message.h"
//tools
#include "other_tools/log.h"
#include "other_tools/tool.h"
//screen
#include "screen/ncurses_main.h"
#include "FileBridge.h"

#include "main.h"
#include <config.h>

Block* gameData;

unsigned int X = 10, SPACES = 1, MINES_COUNT = 10;

static int type;//0:consle, 1:ncurses
enum Types { TYPES_CONSLE = 0, TYPES_NCURSES = 1};
static int where2load = 0;
enum WhereLoad {WHERE_DEFALUT=0, WHERE_LOAD_FROM_FILE=1};

static void init();
static void startGameWithConsle();
static inline void loadArgs(int argc, char** argv);
static void startGameWithNcurses();
static void freeData();
void updateConlse(Vector2d pos);

static void handleSigInt(int i);

int main(int argc, char** argv) {

	sl_logger_openCppStream();
	sl_logger_info("starting game");
	loadArgs(argc, argv);
	init();

	if (type == TYPES_CONSLE) {
		startGameWithConsle();
	} else if(type == TYPES_NCURSES) {
		sl_ncursesMain_initNcurses();
		startGameWithNcurses();
	}
}

static void handleError11(int i) {
	printf("I'm so sorry that there is some error(code %d )\nExiting\n", i);
	sl_logger_error("Error with code %d\n", i);
	exit(-1);
}


static void startGameWithNcurses() {
	//int showX = -1, showY = -1;
	Vector2d showPos = {-1, -1};
	for(;;) {
		sl_ncursesMain_clearNcursesScreen();
		sl_ncursesMain_printData(showPos, 0);
		if(sl_keyboard_checkIsWinner(gameData)) {
			sl_ncursesMain_clearNcursesScreen();
			printw("Yon win!\n, Press any key to exit game");
			sl_logger_info("player win, exitGame");
			sl_ncursesMain_printData(showPos, 1);
			refresh();
			getch();
			sl_main_exitGame(0);
		}

		UserInput v = sl_ncursesMain_runSecondMouse();
		showPos.x = v.pos.x - 1;
		showPos.y = v.pos.y - 1;
		if(v.status == SIMPLE_OPEN_MODE) {
			register OpenFinal final = sl_keyboard_open(showPos);
			if (final == FAIL) {
				sl_ncursesMain_clearNcursesScreen();
				printw("You lost!\n");
				sl_ncursesMain_printData(showPos, 1);
				getch();
				sl_main_exitGame(SAVE_STATUS_DONOT_SAVE_STATUS);
			}
		} else if (v.status == SIMPLE_SET_FLAG_MODE) {
			sl_keyboard_setFlag(showPos);
		}

	}
}

static inline void loadArgs(int argc, char** argv) {
	static char helpText[] = "Usage: -v/--version Print version.\n\t-h/--help Print this text;\n\t-c/--consle 控制台模式 \n\t-n/--ncurses: ncurses模式 \n\t-m/--mines <number> 地雷数量\n\t-s/--spaces <number>间距\n\t-x <number> 地图大小\n\t-f/--file [file] 从路径加载\n";
#define exitDontSaveAnyThing() sl_main_exitGame(SAVE_STATUS_DONOT_SAVE_ANYTHING)
#define setNumberArgument(what) { \
if (optarg) { \
	what = atoi(optarg); \
	break; \
} else { \
	sl_logger_error("Error: Input m doesn't with arguments, exit soon"); \
	printf("Can't find number arguments, try '-h'.\n"); \
	sl_main_exitGame(SAVE_STATUS_DONOT_SAVE_ANYTHING); \
} \
}
	int choice;
	type = TYPES_CONSLE;
	sl_logger_info("loading arguments");
	while (1)
	{
		static struct option long_options[] =
		{
			/* Use flags like so:
			{"verbose",	no_argument,	&verbose_flag, 'V'}*/
			/* Argument styles: no_argument, required_argument, optional_argument */
			{"version", no_argument,0,'v'},
			{"help",	no_argument,0,'h'},
			{"consle", no_argument, 0, 'c'},
			{"ncurses", no_argument, 0, 'n'},
			{"mines", required_argument, 0, 'm'},
			{"spaces", required_argument, 0, 's'},
			{"", required_argument, 0, 'x'},
			{"file", optional_argument, 0, 'f'},
			{0,0,0,0}
		};

		int option_index = 0;

		/* Argument parameters:
			no_argument: " "
			required_argument: ":"
			optional_argument: "::" */

		choice = getopt_long( argc, argv, "-vhcnm:s:x:f::",
					long_options, &option_index);
		if (choice == -1)
			break;
		switch( choice )
		{
			case 'h':
				sl_logger_info("option h, exit soon");
				printf("%s\n", helpText);
				exitDontSaveAnyThing();
			case 'v':
				sl_logger_info("option v, exit soon");
				printf("version: %s\n", VERSION);
				exitDontSaveAnyThing();
			case 'c' :
				sl_logger_info("option c");
				type = TYPES_CONSLE;
				break;
			case 'n' :
				sl_logger_info("option n");
				type = TYPES_NCURSES;
				break;
			case 'm':
				sl_logger_info("option m");
				setNumberArgument(MINES_COUNT);
			case 'x': 
				sl_logger_info("option x");
				setNumberArgument(X);
			case 's':
				sl_logger_info("option s");
				setNumberArgument(SPACES);
			case 'f':
				sl_logger_info("option f");
				sl_FileBridge_loadArgsFromFile(optarg ? optarg : NULL);
				where2load = WHERE_LOAD_FROM_FILE;
				break;
			case 1:
			case '?':
				/* getopt_long will have already printed an error */
				sl_logger_error("Unknow argument! %c", (char)optopt);
				printf("try -h\n");
				exitDontSaveAnyThing();
				break;
			default:
				/* Not sure how to get here... */
				sl_logger_error("Unknow error, exit soon");
				exitDontSaveAnyThing();
		}
	}
#undef exitDontSaveAnyThing
#undef setNumberArgument
}

static void startGameWithConsle() {
	Vector2d showPos = {-1, -1};
	for(;;) {

		updateConlse(showPos);

		if(sl_keyboard_checkIsWinner(gameData)) {
			printf("Yon win!\n");
			sl_logger_info("player win, exitGame");
			sl_consle_printDataForDEBUG(showPos, 1);
			sl_main_exitGame(0);
		}
		sl_input_readHandleInput();
		UserInput v = sl_input_getUserInput();
		showPos.x = v.pos.x - 1;
		showPos.y = v.pos.y - 1;
		if (v.status == SIMPLE_OPEN_MODE) {
			register OpenFinal final = sl_keyboard_open(showPos);
			if (final == FAIL) {
				sl_consle_printDataForDEBUG(showPos, 1);
				sl_logger_info("player lose");
				sl_main_exitGame(SAVE_STATUS_DONOT_SAVE_STATUS);
			}
		} else if (v.status == SIMPLE_SET_FLAG_MODE) {
			sl_keyboard_setFlag(showPos);
		} else if (v.status == SIMPLE_MESSAGE) {
			sl_messgae_addMessage(v.message, 1);
		}
	}
}

void updateConlse(Vector2d pos) {
	system("clear");
	sl_consle_printData(pos);
	sl_message_printAll();
	sl_inputprintInput();
}

static void init() {
	signal(SIGSEGV, handleError11);
	signal(SIGINT, handleSigInt);
	atexit(freeData);
	gameData = (Block*)calloc(sizeof(Block), X * X + 1);
	printf("start to load data\n");
	for (register unsigned int x = 0; x < X; x++) {
		for (register unsigned int y = 0; y < X; y++) {
			Block* dat = gameData + X*x + y;
			dat->pos.x = x;
			dat->pos.y = y;
			sl_process_showProcess(x*X + y, X * X - 1, "loading");
		}
	}
	if(where2load == WHERE_LOAD_FROM_FILE) {
		sl_logger_info("load from file");
		sl_FileBridge_loadDataFromFile();
	} else if (where2load == WHERE_DEFALUT) {
		spawnMines();
		sl_keyboard_initNumbers();
	}
	sl_logger_info("loaded Game Data");
	sl_messgae_addMessage("输入格式：x y mode，mode：0: null, 1：翻牌，2：插旗", 0);
	system("stty -icanon");  //read keyboard data
}

SL_EXTERN(void) sl_main_exitGame(int shouldSaveStatus) {
	sl_logger_info("exiting game......");
	if(shouldSaveStatus != SAVE_STATUS_DONOT_SAVE_ANYTHING) {
		printf("\nneed to save(y)???");
		if (getchar() == 'y') {
			sl_logger_info("start to save data");
			sl_FileBridge_save(shouldSaveStatus);
			sl_logger_info("save finished!");
		}
	}
	if (type == TYPES_NCURSES) {
		sl_ncursesMain_exitNcurses();
	}
	exit(0);
}

static void freeData() {
#define ifNotNullFree(what) if (what != NULL) free(what);
	sl_logger_info("freeing game data");
	ifNotNullFree(gameData);
	sl_message_freeMessage();
	sl_input_freeInput();
	sl_theme_showCursor();
	sl_logger_closeCppStream();
#undef ifNotNullFree
}

static void handleSigInt(int i) {
	printf("exit by user\n");
	sl_main_exitGame(SAVE_STATUS_SAVE_STATUS);
}
