// Shared types for agent50 matches.
#ifndef AGENT50_TYPES_H
#define AGENT50_TYPES_H
typedef struct Entry198 { char data[198]; } Entry198;
typedef struct Tbl30C {
    char pad0[0x10];
    Entry198 entries[1];
} Tbl30C;
/* Packed resource header: counts at +8..+0x18 then sections starting at +0x20. */
typedef struct Pack0D188 {
    int w0, w4;
    int n0, n1, n2, n3, n4;
    int pad;
    char data[1];
} Pack0D188;
typedef struct { Pack0D188 *pack; } Ref0D188;
#endif
