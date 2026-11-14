/* Screen15A88: tints the names (texts 0x1F..) of the listed pieces with
   flag 1, in the colour of frame 0x20 of model 0x2C: the flag-2 pieces
   (+0xF00) or, with `others` set, the rest (+0xF20). Standalone: the
   piece list is not in the region header's Work15A88. */
typedef struct Piece29 {
    unsigned char flags;
    char pad_01[0x1D];
} Piece29;

typedef struct PieceWork29 {
    char pad_000[0x400];
    void *handles[0x120];  /* +0x400 */
    void *texts[0x11A];    /* +0x880 */
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

extern void func_088DA0A8(void *model, int index, float *matrix, unsigned int *color);
extern void func_0887B828(void *text, const unsigned int *color);

void func_08A124FC(PieceScreen29 *self, int others)
{
    float matrix[16];
    unsigned int color;
    int i;

    func_088DA0A8(self->work->handles[0x2C], 0x20, matrix, &color);
    if (others == 0) {
        for (i = 0; i < self->work->marked_count; i++) {
            if (self->work->pieces[self->work->marked[i]].flags & 1) {
                func_0887B828(self->work->texts[0x1F + i], &color);
            }
        }
    } else {
        for (i = 0; i < self->work->others_count; i++) {
            if (self->work->pieces[self->work->others[i]].flags & 1) {
                func_0887B828(self->work->texts[0x1F + i], &color);
            }
        }
    }
}
