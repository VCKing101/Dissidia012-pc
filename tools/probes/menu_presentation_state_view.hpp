#ifndef MENU_PRESENTATION_PROBE_STATE_H
#define MENU_PRESENTATION_PROBE_STATE_H

#include "menu/runtime_prefix.h"
#include "menu/savedata_request.h"

/* Observed layout views; original class identity and field ownership remain
 * provisional. The state view now covers the checked allocation footprint. */
typedef struct MenuResourceView {
    unsigned char unknown_0000[0xC];
    signed short field_000C;
    unsigned short field_000E;
    unsigned char unknown_0010[0x10];
    unsigned short field_0020;
} MenuResourceView;

typedef struct MenuExecutionState {
    unsigned char field_0000;
    unsigned char field_0001;
    unsigned char field_0002;
    unsigned char unknown_0003;
    unsigned int field_0004;
    int field_0008;
    int field_000C;
    unsigned int field_0010;
    unsigned int field_0014;
    void *field_0018;
    MenuResourceView *field_001C;
    void *field_0020;
    MenuResourceView *field_0024;
    unsigned int field_0028;
    void *field_002C;
    void *field_0030;
    unsigned char unknown_0034[0x10];
    unsigned int field_0044;
    unsigned char field_0048;
    unsigned char field_0049;
    unsigned char field_004A;
    unsigned char unknown_004B;
    int field_004C;
    int field_0050;
    int field_0054;
    void *field_0058;
    unsigned int field_005C;
    void *field_0060;
    unsigned int field_0064;
    unsigned int field_0068;
    unsigned int field_006C;
    unsigned int field_0070;
    unsigned int field_0074;
    unsigned char field_0078;
    unsigned char unknown_0079[3];
    MenuSavedataRequest savedata_007C;
    MenuSaveSlotList slots_067C;
    MenuSaveSlotEntry entries_0688[20];
    char names_0C28[21][20];
    unsigned char field_0DCC;
    unsigned char unknown_0DCD[3];
    unsigned char data_0DD0[0x3ECC8];
    MenuSaveTimestamp cached_time_3FA98;
    unsigned char unknown_3FAA8[8];
} MenuExecutionState;

#endif
