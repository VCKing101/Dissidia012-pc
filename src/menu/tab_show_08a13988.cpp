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

/* The four help models (0x45..0x48) and the help text for the current
   mode (flag 1: text 0x38, flag 2: text 0x39); keeps `value` (+0xEE4). */
extern "C" void func_08A13988(TabScreen29 *self, unsigned char show, int value)
{
    if (show == 1) {
        func_088D9CD8(self->work->handles[0x45]);
        func_088D9CD8(self->work->handles[0x46]);
        func_088D9CD8(self->work->handles[0x47]);
        func_088D9CD8(self->work->handles[0x48]);
        self->work->flags |= 4;
        if (self->work->flags & 1) {
            func_0887B924(self->work->texts[0x38]);
        } else if (self->work->flags & 2) {
            func_0887B924(self->work->texts[0x39]);
        }
    } else {
        func_088D9CFC(self->work->handles[0x45]);
        func_088D9CFC(self->work->handles[0x46]);
        func_088D9CFC(self->work->handles[0x47]);
        func_088D9CFC(self->work->handles[0x48]);
        func_0887B944(self->work->texts[0x38]);
        func_0887B944(self->work->texts[0x39]);
    }
    self->work->field_EE4 = value;
}
