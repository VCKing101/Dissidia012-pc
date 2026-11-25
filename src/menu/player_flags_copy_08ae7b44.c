/* Per-player flag halfword: clears the low flags and copies bits 0x1000/0x2000/0x4000/0x8000 and 2 from the selected entry. */
typedef struct B7B44 { char pad[0x2694]; int sel; char p2[0x26B0 - 0x2698]; unsigned short fl[4]; } B7B44;
typedef struct S7B44 { char pad[0xA690]; B7B44 *b; } S7B44;
void func_08AE7B44(S7B44 *self)
{
    int i;
    for (i = 0; i < 4; i++) {
        unsigned int k = *(unsigned short *)((char *)self->b + self->b->sel * 8 + i * 2 + 0x7E98);
        self->b->fl[i] = self->b->fl[i] & 0xFFFF0FFD;
        if (k != 0x7FFF) {
            if (*(unsigned short *)((char *)self->b + i * 0x15F8 + k * 8 + 0x26BA) & 0x1000) {
                self->b->fl[i] = self->b->fl[i] | 0x1000;
            } else if (*(unsigned short *)((char *)self->b + i * 0x15F8 + k * 8 + 0x26BA) & 0x2000) {
                self->b->fl[i] = self->b->fl[i] | 0x2000;
            } else if (*(unsigned short *)((char *)self->b + i * 0x15F8 + k * 8 + 0x26BA) & 0x4000) {
                self->b->fl[i] = self->b->fl[i] | 0x4000;
            }
            if (*(unsigned short *)((char *)self->b + i * 0x15F8 + k * 8 + 0x26BA) & 0x8000) {
                self->b->fl[i] = self->b->fl[i] | 0x8000;
            }
            if (*(unsigned short *)((char *)self->b + i * 0x15F8 + k * 8 + 0x26BA) & 2) {
                self->b->fl[i] = self->b->fl[i] | 2;
            }
        }
    }
}
