/* Resets the entry to its defaults (ids -1, floats 0, the given value in the two pair slots); -1 when inactive. */
typedef struct Ent38 {
    unsigned char active; char pad0[3];
    int state; int s8; char pad1[0x44];
    float f50; float f54; int i58; unsigned char b5c; unsigned char b5d; char pad2[2];
    int i60; int v64; int i68; int v6c; float f70; float f74; unsigned char b78; unsigned char b79; char pad3[0x16];
    int i90; float f94; float f98; unsigned char b9c; unsigned char b9d; char pad4[0x46];
    unsigned char bE4; unsigned char bE5; unsigned char bE6; unsigned char bE7; unsigned char bE8;
} Ent38;
int func_08A95C10(Ent38 *e, int value)
{
    int r = -1;
    if (e->active == 0) {
        return r;
    }
    {
        e->state = 2;
        e->s8 = 0;
        e->f50 = 0.0f;
        e->f54 = 0.0f;
        e->i58 = r;
        e->b5c = 0;
        e->b5d = 0;
        e->i60 = r;
        e->v64 = value;
        e->i68 = r;
        e->v6c = value;
        e->f70 = 0.0f;
        e->f74 = 0.0f;
        e->b78 = 0;
        e->b79 = 0;
        e->i90 = r;
        e->f94 = 0.0f;
        e->f98 = 0.0f;
        e->b9c = 0;
        e->b9d = 0;
        e->bE4 = 0;
        e->bE5 = 0;
        e->bE6 = 0;
        e->bE7 = 0;
        e->bE8 = 0;
    }
    return 0;
}
