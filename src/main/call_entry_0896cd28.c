/* If bit 1 of the halfword at 2 of the table at 0x8C is clear, calls func_0896CD88 with the object, a2 and the address of the current entry (table + entries[cursor] + 0x60). */
typedef struct { char pad[2]; unsigned short flags; } Tab0896CD28;
typedef struct { int cursor0; int cur; unsigned short *offs; } Cur0896CD28;
typedef struct { char pad[0x8C]; Tab0896CD28 *tab; Cur0896CD28 *cur; } Obj0896CD28;
extern void func_0896CD88(Obj0896CD28 *, int, char *);
void func_0896CD28(int a0, Obj0896CD28 *o, int a2) {
    if (o->tab->flags & 2) { return; }
    func_0896CD88(o, a2, (char *)o->tab + o->cur->offs[o->cur->cur] + 0x60);
}
