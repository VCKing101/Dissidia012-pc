/* Starts the two maxima at +0x7CB0/+0x7CB4 of the block at 1 and raises them over the five listed entries: each entry's two values from func_088CBAD0 and its record byte (both replaced by a table value when the game flag is set). */
typedef struct Pair08AA9238 { int id; int v; } Pair08AA9238;
typedef struct Blk08AA9238 {
    char pad0[0x788];
    Pair08AA9238 e[5];
    char pad1[0x7CB0 - 0x7B0];
    int max0;
    int max1;
} Blk08AA9238;
typedef struct Rec08AA9238 { char pad; unsigned char b; } Rec08AA9238;
typedef struct Game08AA9238 { char pad0[8]; int w8; char pad1[0x1C - 0xC]; int w1C; } Game08AA9238;
typedef struct S08AA9238 { char pad[0x7CFC]; Blk08AA9238 *blk; } S08AA9238;
extern char *D_089D712C;
extern Game08AA9238 *D_089D8140;
extern unsigned short D_08AB06B0[];
extern Rec08AA9238 *func_088CB164(char *base, int id);
extern int func_088CBAD0(Rec08AA9238 *r);
void func_08AA9238(S08AA9238 *self)
{
    int i;
    self->blk->max0 = 1;
    self->blk->max1 = 1;
    for (i = 0; i < 5; i++) {
        int id = self->blk->e[i].id;
        if (id > 0) {
            Rec08AA9238 *r = func_088CB164(D_089D712C + 0xF58, id);
            int a = func_088CBAD0(r);
            int b = r->b;
            if (D_089D8140->w1C & 1) {
                a = D_08AB06B0[D_089D8140->w8];
                b = a;
            }
            if (self->blk->max0 < a) {
                self->blk->max0 = a;
            }
            if (self->blk->max1 < b) {
                self->blk->max1 = b;
            }
        }
    }
}
