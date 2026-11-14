/* Screen0B6A0: resets the page cursor block (+0x2724..+0x27C8), the 0x80
   marks at +0x27CC and the 100-word seen bitmap at +0x2F40, marks the
   current entry (+0x272E) seen and clears flags 0x20000 and 0x40000.
   (Work0B6A0 in menu/auto_region1.h leaves these fields in padding.) */
typedef struct Mark27CC {
    unsigned char used;
    char pad_01;
    short id;
} Mark27CC;

typedef struct PageWork29 {
    char pad_0000[0x2724];
    unsigned int flags;
    short field_2728;
    short field_272A;
    short field_272C;
    unsigned short entry;   /* +0x272E */
    char pad_2730[0x4];
    unsigned short field_2734;
    unsigned short field_2736;
    unsigned short field_2738;
    char pad_273A[0xA];
    short field_2744;
    char pad_2746[0x56];
    unsigned char field_279C;
    unsigned char field_279D;
    short field_279E;
    short field_27A0;
    short field_27A2;
    int field_27A4;
    int field_27A8;
    int field_27AC;
    char pad_27B0[0x14];
    int field_27C4;
    int field_27C8;
    Mark27CC marks[0x80];
    char pad_29CC[0x574];
    unsigned int seen[100]; /* +0x2F40 */
} PageWork29;

typedef struct PageScreen29 {
    char pad_000[0x14C];
    PageWork29 *work;
} PageScreen29;

extern "C" void func_089F5460(PageScreen29 *self)
{
    unsigned int entry;
    unsigned int *seen;

    self->work->field_2734 = 0;
    self->work->field_2736 = 0;
    self->work->field_2738 = 0;
    self->work->field_2728 = 0;
    self->work->field_2744 = 0;
    self->work->field_272A = -1;
    self->work->field_272C = 0;
    self->work->field_279C = 0;
    self->work->field_27A0 = 0;
    self->work->field_279D = 0;
    self->work->field_279E = 0;
    self->work->field_27A2 = 0;
    self->work->field_27A4 = 0;
    self->work->field_27A8 = 0;
    self->work->field_27AC = 0;
    self->work->field_27C4 = 0;
    self->work->field_27C8 = 0;
    for (int i = 0; i < 0x80; i++) {
        self->work->marks[i].used = 0;
        self->work->marks[i].id = -1;
    }
    seen = self->work->seen;
    for (int i = 0; i < 100; i++) {
        seen[i] = 0;
    }
    entry = self->work->entry;
    seen = self->work->seen;
    seen[entry >> 5] |= 1 << (entry & 0x1F);
    self->work->flags &= ~0x60000;
}
