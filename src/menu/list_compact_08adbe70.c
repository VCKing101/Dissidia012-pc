/* Packs the selected row's non-empty (not 0x7FFF) halfwords into the shared list, notifies each, writes them back padded with 0x7FFF, then restores the saved list. */
typedef struct B8BE70 { char p0[0x1CA4]; int sel; char p1[0x30C8 - 0x1CA8]; unsigned short list[10]; } B8BE70;
typedef struct S8BE70 { char pad[0x46AC]; B8BE70 *b; } S8BE70;
extern void func_08ADBC84(S8BE70 *self, int i);
void func_08ADBE70(S8BE70 *self)
{
    unsigned short saved[10];
    int n = 0;
    int i;
    int j;

    for (i = 0; i < 10; i++) {
        unsigned short v;
        saved[i] = self->b->list[i];
        v = *(unsigned short *)((char *)self->b + self->b->sel * 20 + i * 2 + 0x3064);
        if (v != 0x7FFF) {
            self->b->list[n] = v;
            n++;
        }
    }
    for (j = 0; j < n; j++) {
        func_08ADBC84(self, j);
    }
    for (i = 0; i < 10; i++) {
        unsigned short v = 0x7FFF;
        if (i < n) {
            v = self->b->list[i];
        }
        *(unsigned short *)((char *)self->b + self->b->sel * 20 + i * 2 + 0x3064) = v;
    }
    for (i = 0; i < 10; i++) {
        self->b->list[i] = saved[i];
    }
}
