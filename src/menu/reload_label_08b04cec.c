/* Frees the label handle in the selected slot, formats a new name, creates the label and registers it. */
extern void func_0887BA5C(void *h);
extern int func_08B0C4B8(void *self);
extern void func_0893476C(char *buf, const char *fmt, int arg);
extern void func_0887DF14(void *g, void *dst, int n, char *src);
extern void *func_0887D868(void *g, int a, void *name, int b, int c, int d, int e, int f);
extern void func_088D428C(void *a, void *b, const char *c, int d);
extern const char D_08B1C8EC[];
extern const char D_08B1C5D8[];
extern struct Idx04CEC { char pad[0x28]; unsigned short idx; } D_08B1C55A;
extern void *D_089D6998;

typedef struct R04CEC {
    char pad0[0x404];
    char at404[0x80];
    char pad484[0x884 - 0x484];
    void *handle[4];
    char pad894[0x1A3A - 0x894];
    char name[0x80];
} R04CEC;
typedef struct S04CEC { char pad[0x138]; R04CEC *rec; } S04CEC;

void func_08B04CEC(S04CEC *self)
{
    char buf[64];
    if (self->rec->handle[D_08B1C55A.idx] != 0) {
        func_0887BA5C(self->rec->handle[D_08B1C55A.idx]);
        self->rec->handle[D_08B1C55A.idx] = 0;
    }
    func_0893476C(buf, D_08B1C8EC, func_08B0C4B8(self));
    func_0887DF14(D_089D6998, self->rec->name, 0x80, buf);
    {
        char *nm = self->rec->name;
        void *h = func_0887D868(D_089D6998, 0, nm, -1, 0, 0, 0, 0);
        self->rec->handle[D_08B1C55A.idx] = h;
    }
    func_088D428C(self->rec->at404, self->rec->handle, D_08B1C5D8, 1);
}
