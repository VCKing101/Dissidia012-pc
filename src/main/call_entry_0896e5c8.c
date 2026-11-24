/* If bit 1 of the halfword at 2 of the table at 0x8C is clear, calls func_0896E628 with the object, a2 and the address of the current entry (table + entries[cursor] + 0x60). */
typedef struct { char pad[2]; unsigned short flags; } Tab0896E5C8;
typedef struct { int cursor0; int cur; unsigned short *offs; } Cur0896E5C8;
typedef struct { char pad[0x8C]; Tab0896E5C8 *tab; Cur0896E5C8 *cur; } Obj0896E5C8;
extern void func_0896E628(Obj0896E5C8 *, int, char *);
void func_0896E5C8(int a0, Obj0896E5C8 *o, int a2) {
    if (o->tab->flags & 2) { return; }
    func_0896E628(o, a2, (char *)o->tab + o->cur->offs[o->cur->cur] + 0x60);
}
