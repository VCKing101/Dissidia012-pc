/*
 * replay_types.h - Dissidia 012 (USA ULUS10566) battle replay data layouts.
 *
 * Established on 26 November 2026 from the retail disassembly of main 0x0888948C (the task), 0x088CA680 (the
 * recorder), 0x088CA9A8 (the player), 0x0888AD50 (the save copy), 0x088C82E0 (the clear) and 0x088C96E8 (the
 * load copy), and checked against four REPLAY.BIN files of the port's test data, decrypted with the replay key.
 * See docs/research/replay-module.md for the functions that read and write each field.
 *
 * Marks in the field comments (as in text_types.h):
 *   V  verified in the disassembly (a load/store at that offset with that width, used that way)
 *   F  seen in the decrypted files (values that fit the meaning)
 *   I  inferred (name/meaning from how it is used, or offset known but use not fully traced)
 *
 * Conventions: guest memory (PSP, 32-bit little endian); unknown fields are unk_XXXX (hex offset); sizes are
 * asserted only where a copy, a stride or a clear gives them. Compiles as C and C++ under SNC and a host compiler.
 */
#ifndef D012_REPLAY_TYPES_H
#define D012_REPLAY_TYPES_H

#ifdef __SNC__
#define D012_REPLAY_OFFSETOF(type, field) ((unsigned int)&((type *)0)->field)
#else
#include <stddef.h>
#define D012_REPLAY_OFFSETOF(type, field) offsetof(type, field)
#endif

#ifndef D012_STATIC_ASSERT
#if defined(__cplusplus) && __cplusplus >= 201103L
#define D012_STATIC_ASSERT(cond, msg) static_assert(cond, msg)
#elif !defined(__cplusplus) && defined(__STDC_VERSION__) && __STDC_VERSION__ >= 201112L
#define D012_STATIC_ASSERT(cond, msg) _Static_assert(cond, msg)
#else
/* SNC 1.2 (C and C++) has neither: a typedef of a char array whose size is -1 when cond is false. */
#define D012_REPLAY_JOIN2(a, b) a##b
#define D012_REPLAY_JOIN(a, b) D012_REPLAY_JOIN2(a, b)
#define D012_STATIC_ASSERT(cond, msg) typedef char D012_REPLAY_JOIN(d012_replay_assert_, __LINE__)[(cond) ? 1 : -1]
#endif
#endif

