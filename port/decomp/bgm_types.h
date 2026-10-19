/* The battle music (BGM) subsystem of Dissidia 012 (ULUS10566 v1.00):
 * structures recovered from the code and data, and the addresses the
 * decompiled functions use. Every address is tagged with its module (main,
 * or the battle overlay loaded at 0x089DD4C0). See
 * docs/port/bgm-playlist-design.md for how the pieces fit together.
 *
 * MIT licence; see port/README.md. */
#ifndef D012_DECOMP_BGM_TYPES_H
#define D012_DECOMP_BGM_TYPES_H

#include "guest.h"

#ifdef __cplusplus
extern "C" {
#endif

/* ---- The track table ------------------------------------------------------
 * bgm_entry.bin (in general_archive/main/main.bin, "prologus_bgm_entry.bin"
 * in the Prologus build) is a BgmFile header and `count` rows of BgmEntry;
 * the retail file has 131 rows (version word 0x20100831). main 0x0889EE60
 * copies it into a buffer with room for 256 rows in all, so that DLC rows can
 * be appended (main 0x0889F040, from ms0 dlc_000..019.bin, same layout). */
typedef struct BgmFile {
    char magic[4];         /* 0x00 "BENT" */
    uint32_t version;      /* 0x04 */
    int32_t count;         /* 0x08 rows in use, disc + DLC */
    uint32_t pad0C;
} BgmFile;

typedef struct BgmEntry {
    int16_t id;            /* 0x00 the BGM id everything else uses (0..130 on the disc) */
    int16_t name_text;     /* 0x02 string number in bgm_name.bin */
    int16_t unlock[4];     /* 0x04 save flags that must all be set (-1: none); PP Catalog purchases */
    uint8_t flags;         /* 0x0C bit 0 and bit 1: battle-theme groups the random choice and the
                            *      Museum use; bit 2: a DLC track, streamed from an ms0 EDAT file */
    int8_t volume;         /* 0x0D percent */
    int8_t weight;         /* 0x0E weight in the random choice; 0: never offered in battle */
    uint8_t pad0F;
    uint32_t stage_mask[2];/* 0x10 a bit per stage it is drawn for at random */
    uint32_t mask18;       /* 0x18 a further bit set the random choice tests */
    char file[16];         /* 0x1C file name, NUL-terminated (at most 15 characters) */
} BgmEntry;                /* 0x2C */

/* A DLC row's name (main 0x0889F040, 0x0889F18C). */
typedef struct BgmDlcName {
    int16_t dlc_slot;      /* 0x00 */
    int16_t row;           /* 0x02 */
    uint8_t text[0x80];    /* 0x04 the name, in the game's text encoding */
} BgmDlcName;              /* 0x84 */

/* The table object, *(main 0x089D6C30); built by main 0x0889F4F4. */
typedef struct BgmTable {
    gaddr buffer;          /* 0x00 allocation holding the file */
    gaddr file;            /* 0x04 BgmFile * */
    gaddr entries;         /* 0x08 BgmEntry * (file + 0x10) */
    uint8_t names[0x10];   /* 0x0C text table of bgm_name.bin */
    uint8_t player[0x24];  /* 0x1C in-memory player for short pieces (victory themes) */
    int32_t disc_count;    /* 0x40 rows from bgm_entry.bin; DLC rows follow */
    gaddr dlc_names;       /* 0x44 BgmDlcName[256 - disc_count] */
} BgmTable;                /* 0x48 */

#define BGM_TABLE_CAPACITY 256

/* ---- The loading screen's BGM selector -------------------------------------
 * Inside the loading screen (battle 0x08AF5C88 builds it) at +0x6AC. The
 * screen also holds the mode (+0xF54, from the battle rules byte at main
 * 0x09AFDE50: no selector for 0 and 7), the BGM id it starts on (+0xF60) and
 * whether input is taken (+0xF6D). */
typedef struct BgmSelector {
    gaddr layout;          /* 0x000 layout element 0: anchor 0 holds the name, 1 and 2 the arrows;
                            *       keeps a list of the sprites drawn in it */
    gaddr arrow_left;      /* 0x004 layout element: arrow, animation 2 when pressed, 1 at rest */
    gaddr arrow_right;     /* 0x008 layout element: arrow, animation 4 when pressed, 3 at rest */
    uint32_t pad0C[2];
    gaddr text;            /* 0x014 the track name, a text sprite */
    gaddr entry[256];      /* 0x018 BgmEntry * per choice */
    gaddr name[256];       /* 0x418 name string per choice */
    int32_t count;         /* 0x818 */
    int32_t index;         /* 0x81C the choice shown */
} BgmSelector;             /* 0x820 */

#define BGM_SELECTOR_MAX 256

/* Layout element (the game's 2D UI parts): the fields the selector touches. */
#define LAYOUT_IDLE_ANIMATION 0x0E   /* s16: animation played when the current one ends */
#define LAYOUT_SPRITE_HEAD 0x30      /* first sprite drawn with the element */
#define LAYOUT_SPRITE_TAIL 0x34
#define LAYOUT_SPRITE_COUNT 0x38
/* Sprite (text or picture): list links and flags. */
#define SPRITE_FLAGS 0x10
#define SPRITE_IN_LIST 0x8000u
#define SPRITE_NEXT 0x4C
#define SPRITE_PREV 0x54

/* ---- Globals ---------------------------------------------------------------- */
#define G_BGM_TABLE 0x089D6C30u        /* main: BgmTable * */
#define G_SAVE_FLAGS 0x089D726Cu       /* main: u32 * to the save's 4096+ flag bits (save root + 0x1DF88) */
#define G_TEXT_SYSTEM 0x089D6998u      /* main: text system object */
#define G_SOUND 0x089D6C88u            /* main: sound manager object */
/* The streamed-music player (main 0x0889FC90 .. 0x088A0734). */
#define G_STREAM_BUFFER 0x089D6C38u    /* u8 * , 0x18002 bytes allocated, 0x18000 used */
#define G_STREAM_FD 0x089D6C3Cu        /* file handle, -1 */
#define G_STREAM_SLOT 0x089D6C40u      /* sound slot playing, -1 */
#define G_STREAM_VOLUME 0x089D6C44u    /* float */
#define G_STREAM_FADE 0x089D6C48u      /* float, volume step a frame when fading out */
#define G_STREAM_FADING 0x089D6C4Cu    /* u8 */
#define G_STREAM_STATE 0x089D6C50u     /* s32: 0 idle, 1 open queued, 2 opening, 3 reading, 4 playing,
                                        *      5 stopping, 6 resume after sleep, 7 suspended */
#define G_STREAM_LOCKED 0x089D6C54u    /* u8: 1 = a caller holds the buffer (main 0x088A0128) */
#define G_STREAM_STOPPED 0x089D6C60u   /* s32 */
#define G_STREAM_REQUEST 0x09B4CEF0u   /* char name[0x80], then +0x80 slot, +0x84 float volume, +0x88 u8 from_ms0 */
#define STREAM_BUFFER_BYTES 0x18000u

/* Battle overlay data: the pad masks the selector tests, {lo, hi} each. */
#define G_MASK_DPAD_RIGHT 0x08AFBBC8u  /* {0x80, 0}: logical D-pad right */
#define G_MASK_DPAD_LEFT 0x08AFBBD0u   /* {0x40, 0}: logical D-pad left */

/* The pad module's key repeat (main 0x08871BD0): a held button repeats when
 * a timer, 30 at the press and 4 after each repeat, has counted down by the
 * frame step (at least 2) once a pad update. */
#define G_PAD_FRAME_STEP 0x089D5EACu   /* main: float */
#define PAD_REPEAT_DELAY 30.0f
#define PAD_REPEAT_INTERVAL 4.0f
/* SceCtrlData buttons the loading screen's page list scrolls with (raw; the
 * logical pad has them as 0x10 and 0x20). */
#define RAW_DPAD_UP 0x0010u
#define RAW_DPAD_DOWN 0x0040u

/* main 0x089CAB94 "DISC0:/PSP_GAME/USRDIR/DATA/BGM/%s" */
#define G_FMT_DISC_BGM 0x089CAB94u

/* ---- Functions the decompiled code calls ------------------------------------ */
#define F_PAD_LOGICAL 0x088729A0u      /* main: (player) -> pad; vtable at +0x34, slot +0x20 = is-repeat(mask) */
#define PAD_VTABLE 0x34u
#define PAD_SLOT_IS_REPEAT 0x20u
#define F_LAYOUT_PLAY 0x088D9F5Cu      /* main: (element, animation, 1) */
#define F_LAYOUT_ANCHOR 0x088DA0A8u    /* main: (element, anchor, out position, out size) */
#define F_LAYOUT_SET_POS 0x088D9B84u   /* main: (element, position) */
#define F_LAYOUT_SET_SIZE 0x088D9AF4u  /* main: (element, size) */
#define F_SE_PLAY 0x088A24D4u          /* main: (0, 10000, 1, 0): the cursor sound */
#define F_TEXT_CREATE 0x0887D868u      /* main: (text system, 0, string, -1, 0, 0, 0, 0) -> sprite */
#define F_SPRITE_SHOW 0x0887B924u      /* main: (sprite) */
#define F_SPRITE_FINISH 0x0887B944u    /* main: (sprite) */
#define F_SPRITE_DELETE 0x0887BA5Cu    /* main: (sprite) */
#define F_SPRITE_ON_END 0x0887BA80u    /* main: (sprite, callback, argument) */
#define F_SPRITE_SET_POS 0x0887B778u   /* main: (sprite, position) */
#define F_SPRITE_SET_SIZE 0x0887B828u  /* main: (sprite, size) */
#define F_SPRITE_UPDATE 0x0887B9C4u    /* main: (sprite) */
#define F_SPRITE_DRAW 0x0887B98Cu      /* main: (sprite) */
#define F_SELECTOR_REMADE 0x08AF5C6Cu  /* battle: the callback that remakes the name sprite */
#define F_TEXT_STRING 0x0887AFBCu      /* main: (text table, n, 0) -> string */
#define F_SPRINTF 0x0893476Cu          /* main */
#define F_STRUPR 0x08935D58u           /* main */
#define F_STRCPY 0x089359A4u           /* main */
#define F_DLC_BGM_PATH 0x08862028u     /* main: (out, name): ms0:/PSP/GAME/<id>/<hash of "bgm/name">.edat */
#define F_FILE_OPEN 0x08862E08u        /* main: (path, from_ms0) -> handle */
#define F_FILE_READ 0x08862EE4u        /* main: (handle, buffer, size, from_ms0) -> ok */
#define F_FILE_CLOSE 0x0886300Cu       /* main: (handle) */
#define F_NPDRM_SETUP_KEY 0x089C5D50u  /* main import stub: sceNpDrmEdataSetupKey(handle) */
#define F_STREAM_STOP 0x0889FF8Cu      /* main: (fade frames) */
#define F_SOUND_ATRAC_SET 0x088A1380u  /* main: (sound, slot, buffer, size, streamed) -> 1 */
#define F_SOUND_PLAY 0x088A1400u       /* main: (sound, slot) -> 1 */
#define F_SOUND_VOLUME 0x088A14D0u     /* main: (f12 volume, sound, slot, 0, 0) */

/* ---- The decompiled functions ----------------------------------------------- */
/* main 0x0889F5E8: row of `id`, or -1. */
int32_t bgm_table_find_row(Guest *g, gaddr table, uint32_t id);
/* main 0x0889F3E4: BgmEntry * of `id`, or 0. */
gaddr bgm_table_find(Guest *g, gaddr table, uint32_t id);
/* main 0x0889F434: name of row `row` (disc rows from bgm_name.bin, DLC rows their own). */
gaddr bgm_table_name(Guest *g, gaddr table, int32_t row);
/* main 0x0889F4AC: name of `id`, or 0. */
gaddr bgm_table_name_of(Guest *g, gaddr table, uint32_t id);
/* main 0x0889F234: plays `id` as the streamed BGM; 1 when it started. Ends
 * with a tail call to bgm_stream_start (main 0x0889FD38). */
uint32_t bgm_play_id(Guest *g, gaddr table, uint32_t id);
/* main 0x0889F534: plays `entry` (a BgmEntry *); as bgm_play_id. Returns 0
 * itself, or leaves a tail call pending. */
uint32_t bgm_play_entry(Guest *g, gaddr table, gaddr entry);
/* main 0x0889FD38: opens and starts streaming `name`; 1 when it started. */
uint32_t bgm_stream_start(Guest *g, float volume, uint32_t slot, gaddr name, uint32_t async_open,
                          uint32_t from_ms0);
/* main 0x088A04A8: the path the stream opens; returns `out`. */
gaddr bgm_stream_path(Guest *g, gaddr out, gaddr name, uint32_t from_ms0);

/* battle 0x08ADE4E0: the choices the selector offers: every row with a
 * non-zero weight whose unlock flags are all set, in table order, at most
 * `max`. Fills entries[] and names[]; returns the count. */
int32_t bgm_list_offered(Guest *g, gaddr unused, gaddr entries, gaddr names, int32_t max);
/* battle 0x08AF8CA4: one frame of the selector: D-pad left/right when
 * `take_input` is 1, then places the name and the arrows. */
void bgm_selector_update(Guest *g, gaddr selector, uint32_t take_input);
/* battle 0x08AF5AE8: makes the name sprite of the current choice. */
void bgm_selector_make_name(Guest *g, gaddr selector);
/* battle 0x08AF5AC0: the BGM id chosen, or -1. */
int32_t bgm_selector_chosen(Guest *g, gaddr selector);
/* Part of battle 0x08AF8CA4, after the choice changed: makes the name sprite
 * when there is none, else retires the old one, whose end callback (battle
 * 0x08AF5C6C) makes the new one. */
void bgm_selector_name_changed(Guest *g, gaddr selector);

#ifdef __cplusplus
}
#endif

#endif
