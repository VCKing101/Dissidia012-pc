typedef struct Info0884F90C {
    unsigned char unk_00[8];
    unsigned char flags;               /* +0x08 bit 3 */
} Info0884F90C;

typedef struct Owner0884F90C {
    unsigned char unk_000[0x2F8];
    Info0884F90C *info;                /* +0x2F8 */
    unsigned char unk_2FC[0x5A4 - 0x2FC];
    unsigned char *kind;               /* +0x5A4 */
} Owner0884F90C;

typedef struct Self0884F90C {
    unsigned char unk_000[0x200];
    Owner0884F90C *owner;              /* +0x200 */
    unsigned char unk_204[3];
    signed char value;                 /* +0x207 */
    unsigned char dirty;               /* +0x208 */
} Self0884F90C;

extern signed char func_08850A64(int kind, unsigned char flag);

int func_0884F90C(Self0884F90C *self)
{
    if (self->dirty) {
        self->value = func_08850A64(*self->owner->kind, (self->owner->info->flags & 8) != 0);
    }
    return self->value;
}
