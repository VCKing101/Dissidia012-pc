#ifndef AGENT60_TYPES_H
#define AGENT60_TYPES_H
typedef struct Rec8C {
    int state;
    char pad04[0x88];
    int f8C;
    char pad90[4];
    int f94;
    int f98;
    int f9C;
    unsigned char fA0;
    unsigned char fA1;
    unsigned char fA2;
} Rec8C;
typedef struct S4C66C { char pad[0x30]; int f30; int f34; int f38; void *h; int f40; int f44; } S4C66C;
typedef struct W39B84 { void *w0; char pad04[0x10]; void *w14; char pad18[4]; void *w1C; char pad20[4]; void *w24; void *w28; } W39B84;
typedef struct B39B84 { char pad[0xC]; void *wC; void *w10; char pad14[0x18]; int f2C; } B39B84;
typedef struct S39B84 { char pad[0x44]; unsigned flags; char pad48[0x7C]; B39B84 *b; W39B84 *w; } S39B84;
typedef struct Item51B8 {
    short kind; short pad2;
    int pad4;
    int f8; int fC; int f10;
    char pad14[0xC];
    int f20; int f24;
    char pad28[0x10];
    void (*cb)(struct Item51B8 *);
} Item51B8;
typedef struct Item510C {
    short kind; short pad2;
    int pad4;
    int f8; int fC;
    char pad10[0x10];
    int f20; int f24;
    char pad28[4];
    void (*cb)(struct Item510C *);
    char pad30[0xC];
    int f3C; int f40; int f44;
} Item510C;
typedef struct Pair35 { int a; unsigned char b; char pad[3]; } Pair35;
typedef struct P35 {
    int w0; int w1; int w2; int w3;
    Pair35 pr[5];
    int w0E; int w0F; int w10; int w11; int w12; int w13; int w14; int w15;
} P35;
typedef struct GS35 {
    int g0; int g4; int g8; int gC; int g10; int g14;
    int a[5];
    int b[5];
    int g40; int g44; int g48; int g4C; int g50; int g54; int g58;
} GS35;
typedef struct GH35 { char pad[0x3F63C]; GS35 s; } GH35;
#endif
