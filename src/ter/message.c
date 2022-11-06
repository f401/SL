#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include "main.h"
#include "ter/message.h"
#include <string.h>

struct {
	char* always;
	char* shoot;
	int needShowing;
} list;

void sl_message_freeMessage() {
#define ifNotNull(what) if (what != NULL) free(what);
ifNotNull(list.always);
ifNotNull(list.shoot);
#undef ifNotNull
}

void sl_messgae_addMessage(const char* message, int type) {
	char* tmp = (char*)malloc(sizeof(char) * (strlen(message) + 2));
	for(unsigned int i = 0 ; i < strlen(message); i++) {
		char sign = message[i];
		if (sign == '\0') {
			break;
		}
		tmp[i] = sign;
	}
	tmp[strlen(message) - 2] = '\n';
	tmp[strlen(message) - 1] = '\0';
	if (type == 0) {
		if (list.always != NULL) {
			list.always = (char*) realloc(list.always, sizeof(char) * (strlen(list.always) + strlen(message) + 2));
			strcat(list.always, tmp);
		} else {
			list.always = tmp;
		}
	} else {
		if (list.shoot != NULL) {
			list.shoot = (char*) realloc(list.shoot, sizeof(char) * (strlen(list.shoot) + strlen(message) + 2)); 
			strcat(list.shoot, tmp);
		} else {
			list.shoot = tmp;
		}
		list.needShowing = 1;
	}
}

void sl_message_printAll() {
	printf("----------------------------------------------------------\n");
	printf("%s", list.always);
	printf("----------------------------------------------------------\n");
	if (list.needShowing) {
		printf("%s", list.shoot);
		printf("----------------------------------------------\n");
		free(list.shoot);
		list.shoot = NULL;
		list.needShowing = 0;
	}
}
