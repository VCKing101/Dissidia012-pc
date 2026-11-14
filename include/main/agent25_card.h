/* Card-battle unit body (unit->body at 0x20), shared by agent25's
   0x08925xxx-0x08926xxx matches. */
#ifndef AGENT25_CARD_H
#define AGENT25_CARD_H

typedef struct CardLink8 {
    short id;                   /* 0x0 */
    short pad_2;
    int value;                  /* 0x4 */
} CardLink8;

typedef struct CardLink12 {
    short id;                   /* 0x0 */
    short pad_2;
    int value;                  /* 0x4 */
    int extra;                  /* 0x8 */
} CardLink12;

typedef struct CardBody {
    char pad_000[0x12];
    short kind;                 /* 0x012 */
    char pad_014[2];
    short special;              /* 0x016 */
    char pad_018[0x8C - 0x18];
    short ids[21];              /* 0x08C */
    signed char id_count;       /* 0x0B6 */
    signed char link8_count;    /* 0x0B7 */
    signed char link12_count;   /* 0x0B8 */
    char pad_0B9[3];
    CardLink8 links8[120];      /* 0x0BC */
    CardLink12 links12[120];    /* 0x47C */
} CardBody;

typedef struct CardUnit {
    char pad_00[0x20];
    CardBody *body;             /* 0x20 */
} CardUnit;

#endif
