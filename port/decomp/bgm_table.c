/* The BGM table: lookups and the start of playback (main 0x0889F234 ..
 * 0x0889F638, 0x0889FD38, 0x088A04A8). Decompiled from ULUS10566 v1.00;
 * behaviourally equivalent, not matching. MIT licence; see port/README.md. */
#include "bgm_types.h"

static BgmTable *table_of(Guest *g, gaddr table) { return G_STRUCT(g, BgmTable, table); }
static int32_t row_count(Guest *g, gaddr table) {
    return G_STRUCT(g, BgmFile, table_of(g, table)->file)->count;
}

/* main 0x0889F5E8 */
int32_t bgm_table_find_row(Guest *g, gaddr table, uint32_t id) {
    const int32_t count = row_count(g, table);
    gaddr entry = table_of(g, table)->entries;
    for (int32_t row = 0; row < count; ++row, entry += sizeof(BgmEntry)) {
        /* lh, compared with the whole register the caller passed */
        if ((uint32_t)(int32_t)G_STRUCT(g, BgmEntry, entry)->id == id) return row;
    }
    return -1;
}

/* main 0x0889F3E4 */
gaddr bgm_table_find(Guest *g, gaddr table, uint32_t id) {
    const int32_t row = bgm_table_find_row(g, table, id);
    if (row < 0) return 0;
    return table_of(g, table)->entries + (uint32_t)row * (uint32_t)sizeof(BgmEntry);
}

/* main 0x0889F434 */
gaddr bgm_table_name(Guest *g, gaddr table, int32_t row) {
    const BgmTable *t = table_of(g, table);
    if (row < t->disc_count) {
        const gaddr entry = t->entries + (uint32_t)row * (uint32_t)sizeof(BgmEntry);
        if (entry == 0) return 0;
        const int16_t text = G_STRUCT(g, BgmEntry, entry)->name_text;
        g_push(g, 0x10u);  /* the original's frame: the callee sees the same $sp */
        const gaddr name = g_call3(g, F_TEXT_STRING, table + 0x0Cu, (uint32_t)(int32_t)text, 0);
        g_pop(g, 0x10u);
        return name;
    }
    return t->dlc_names + (uint32_t)(row - t->disc_count) * (uint32_t)sizeof(BgmDlcName) + 4u;
}

/* main 0x0889F4AC */
gaddr bgm_table_name_of(Guest *g, gaddr table, uint32_t id) {
    const int32_t row = bgm_table_find_row(g, table, id);
    if (row < 0) return 0;
    g_push(g, 0x10u);
    const gaddr name = bgm_table_name(g, table, row);
    g_pop(g, 0x10u);
    return name;
}

/* main 0x0889F534. The original calls main 0x0889FD38 and, when it answers
 * 1, stores the volume in G_STREAM_VOLUME and answers 1, else 0. 0x0889FD38
 * answers only 0 or 1 and stores that same volume itself before answering 1,
 * so a tail call behaves the same and leaves the streaming start, which
 * reads the disc, to run as ordinary guest code. */
uint32_t bgm_play_entry(Guest *g, gaddr table, gaddr entry) {
    (void)table;
    if (entry == 0) return 0;
    const BgmEntry *e = G_STRUCT(g, BgmEntry, entry);
    if (e->file[0] == '\0') return 0; /* strlen (main 0x089359E8) == 0 */
    uint32_t hundredth_bits = 0x3C23D70Au;  /* 0.01f */
    float hundredth;
    memcpy(&hundredth, &hundredth_bits, sizeof hundredth);
    const float volume = (float)e->volume * hundredth;
    GuestRegs r;
    memset(&r, 0, sizeof r);
    r.f[0] = volume;
    r.a[0] = 0;                      /* sound slot 0 */
    r.a[1] = entry + 0x1Cu;          /* file name */
    r.a[2] = 0;                      /* open now */
    r.a[3] = (e->flags & 4u) != 0u;  /* a DLC track on ms0 */
    g_tail(g, 0x0889FD38u, &r);
    return 0;
}

