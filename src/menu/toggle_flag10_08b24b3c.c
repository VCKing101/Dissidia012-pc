/* If on == 1 calls func_088FCB8C and clears bit 0x10 in the word at +0x910 of self->0x13C; else calls func_088FCBBC(D_089D7F90) and sets it. */
struct Obj { char pad[0x910]; unsigned flags; };
struct Self { char pad[0x13C]; struct Obj *obj; };
extern void *D_089D7F90;
extern void func_088FCB8C(void *p);
extern void func_088FCBBC(void *p);
void func_08B24B3C(struct Self *self, unsigned char on)
{
    if (on == 1) {
        func_088FCB8C(D_089D7F90);
        self->obj->flags = self->obj->flags & ~0x10;
    } else {
        func_088FCBBC(D_089D7F90);
        self->obj->flags = self->obj->flags | 0x10;
    }
}
