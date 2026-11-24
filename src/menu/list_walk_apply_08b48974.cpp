// Walks every 0x24-byte entry of the owner's list, looks up a value from each and applies it back.
struct List { int count; char *items; };
struct Owner { List *list; };
extern "C" {
extern int func_08923080(void *);
extern int func_08B4A59C(int, int);
extern void func_089241C8(void *, int);
void func_08B48974(Owner *self, int arg) {
    for (int i = 0, off = 0; i < self->list->count; i++, off += 0x24) {
        int a = func_08923080(self->list->items + off);
        int b = func_08B4A59C(arg, a);
        func_089241C8(self->list->items + off, b);
    }
}
}
