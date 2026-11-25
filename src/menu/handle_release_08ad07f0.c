/* Releases the handle lists (7 + 7 + 5 slots) held by the menu block: notifies func_08AD0934, frees via func_0887BA5C and clears each slot. */
typedef struct B8D07F0 { char p0[0xB28]; int ha[7]; int hb[7]; int hc[5]; } B8D07F0;
typedef struct S8D07F0 { char pad[0xDB30]; B8D07F0 *b; } S8D07F0;
extern void func_08AD0934(S8D07F0 *self, int h);
extern void func_0887BA5C(int h);
void func_08AD07F0(S8D07F0 *self)
{
    int i;
    for (i = 0; i < 7; i++) {
        int h = self->b->ha[i];
        if (h != 0) {
            func_08AD0934(self, h);
            h = self->b->ha[i];
            if (h != 0) {
                func_0887BA5C(h);
                self->b->ha[i] = 0;
            }
        }
        if (self->b->hb[i] != 0) {
            func_08AD0934(self, self->b->hb[i]);
            if (self->b->hb[i] != 0) {
                func_0887BA5C(self->b->hb[i]);
                self->b->hb[i] = 0;
            }
        }
    }
    for (i = 0; i < 5; i++) {
        if (self->b->hc[i] != 0) {
            func_08AD0934(self, self->b->hc[i]);
            if (self->b->hc[i] != 0) {
                func_0887BA5C(self->b->hc[i]);
                self->b->hc[i] = 0;
            }
        }
    }
}
