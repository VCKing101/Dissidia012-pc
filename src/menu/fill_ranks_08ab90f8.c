/* Fills a rank list: all -1, first 0, middle entries 2 (second 1), and a final 3 once there are two or more entries. */
typedef struct Dt { char pad0[0xD40]; int count; int v[1]; } Dt;
typedef struct Fr { char pad0[0x138]; Dt *d; } Fr;
void func_08AB90F8(Fr *self) {
    int i = 0;
    int minus = -1;
    int off;
    if (i < self->d->count) {
        off = 0;
        do {
            *(int *)((char *)self->d + off + 0xD44) = minus;
            i++;
            off += 4;
        } while (i < self->d->count);
    }
    self->d->v[0] = 0;
    i = 1;
    if (self->d->count > 1) {
        int two = 2;
        self->d->v[0] = i;
        if (i < self->d->count - 1) {
            off = 4;
            do {
                *(int *)((char *)self->d + off + 0xD44) = two;
                i++;
                off += 4;
            } while (i < self->d->count - 1);
        }
        self->d->v[self->d->count - 1] = 3;
    }
}
