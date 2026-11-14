typedef struct Ent12 { int field_00; unsigned short offset; unsigned short size; unsigned char field_08; unsigned char bank; char pad_0A[2]; } Ent12;
typedef struct Pad { char pad_00[0x94]; int banks[29]; Ent12 *entries; } Pad;
int func_0886CDE0(Pad *self, int idx)
{
    Ent12 *e = &self->entries[idx];
    if (e->size == 0) {
        return 0;
    } else {
        return e->offset + self->banks[e->bank] + 4;
    }
}
int func_0886CE28(Pad *self, int idx)
{
    Ent12 *e = &self->entries[idx];
    if (e->size == 0) {
        return 0;
    } else {
        return e->offset + self->banks[e->bank] + ((unsigned int)e->size >> 2) * 4 - 8;
    }
}
