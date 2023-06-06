#ifndef CLASSIC_H
#define CLASSIC_H

#define PS2_PORT 0x64
#define PS2_COMMAND_ENABLE_FIRST_PORT 0XAE

struct keyboard* classic_init();

#endif