#define D012_REPLAY_AT(type, field, offset) \
    D012_STATIC_ASSERT(D012_REPLAY_OFFSETOF(type, field) == (offset), #type "." #field)

#ifdef __cplusplus
extern "C" {
#endif

#define D012_REPLAY_PTR          0x089D7750u  /* V  Replay* (stored by 0x088769CC at 0x08877794; FIELD +0x7C8 copies it) */
#define D012_REPLAY_STATIC       0x08B99030u  /* V  the one replay object (guard word 0x08B9902C, built by 0x088C9564) */
#define D012_REPLAY_MAGIC        0x20081106u  /* V F  written to +0x0000 by the clear 0x088C82E0 */
#define D012_REPLAY_SLOTS        0x1518       /* V  5,400 records a fighter */
#define D012_REPLAY_CAMERA_SLOTS 200          /* V */
#define D012_REPLAY_MASKS        0x089D7758u  /* V  15 entries of {u32 lo, u32 hi}: the pad bits recorded */
#define D012_REPLAY_KEY          0x089CB708u  /* V  the 16-byte save key REPLAY.BIN is encrypted with */
#define D012_REPLAY_FILE_SIZE    0xBEA0       /* V F  REPLAY.BIN's plaintext */
#define D012_REPLAY_FILE_OFFSET  0xE8         /* V F  file offset of replay +0x0000 */

/* State at +0xBDB8. */
enum {
    D012_REPLAY_RECORD_START = 0, /* V  the recorder's first frame: set to 1, clock cleared; the player's: set to 2 */
    D012_REPLAY_RECORDING = 1,    /* V */
    D012_REPLAY_PLAYING = 2,      /* V */
    D012_REPLAY_STOP_3 = 3,       /* V  the recorder does nothing (I: stopped) */
    D012_REPLAY_PAUSE_4 = 4,      /* V  the player sets 5 */
    D012_REPLAY_PAUSED_5 = 5      /* V  the player does nothing; 6 likewise */
};

/* Flags at +0xBDE8. */
#define D012_REPLAY_FULL     0x1u  /* V  a record or camera table filled; the ending then does not offer Save Replay */
#define D012_REPLAY_CAMERA   0x2u  /* V  set by the menu's hand-off 0x08889BAC (third argument); the player then
                                    *    drives the camera functions 0x088FF7CC/0x088FF868/0x088FF8F8 (I: plays the
                                    *    camera data) and the task starts the music after FIELD +0x7C5 counts down */

/* A camera entry, 0x24 bytes (copied field by field by 0x0888AD50 and 0x088C96E8). */
typedef struct D012ReplayCamera {
    float pos_a[3];               /* +0x00 V  a vector (zero when recorded; I: filled by Camera Edit) */
    float pos_b[3];               /* +0x0C V  a vector (zero when recorded) */
    int frame;                    /* +0x18 V F  the recorder's clock when the entry was made */
    short unk_1C;                 /* +0x1C V */
    short unk_1E;                 /* +0x1E V */
    unsigned char unk_20;         /* +0x20 V */
    unsigned char unk_21[3];      /* +0x21 V */
} D012ReplayCamera;

typedef struct D012Replay {
    /* Saved: REPLAY.BIN holds +0x0000..+0xBDB8 at file offset 0xE8. */
    unsigned int magic;           /* +0x0000 V F  0x20081106 */
    unsigned char sides[2][0x1060]; /* +0x0004 V  two 0x1060-byte objects (array built by 0x088C829C);
                                   *         I: the per-side battle set-ups (stored by 0x088FB1A4, applied by 0x088FB028) */
    unsigned char unk_20C4[0x220]; /* +0x20C4 I  the rest of the battle's set-up */
    int bgm;                      /* +0x22E4 V F  the battle music (FIELD +0x804 in playback; 83 and 84 seen) */
    unsigned char records[2][0x3F48]; /* +0x22E8 V F  5,400 records of 3 bytes a fighter: big-endian (mask << 8) | run */
    int record_count[2];          /* +0xA178 V F */
    D012ReplayCamera camera[200]; /* +0xA180 V F  filled from the end: entry 199 - camera_count */
    int unk_BDA0;                 /* +0xBDA0 V  cleared, copied */
    int camera_count;             /* +0xBDA4 V F */
    int unk_BDA8;                 /* +0xBDA8 V  cleared, copied */
    int frame_total;              /* +0xBDAC V F  the recorder's clock: frames recorded (= the sum of the runs) */
    unsigned char unk_BDB0;       /* +0xBDB0 V  flipped by 0x088CA510 */
    unsigned char pad_BDB1[3];
    int player_side;              /* +0xBDB4 V  FIELD +0x54 when a battle is recorded (0x088C9928) */
    /* Not saved. */
    int state;                    /* +0xBDB8 V */
    int rec_last[2];              /* +0xBDBC V  the mask of the run being counted */
    int rec_run[2];               /* +0xBDC4 V  its length so far, 1..255 */
    int play_mask[2];             /* +0xBDCC V  the mask being played */
    int play_left[2];             /* +0xBDD4 V  frames left in that run */
    int play_index[2];            /* +0xBDDC V  the next record to read */
    int play_frame;               /* +0xBDE4 V  frames played; the camera data is looked up by it (0x088CA540) */
    unsigned int flags;           /* +0xBDE8 V */
} D012Replay;

D012_REPLAY_AT(D012ReplayCamera, frame, 0x18);
D012_STATIC_ASSERT(sizeof(D012ReplayCamera) == 0x24, "D012ReplayCamera size");
D012_REPLAY_AT(D012Replay, sides, 0x0004);
D012_REPLAY_AT(D012Replay, bgm, 0x22E4);
D012_REPLAY_AT(D012Replay, records, 0x22E8);
D012_REPLAY_AT(D012Replay, record_count, 0xA178);
D012_REPLAY_AT(D012Replay, camera, 0xA180);
D012_REPLAY_AT(D012Replay, camera_count, 0xBDA4);
D012_REPLAY_AT(D012Replay, frame_total, 0xBDAC);
D012_REPLAY_AT(D012Replay, player_side, 0xBDB4);
D012_REPLAY_AT(D012Replay, state, 0xBDB8);
D012_REPLAY_AT(D012Replay, play_mask, 0xBDCC);
D012_REPLAY_AT(D012Replay, play_frame, 0xBDE4);
D012_REPLAY_AT(D012Replay, flags, 0xBDE8);
D012_STATIC_ASSERT(D012_REPLAY_FILE_OFFSET + D012_REPLAY_OFFSETOF(D012Replay, state) == D012_REPLAY_FILE_SIZE,
                   "REPLAY.BIN ends where the unsaved fields start");

/* One save request's object (60,000 bytes for a replay; FIELD +0x7CC). Only what the replay code touches. */
typedef struct D012ReplaySave {
    unsigned char unk_0000[0x7E4];
    unsigned char params[0x600];  /* +0x07E4 V  the sceUtilitySavedata parameters (cleared by 0x088B04AC);
                                   *         +0x814 is their mode (4 LISTLOAD, 5 LISTSAVE, 8 SIZES) */
    unsigned char unk_0DE4[0x1DA0];
    int enabled;                  /* +0x2B84 V  1; also the file's first word (0x088AEFA0 writes, 0x088AEF54 checks) */
    int size;                     /* +0x2B88 V  0xBEA0; also the file's second word */
    const char *file;             /* +0x2B8C V  "REPLAY.BIN" (0x089CB718) */
    const char *title;            /* +0x2B90 V  "Replay" (0x089CB724): the folder name's stem */
    const unsigned char *key;     /* +0x2B94 V  the save key (0x089CB708), copied into the parameters' key */
    const char *icon;             /* +0x2B98 V  "save/JP/ICON1.PNG" */
    const char *icon_new;         /* +0x2B9C V  "save/JP/ICON1_NEW.PNG" */
    void (*callback)(void);       /* +0x2BA0 V  0x088B0CAC: builds the save's texts, then LISTSAVE */
    int unk_2BA4;
    unsigned char buffer[0xBEA0]; /* +0x2BA8 V  what REPLAY.BIN holds */
    int detail_args[6];           /* +0xEA48 V  the detail text's inputs (0x088B0BA8 stores, 0x088B0CC8 reads): the
                                   *         player side's fighter id (1..0x20) and its variant (0..2), the other
                                   *         side's two, the stage index and the length in seconds */
} D012ReplaySave;

D012_REPLAY_AT(D012ReplaySave, enabled, 0x2B84);
D012_REPLAY_AT(D012ReplaySave, key, 0x2B94);
D012_REPLAY_AT(D012ReplaySave, buffer, 0x2BA8);
D012_REPLAY_AT(D012ReplaySave, detail_args, 0xEA48);

#ifdef __cplusplus
}
#endif

#endif /* D012_REPLAY_TYPES_H */
