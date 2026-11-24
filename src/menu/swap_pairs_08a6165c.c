/* Calls a show and a hide function on two pairs of sub-objects, in an order set by two mode bytes. */
typedef struct Mb { char pad0[0x1C]; unsigned char *m1C; unsigned char *m20; } Mb;
typedef struct Sp { char pad0[0x5C]; int o5C, o60, o64, o68; char pad1[0x128 - 0x6C]; Mb *mb; } Sp;
extern void func_088D9CD8(int);
extern void func_088D9CFC(int);
void func_08A6165C(Sp *self) {
    switch (*self->mb->m20) {
    case 1:
        func_088D9CD8(self->o5C);
        func_088D9CFC(self->o60);
        break;
    case 0:
        func_088D9CFC(self->o5C);
        func_088D9CD8(self->o60);
        break;
    }
    switch (*self->mb->m1C) {
    case 0:
        func_088D9CD8(self->o68);
        func_088D9CFC(self->o64);
        break;
    case 1:
        func_088D9CFC(self->o68);
        func_088D9CD8(self->o64);
        break;
    }
}
