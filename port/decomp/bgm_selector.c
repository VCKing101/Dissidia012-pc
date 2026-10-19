/* The loading screen's BGM selector (battle overlay 0x08ADE4E0,
 * 0x08AF5AC0, 0x08AF5AE8, 0x08AF8CA4). Decompiled from ULUS10566 v1.00;
 * behaviourally equivalent, not matching. MIT licence; see port/README.md. */
#include "bgm_types.h"

/* battle 0x08ADE4E0 */
int32_t bgm_list_offered(Guest *g, gaddr unused, gaddr entries, gaddr names, int32_t max) {
    (void)unused;
    g_push(g, 0x30u);  /* the original's frame: the callee sees the same $sp */
    const gaddr table0 = g_ld32(g, G_BGM_TABLE);
    const int32_t count = G_STRUCT(g, BgmFile, G_STRUCT(g, BgmTable, table0)->file)->count;
    int32_t offered = 0;
    for (int32_t row = 0; row < count; ++row) {
        const gaddr table = g_ld32(g, G_BGM_TABLE);
        const gaddr entry = G_STRUCT(g, BgmTable, table)->entries + (uint32_t)row * (uint32_t)sizeof(BgmEntry);
        const gaddr name = g_call2(g, 0x0889F434u, table, (uint32_t)row);  /* bgm_table_name */
        const BgmEntry *e = G_STRUCT(g, BgmEntry, entry);
        if (e->weight == 0) continue;
        const gaddr flags = g_ld32(g, G_SAVE_FLAGS);
        int unlocked = 1;
        for (int k = 0; k < 4; ++k) {
            const int16_t flag = e->unlock[k];
            if (flag < 0) continue;
            const uint32_t word = g_ld32(g, flags + ((uint32_t)flag >> 5) * 4u);
            if ((word & (1u << ((uint32_t)flag & 31u))) == 0) {
                unlocked = 0;
                break;
            }
        }
        if (!unlocked) continue;
        if (offered >= max) break;
        g_st32(g, entries + (uint32_t)offered * 4u, entry);
        g_st32(g, names + (uint32_t)offered * 4u, name);
        ++offered;
    }
    g_pop(g, 0x30u);
    return offered;
}

/* battle 0x08AF5AC0 */
int32_t bgm_selector_chosen(Guest *g, gaddr selector) {
    const BgmSelector *s = G_STRUCT(g, BgmSelector, selector);
    const gaddr entry = s->entry[s->index];
    if (entry == 0) return -1;
    return G_STRUCT(g, BgmEntry, entry)->id;
}

/* Takes the name sprite out of the layout element's list (inlined at
 * battle 0x08AF5B1C and 0x08AF8DF8). */
static void unlink_sprite(Guest *g, gaddr owner, gaddr sprite) {
    g_st32(g, sprite + SPRITE_FLAGS, g_ld32(g, sprite + SPRITE_FLAGS) & ~SPRITE_IN_LIST);
    const gaddr prev = g_ld32(g, sprite + SPRITE_PREV);
    const gaddr next = g_ld32(g, sprite + SPRITE_NEXT);
    if (prev == 0) g_st32(g, owner + LAYOUT_SPRITE_HEAD, next);
    else g_st32(g, prev + SPRITE_NEXT, next);
    if (next == 0) g_st32(g, owner + LAYOUT_SPRITE_TAIL, prev);
    else g_st32(g, next + SPRITE_PREV, prev);
    g_st32(g, sprite + SPRITE_PREV, 0);
    g_st32(g, sprite + SPRITE_NEXT, 0);
    g_st32(g, owner + LAYOUT_SPRITE_COUNT, g_ld32(g, owner + LAYOUT_SPRITE_COUNT) - 1u);
}

