/* Finds an entry matching a freshly built key in a 32-byte-entry list and processes it; 0 if absent. */
typedef struct { char d[32]; } Tmp;
typedef struct { int pad; int count; char *items; } Self;
extern void func_08B42290(Tmp *);
extern char *func_08B42524(char *, char *, Tmp *);
extern int func_08B42480(Self *, char *);
int func_08B422F0(Self *self)
{
    Tmp t;
    char *found;
    func_08B42290(&t);
    found = func_08B42524(self->items, self->items + self->count * 32, &t);
    if (found == self->items + self->count * 32) return 0;
    return func_08B42480(self, found);
}