/* main 0x0889F234 */
uint32_t bgm_play_id(Guest *g, gaddr table, uint32_t id) {
    return bgm_play_entry(g, table, bgm_table_find(g, table, id));
}

/* main 0x088A04A8 */
gaddr bgm_stream_path(Guest *g, gaddr out, gaddr name, uint32_t from_ms0) {
    g_push(g, 0x10u);
    if ((from_ms0 & 0xFFu) == 1u) {
        g_call2(g, F_DLC_BGM_PATH, out, name);
    } else {
        g_call3(g, F_SPRINTF, out, G_FMT_DISC_BGM, name);
    }
    const gaddr result = g_call1(g, F_STRUPR, out);
    g_pop(g, 0x10u);
    return result;
}

static void stream_forget(Guest *g) {
    g_st32(g, G_STREAM_FD, 0xFFFFFFFFu);
    g_st32(g, G_STREAM_SLOT, 0xFFFFFFFFu);
}

static void stream_close(Guest *g) {
    g_call1(g, F_FILE_CLOSE, g_ld32(g, G_STREAM_FD));
    stream_forget(g);
}

/* main 0x0889FD38 */
uint32_t bgm_stream_start(Guest *g, float volume, uint32_t slot, gaddr name, uint32_t async_open,
                          uint32_t from_ms0) {
    const uint32_t ms0 = from_ms0 & 0xFFu;
    const uint32_t queued = async_open & 0xFFu;
    if (g_ld8(g, G_STREAM_LOCKED) == 1u) return 0;

    const gaddr frame = g_push(g, 0x120u);  /* +0x000 the path, +0x100 saved arguments */
    g_st8(g, frame + 0x100u, (uint8_t)queued);
    g_st32(g, frame + 0x104u, name);
    uint32_t result = 0;

    g_call1(g, F_STREAM_STOP, 0);
    g_st32(g, G_STREAM_STOPPED, 0);
    g_call2(g, F_STRCPY, G_STREAM_REQUEST, name);
    g_st32(g, G_STREAM_REQUEST + 0x80u, slot);
    g_stf(g, G_STREAM_REQUEST + 0x84u, volume);
    g_st8(g, G_STREAM_REQUEST + 0x88u, (uint8_t)ms0);

    if (queued == 1u) {
        /* The stream thread opens it (state 1, main 0x088A0170). */
        g_st32(g, G_STREAM_STATE, 1);
        result = 1;
        goto done;
    }

    bgm_stream_path(g, frame, name, ms0);
    const int32_t fd = (int32_t)g_call2(g, F_FILE_OPEN, frame, ms0);
    g_st32(g, G_STREAM_FD, (uint32_t)fd);
    if (fd < 0) {
        stream_forget(g);
        goto done;
    }
    if (ms0 == 1u && (int32_t)g_call1(g, F_NPDRM_SETUP_KEY, g_ld32(g, G_STREAM_FD)) < 0) {
        stream_close(g);
        goto done;
    }
    /* The first 0x18000 bytes: the RIFF header, with the loop points, and
     * the start of the data. The rest comes in through the stream thread. */
    if (g_call4(g, F_FILE_READ, g_ld32(g, G_STREAM_FD), g_ld32(g, G_STREAM_BUFFER), STREAM_BUFFER_BYTES, ms0) == 0) {
        stream_close(g);
        goto done;
    }
    g_st32(g, G_STREAM_SLOT, slot);
    g_st32(g, G_STREAM_STATE, 4);
    if (g_call5(g, F_SOUND_ATRAC_SET, g_ld32(g, G_SOUND), slot, g_ld32(g, G_STREAM_BUFFER), STREAM_BUFFER_BYTES,
                1) != 1u ||
        g_call2(g, F_SOUND_PLAY, g_ld32(g, G_SOUND), g_ld32(g, G_STREAM_SLOT)) != 1u) {
        stream_close(g);
        goto done;
    }
    g_stf(g, G_STREAM_VOLUME, volume);
    g_call_f4(g, F_SOUND_VOLUME, volume, g_ld32(g, G_SOUND), g_ld32(g, G_STREAM_SLOT), 0, 0);
    result = 1;
done:
    g_pop(g, 0x120u);
    return result;
}
