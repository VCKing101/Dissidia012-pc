/* Screen15A88: rebuilds the piece list's name texts (0x1F..) for the
   pieces without flag 2 (+0xF20, count +0xF42), plays the list script,
   moves to state 0xD, hides the two title texts (0x1D, 0x1E) and the
   piece marks. Standalone: the piece list is not in the region header's
   Work15A88. */
typedef struct Piece29 {
    unsigned char flags;
    char pad_01[0x3];
    char name[0x1A]; /* +0x04 */
} Piece29;

typedef struct PieceWork29 {
    char pad_000[0x400];
    void *handles[0x120];  /* +0x400 */
    void *texts[0x103];    /* +0x880 */
    int state;             /* +0xC8C */
    char pad_C90[0x58];
    Piece29 pieces[16];    /* +0xCE8 */
    char pad_EC8[0x38];
    short marked[16];      /* +0xF00 */
    short others[16];      /* +0xF20 */
    short marked_count;    /* +0xF40 */
    short others_count;    /* +0xF42 */
} PieceWork29;

typedef struct PieceScreen29 {
    void *vtable;
    char pad_04[0xF5C];
    PieceWork29 *work; /* +0xF60 */
} PieceScreen29;

extern "C" {
extern void *D_08A23FF0;
extern void *D_089D6998;
extern const unsigned short D_08A15BCA[];
extern void func_08A1E144(void *root, int arg);
extern void func_0887BA5C(void *text);
extern void func_0887B944(void *text);
extern void *func_0887D868(void *owner, int arg1, const char *text, int arg3, int arg4, int arg5, int arg6, int arg7);
extern void func_088D428C(void *handles, void *texts, const unsigned short *script, char arg);
extern void func_08A13B64(PieceScreen29 *self, unsigned char show);
}

/* Inlined calls on the object in D_089D6998. */
struct Object9D6998 {
    static void *Make(const char *text) {
        return func_0887D868(D_089D6998, 0, text, -1, 0, 0, 0, 0);
    }
};

extern "C" void func_08A12374(PieceScreen29 *self)
{
    int i;

    func_08A1E144(D_08A23FF0, 0);
    for (i = 0; i < 16; i++) {
        if (self->work->texts[0x1F + i] != 0) {
            func_0887BA5C(self->work->texts[0x1F + i]);
            self->work->texts[0x1F + i] = 0;
        }
    }
    for (i = 0; i < self->work->others_count; i++) {
        if (self->work->texts[0x1F + i] != 0) {
            func_0887BA5C(self->work->texts[0x1F + i]);
            self->work->texts[0x1F + i] = 0;
        }
        self->work->texts[0x1F + i] =
            Object9D6998::Make(self->work->pieces[self->work->others[i]].name);
    }
    func_088D428C(self->work->handles, self->work->texts, D_08A15BCA, 1);
    self->work->state = 0xD;
    func_0887B944(self->work->texts[0x1D]);
    func_0887B944(self->work->texts[0x1E]);
    func_08A13B64(self, 0);
}