/* battle 0x08AF5AE8 */
void bgm_selector_make_name(Guest *g, gaddr selector) {
    BgmSelector *s = G_STRUCT(g, BgmSelector, selector);
    if (s->name[s->index] == 0) return;
    g_push(g, 0x10u);  /* the original's frame */
    if (s->text != 0) {
        unlink_sprite(g, s->layout, s->text);
        g_call1(g, F_SPRITE_FINISH, s->text);
        g_call1(g, F_SPRITE_DELETE, s->text);
        s->text = 0;
    }
    s->text = g_call8(g, F_TEXT_CREATE, g_ld32(g, G_TEXT_SYSTEM), 0, s->name[s->index], 0xFFFFFFFFu, 0, 0, 0, 0);
    if (s->text != 0) g_call1(g, F_SPRITE_SHOW, s->text);
    g_pop(g, 0x10u);
    if (s->text == 0) return;
    const gaddr owner = s->layout;
    const gaddr sprite = s->text;
    if (owner == 0 || sprite == 0) return;
    /* Appended to the element's list, so it is drawn with it. */
    g_st32(g, sprite + SPRITE_FLAGS, g_ld32(g, sprite + SPRITE_FLAGS) | SPRITE_IN_LIST);
    const gaddr tail = g_ld32(g, owner + LAYOUT_SPRITE_TAIL);
    if (tail == 0) {
        g_st32(g, owner + LAYOUT_SPRITE_TAIL, sprite);
        g_st32(g, owner + LAYOUT_SPRITE_HEAD, sprite);
        g_st32(g, sprite + SPRITE_NEXT, 0);
        g_st32(g, sprite + SPRITE_PREV, 0);
    } else {
        g_st32(g, tail + SPRITE_NEXT, sprite);
        g_st32(g, sprite + SPRITE_NEXT, 0);
        g_st32(g, sprite + SPRITE_PREV, g_ld32(g, owner + LAYOUT_SPRITE_TAIL));
        g_st32(g, owner + LAYOUT_SPRITE_TAIL, sprite);
    }
    g_st32(g, owner + LAYOUT_SPRITE_COUNT, g_ld32(g, owner + LAYOUT_SPRITE_COUNT) + 1u);
}

/* Part of battle 0x08AF8CA4. */
void bgm_selector_name_changed(Guest *g, gaddr selector) {
    BgmSelector *s = G_STRUCT(g, BgmSelector, selector);
    if (s->text == 0) {
        bgm_selector_make_name(g, selector);
        return;
    }
    /* The old name goes; its end callback (battle 0x08AF5C6C) makes the new
     * one. */
    unlink_sprite(g, s->layout, s->text);
    g_call3(g, F_SPRITE_ON_END, s->text, F_SELECTOR_REMADE, selector);
    g_call1(g, F_SPRITE_FINISH, s->text);
    g_call1(g, F_SPRITE_DELETE, s->text);
    s->text = 0;
}

static int pad_repeats(Guest *g, gaddr pad, gaddr mask) {
    return g_vcall_u64(g, pad, PAD_VTABLE, PAD_SLOT_IS_REPEAT, g_ld32(g, mask), g_ld32(g, mask + 4u)) != 0;
}

static void press_arrow(Guest *g, gaddr arrow, uint32_t pressed, uint16_t rest) {
    g_call3(g, F_LAYOUT_PLAY, arrow, pressed, 1);
    g_st16(g, arrow + LAYOUT_IDLE_ANIMATION, rest);
    g_call4(g, F_SE_PLAY, 0, 10000, 1, 0);
}

/* battle 0x08AF8CA4 */
void bgm_selector_update(Guest *g, gaddr selector, uint32_t take_input) {
    const gaddr frame = g_push(g, 0x60u);
    const gaddr position = frame;          /* float[4] */
    const gaddr size = frame + 0x40u;      /* float[4] */
    BgmSelector *s = G_STRUCT(g, BgmSelector, selector);

    if ((take_input & 0xFFu) == 1u) {
        const gaddr pad = g_call1(g, F_PAD_LOGICAL, 0);
        const int32_t before = s->index;
        int32_t step = 0;
        if (pad_repeats(g, pad, G_MASK_DPAD_RIGHT)) {
            press_arrow(g, s->arrow_right, 4, 3);
            step = 1;
        } else if (pad_repeats(g, pad, G_MASK_DPAD_LEFT)) {
            press_arrow(g, s->arrow_left, 2, 1);
            step = -1;
        }
        /* Wraps around both ways. */
        s->index = g_rem(s->index + s->count + step, s->count);
        if (s->index != before) bgm_selector_name_changed(g, selector);
    }

    if (s->text != 0) {
        g_call4(g, F_LAYOUT_ANCHOR, s->layout, 0, position, size);
        g_call2(g, F_SPRITE_SET_POS, s->text, position);
        g_call2(g, F_SPRITE_SET_SIZE, s->text, size);
        g_call1(g, F_SPRITE_UPDATE, s->text);
        g_call1(g, F_SPRITE_DRAW, s->text);
    }
    g_call4(g, F_LAYOUT_ANCHOR, s->layout, 1, position, size);
    g_call2(g, F_LAYOUT_SET_POS, s->arrow_left, position);
    g_call2(g, F_LAYOUT_SET_SIZE, s->arrow_left, size);
    g_call4(g, F_LAYOUT_ANCHOR, s->layout, 2, position, size);
    g_call2(g, F_LAYOUT_SET_POS, s->arrow_right, position);
    g_call2(g, F_LAYOUT_SET_SIZE, s->arrow_right, size);
    g_pop(g, 0x60u);
}
