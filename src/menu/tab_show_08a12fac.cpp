/* Screen15A88: shows or hides groups of the board screen's models and
   texts. Standalone: the region header gives these a plain char flag,
   and retail reads it as unsigned. */
typedef struct TabWork29 {
    char pad_000[0x400];
    void *handles[0x120]; /* +0x400 */
    void *texts[0x58];    /* +0x880 */
    char pad_9E0[0x504];
    int field_EE4;
    char pad_EE8[0x8];
    unsigned int flags;   /* +0xEF0 */
} TabWork29;

typedef struct TabScreen29 {
    void *vtable;
    char pad_04[0xF5C];
    TabWork29 *work; /* +0xF60 */
} TabScreen29;

extern "C" {
void func_088D9CD8(void *model);
void func_088D9CFC(void *model);
void func_0887B924(void *text);
void func_0887B944(void *text);
}

/* The text pairs of the tabs (two texts each, from text 0x32). */
struct TabTexts29 {
    static void *Caption(TabWork29 *work, int pair) { return work->texts[0x33 + pair]; }
    static void *Shadow(TabWork29 *work, int pair) { return work->texts[0x32 + pair]; }
};

/* The five tab models (0x3F..) and the three tab captions. */
extern "C" void func_08A12FAC(TabScreen29 *self, unsigned char show)
{
    int i;

    if (show == 1) {
        for (i = 0; i < 5; i++) {
            func_088D9CD8(self->work->handles[0x3F + i]);
        }
        for (i = 0; i < 3; i++) {
            func_0887B924(TabTexts29::Caption(self->work, i * 2));
        }
    } else {
        for (i = 0; i < 5; i++) {
            func_088D9CFC(self->work->handles[0x3F + i]);
        }
        for (i = 0; i < 3; i++) {
            int pair = i * 2;
            func_0887B944(TabTexts29::Caption(self->work, pair));
            func_0887B944(TabTexts29::Shadow(self->work, pair));
        }
    }
}

/* The four help models (0x45..0x48) and the help text for the current
   mode (flag 1: text 0x38, flag 2: text 0x39); keeps `value` (+0xEE4). */
