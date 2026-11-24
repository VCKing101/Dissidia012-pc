/* Shows (flag 1) or hides the two arrow widgets, and for a valid index applies the index's frame pair to the first. */
extern void func_088D9CD8(void *w);
extern void func_088D9CFC(void *w);
extern void func_088D9F5C(void *w, int id, int a);

typedef struct Fr1AEC { short a; short b; } Fr1AEC;
extern Fr1AEC D_08AE45D2[];
typedef struct W1AEC { char pad[0xE]; short frame; } W1AEC;
typedef struct B1AEC { char pad[0x464]; W1AEC *w[2]; } B1AEC;
typedef struct S1AEC { char pad[0x850]; B1AEC *blk; } S1AEC;

void func_08AE1AEC(S1AEC *self, unsigned char flag, int idx)
{
    if (flag == 1) {
        func_088D9CD8(self->blk->w[0]);
        func_088D9CD8(self->blk->w[1]);
        if (idx != -1) {
            func_088D9F5C(*(void **)((char *)self->blk + idx * 4 + 0x464), D_08AE45D2[idx].a, 1);
            ((W1AEC *)*(void **)((char *)self->blk + idx * 4 + 0x464))->frame = D_08AE45D2[idx].b;
        }
    } else {
        func_088D9CFC(self->blk->w[0]);
        func_088D9CFC(self->blk->w[1]);
    }
}
