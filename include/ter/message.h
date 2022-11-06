#ifndef __MESSAGE_H__
#define __MESSAGE_H__

// type: 0:always 1: shoot

enum MessageType {MESSAGE_TYPE_ALWAYS=0,MESSAGE_TYPE_SHOOT=1};

extern void sl_messgae_addMessage(const char* message, int type);
extern void sl_message_printAll();
extern void sl_message_freeMessage();

#endif
