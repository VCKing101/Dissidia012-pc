// Returns the first 0x24-byte entry that passes both tests for the key, or 0.
struct List { int count; char *items; };
struct Owner { List *list; };
extern "C" {
extern int func_08923098(void *, int);
extern int func_08923370(void *);
char *func_08B49150(Owner *self, int key) {
    for (int i = 0, off = 0; i < self->list->count; i++, off += 0x24) {
        if (func_08923098(self->list->items + off, key)) {
            if (func_08923370(self->list->items + off)) {
                return self->list->items + off;
            }
        }
    }
    return 0;
}
}
