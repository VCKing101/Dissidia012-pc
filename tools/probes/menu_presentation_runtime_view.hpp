#ifndef MENU_PRESENTATION_RUNTIME_PROBE_H
#define MENU_PRESENTATION_RUNTIME_PROBE_H

#include "main/callback_runtime.h"
#include "main/resource_descriptor.h"

struct MenuExecutionState;

/* The call sites establish a 0x24 stride; the constructor's known prefix
 * occupies 0x20 bytes. The trailing bytes retain their unknown meaning. */
typedef struct MenuResourceSlot {
    MainResourceHandlePrefix prefix;
    unsigned char unknown_0020[4];
} MenuResourceSlot;

/* Observed menu setup prefix only; this is not a complete object/class size. */
typedef struct MenuRuntimePrefix {
    unsigned char unknown_0000[0x100];
    MainCallbackBlock callback_0100;
    void *field_0134;
    void *field_0138;
    unsigned char unknown_013C[0x10];
    MenuResourceSlot subobjects_014C[4];
    struct MenuExecutionState *field_01DC;
    unsigned char unknown_01E0[0x18];
    void *table_01F8;
    unsigned char unknown_01FC[0x18];
    void *table_0214;
    unsigned char unknown_0218[0x18];
    void *table_0230;
    void func_089DE1D0();
} MenuRuntimePrefix;

#endif
