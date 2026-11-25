/* Counts the list slots other than the skipped one whose record is live, has no hold flag and carries the given id. */
typedef struct Rec { char p0[0xA]; short fA; char p1[0x14 - 0xC]; signed char f14; } Rec;
typedef struct Slot { short id; char p[6]; } Slot;
typedef struct W {
    char p0[0x1CA4]; int sel; int count; char p1[4]; short f1CB4_0; char p2[0x3064 - 0x1CB8];
    unsigned short rows[1];
} W;
typedef struct O { char p[0x46AC]; W *w; } O;
extern Rec *func_088B29D8(int);
int func_08ADB22C(O *self, int key, int skip) {
    int i;
    int n;
    int off;
    int none;
    W *w;
    n = 0;
    for (i = 0; i < self->w->count; i++) {
        off = i * 2;
        if (i != skip) {
            unsigned id = *(unsigned short *)((char *)self->w + self->w->sel * 20 + off + 0x3064);
            if (id != 0x7FFF) {
                Rec *r = func_088B29D8(*(short *)((char *)self->w + id * 8 + 0x1CB4));
                if (r != 0 && r->f14 == 0 && r->fA == key) n++;
            }
        }
    }
    return n;
}
