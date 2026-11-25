/* Shared structs for agent101 menu matches. */
typedef struct A101Sub { unsigned short s0; unsigned short s2; unsigned short s4; unsigned short s6; } A101Sub;
typedef struct A101Ent { unsigned short s0; unsigned short s2; unsigned short s4; unsigned short s6; unsigned short s8; unsigned short sA; int wC; int w10; A101Sub *w14; } A101Ent;
extern A101Ent D_089E7518[];
extern A101Sub D_089E7488[];
typedef struct A101Mask { unsigned int mask; short s4; short s6; short s8; short sA; } A101Mask;
