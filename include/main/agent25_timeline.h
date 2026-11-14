/* Key timeline used by agent25's 0x0892E3xx-0x0892E6xx matches. */
#ifndef AGENT25_TIMELINE_H
#define AGENT25_TIMELINE_H

typedef struct TimelineKey {
    int kind;                   /* 0x00 */
    int a;                      /* 0x04 */
    int b;                      /* 0x08 */
    int c;                      /* 0x0C */
    int d;                      /* 0x10 */
    int e;                      /* 0x14 */
    float time;                 /* 0x18 */
} TimelineKey;

typedef struct Timeline {
    char pad_00[0x48];
    TimelineKey keys[5];        /* 0x48 */
    int count;                  /* 0xD4 */
    int next;                   /* 0xD8 */
    int current;                /* 0xDC */
    unsigned int flags;         /* 0xE0 */
    float time;                 /* 0xE4 */
} Timeline;

#endif
