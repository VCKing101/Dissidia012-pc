/* Shared struct shapes for agent45 matches. */
#ifndef AGENT45_TYPES_H
#define AGENT45_TYPES_H
typedef struct A45Sub { char pad0[0x140]; char *p140; } A45Sub;
typedef struct A45Root { char pad0[4]; A45Sub *p4; } A45Root;
#endif
