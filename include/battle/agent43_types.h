#ifndef BATTLE_AGENT43_TYPES_H
#define BATTLE_AGENT43_TYPES_H

/* Intrusive effect node: flags at 0x10, next at 0x4C, prev at 0x54. */
typedef struct A43Effect {
    unsigned char pad_00[0x10];
    unsigned int flags;
    unsigned char pad_14[0x38];
    struct A43Effect *next;
    unsigned char pad_50[4];
    struct A43Effect *prev;
} A43Effect;

/* Node owner: list head at 0x30, tail at 0x34, count at 0x38. */
typedef struct A43Model {
    unsigned char pad_00[0x30];
    A43Effect *head;
    A43Effect *tail;
    int count;
} A43Model;

#endif
