/* Screen15A88: hides the sixteen piece marks (models 0x2E..) and, when
   `show` is 1, shows again those of the flag-2 pieces (+0xF00 list,
   count +0xF40) whose state has bit 4. Standalone: the region header
   gives `show` as a plain char, and retail reads it as unsigned. */
typedef struct Piece29 {
    unsigned char flags;
    char pad_01[0x1D];
} Piece29;

typedef struct PieceWork29 {
    char pad_000[0x400];
    void *handles[0x120];  /* +0x400 */
    char pad_880[0x468];
    Piece29 pieces[16];    /* +0xCE8 */
    char pad_EC8[0x38];
    short marked[16];      /* +0xF00 */
    short others[16];      /* +0xF20 */
    short marked_count;    /* +0xF40 */
} PieceWork29;

typedef struct PieceScreen29 {
    void *vtable;
    char pad_04[0xF5C];
    PieceWork29 *work; /* +0xF60 */
} PieceScreen29;

extern void func_088D9CD8(void *model);
extern void func_088D9CFC(void *model);

void func_08A13B64(PieceScreen29 *self, unsigned char show)
{
    int i;

    if (show == 1) {
        for (i = 0; i < 16; i++) {
            func_088D9CFC(self->work->handles[0x2E + i]);
        }
        for (i = 0; i < self->work->marked_count; i++) {
            if (self->work->pieces[self->work->marked[i]].flags & 4) {
                func_088D9CD8(self->work->handles[0x2E + i]);
            }
        }
    } else {
        for (i = 0; i < 16; i++) {
            func_088D9CFC(self->work->handles[0x2E + i]);
        }
    }
}
