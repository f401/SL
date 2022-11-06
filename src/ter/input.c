#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include "main.h"
#include "ter/input.h"
#include <stdbool.h>
#include "other_tools/tool.h"
#include "other_tools/log.h"

static bool isFirst = true;
static char* dat = NULL;
static int size = 1;
static bool isEnter = false;

static int resizeInput(int size);

#define addSize() resizeInput(size + 1)
#define delSize() resizeInput(size - 1)

void sl_input_freeInput() { if (dat) free(dat); }

void sl_input_readHandleInput() {
	char ch = getchar();
	if (ch == '\b') {
		if (size > 1) {
			delSize();
			dat[size - 1] = '\0';
		}
	} else if (ch == 27) {//esc
		sl_main_exitGame(SAVE_STATUS_SAVE_STATUS);
	} else if (ch == '\n') {
		isEnter = true;
		sl_logger_info("user entered");
	} else {
		addSize();
		dat[size - 2] = ch;
		dat[size - 1] = '\0';
	}
}

UserInput sl_input_getUserInput() {	
	UserInput v;
	v.status = SIMPLE_NOTHING;
	v.pos.x = -1;
	v.pos.y = -1;

	Status4Input statusTmp = SIMPLE_NOTHING;

	if (dat == NULL) {
		v.status = SIMPLE_NOTHING;
		v.pos.x = v.pos.y = -1;
	} else if(sl_tools_checkAllIsNumber(dat)) {
		unsigned int count = 0, lastIndex = 0;

		for(unsigned register int i = 0 ; i < size ; i++) {
			char ch = dat[i];
			
			if(ch == ' ') {
				if (count == 0) {
					char* st = sl_tools_substring(dat, 0, i);
					lastIndex = i;
					v.pos.x = atoi(st);
					free(st);
				} else if(count == 1) {
					char* st = sl_tools_substring(dat, lastIndex, i);
					lastIndex = i;
					v.pos.y = atoi(st);
					free(st);

					char* mstr = sl_tools_substring(dat, i, size -1);

					switch(atoi(mstr)) {
						case 1:
							statusTmp = SIMPLE_OPEN_MODE;
							break;
						case 2:
							statusTmp = SIMPLE_SET_FLAG_MODE;
							break;
					}

					free(mstr);
				} else  {
					v.status = SIMPLE_MESSAGE;
					strcpy(v.message, "too many spaces!");
				}
				count++;
			}  
		}		
	} else { 
		strcpy(v.message, "There is not number in input");
		v.status = SIMPLE_MESSAGE;
	}

	if (isEnter) {
		isFirst = true;
		isEnter = false;
		free(dat);
		resizeInput(1);
		dat[0] = '\0';
		v.status = statusTmp;
		sl_logger_info("passing status : %d", statusTmp);
	}
	sl_logger_info("vector: x: %d, y: %d", v.pos.x, v.pos.y);
	return v;
}

static int resizeInput(int msize) {
	if(isFirst) {
		dat = (char*)calloc(sizeof(char), msize);
		isFirst = false;
	} else {
		dat = (char*)realloc(dat, sizeof(char)*msize);
	}
	if(dat == NULL) {
		printf("There is something wrong with resizeInput. msize:%d", msize);
		return 0;	
	}
	size = msize;
	return 1;
}

void sl_inputprintInput() {
	if(dat != NULL) {
		printf("%s", dat);
	}
}
