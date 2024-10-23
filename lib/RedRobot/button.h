#include <avr/io.h>
#define START_BUTTON !(PING & 0x10)

void init_buttons(void);