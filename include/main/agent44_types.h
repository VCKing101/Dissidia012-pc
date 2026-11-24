/* Shared types for agent44's matching functions. */
#ifndef AGENT44_TYPES_H
#define AGENT44_TYPES_H
typedef struct Slot44 {
    char pad0[0x210];
    int f210;
    char pad1[0x22C - 0x214];
    char f22C[0x14];
    char pad2[0x7D0 - 0x240];
    int f7D0;
    char pad3[0xEB8 - 0x7D4];
    int fEB8;
} Slot44;

typedef struct Clock44 {
    char pad0[0xEA8];
    int fEA8;
    char pad1[0x1FDF8 - 0xEAC];
    int f1FDF8;
} Clock44;
#endif
