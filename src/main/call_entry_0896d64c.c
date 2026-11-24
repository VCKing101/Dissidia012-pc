/* If bit 1 of the halfword at 2 of the table at 0x8C is clear, calls func_0896D6AC with the object, a2 and the address of the current entry (table + entries[cursor] + 0x60). */
typedef struct { char pad[2]; unsigned short flags; } Tab0896D64C;
typedef struct { int cursor0; int cur; unsigned short *offs; } Cur0896D64C;
typedef struct { char pad[0x8C]; Tab0896D64C *tab; Cur0896D64C *cur; } Obj0896D64C;
extern void func_0896D6AC(Obj0896D64C *, int, char *);
void func_0896D64C(int a0, Obj0896D64C *o, int a2) {
    if (o->tab->flags & 2) { return; }
    func_0896D6AC(o, a2, (char *)o->tab + o->cur->offs[o->cur->cur] + 0x60);
}
