#ifndef MENU_CONFIGURATION_CONSUMERS_H
#define MENU_CONFIGURATION_CONSUMERS_H

#include "main/drawing_record.h"

/* Observed offsets/stack layout only. Buffer contents and resource-index
 * semantics remain unidentified; these are partial layout views. */
typedef struct MenuConfigurationOwner {
    unsigned char unknown_0000[8];
    void **field_0008;
} MenuConfigurationOwner;

typedef struct MenuConfigurationSelection {
    int index;
    int first;
    int second;
    int padding_000C;
    float buffer[0x10];
    unsigned int output;
} MenuConfigurationSelection;

extern int func_08A93D84(unsigned int index, int second);
extern int func_08A93DAC(unsigned int index);
extern void func_088DA0A8(void *object, int first, void *buffer, void *output);
extern void func_088D9AF4(void *object, void *output);

#endif
