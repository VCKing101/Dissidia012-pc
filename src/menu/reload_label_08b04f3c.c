/* Frees the label handle in the selected slot, formats a new name into a stack buffer and creates the label. */
extern void func_0887BA5C(void *h);
extern void func_0893476C(char *buf, const char *fmt, int arg);
extern void func_0887DF14(void *g, void *dst, int n, char *src);
extern void *func_0887D868(void *g, int a, void *name, int b, int c, int d, int e, int f);
extern const char D_08B1C8F0[];
extern struct Idx04F3C { char pad[0x60]; unsigned short idx; } D_08B1C55A;
extern void *D_089D6998;

typedef struct R04F3C {
    char pad0[0x884];
    void *handle[4];
    char pad894[0x283A - 0x894];
    char name[0x80];
    char pad2[0x1F53C - 0x283A - 0x80];
    int num;
} R04F3C;
typedef struct S04F3C { char pad[0x138]; R04F3C *rec; } S04F3C;

void func_08B04F3C(S04F3C *self)
{
    char buf[64];
    R04F3C *rec = self->rec;
    if (rec->handle[D_08B1C55A.idx] != 0) {
        func_0887BA5C(rec->handle[D_08B1C55A.idx]);
        self->rec->handle[D_08B1C55A.idx] = 0;
        rec = self->rec;
    }
    func_0893476C(buf, D_08B1C8F0, rec->num);
    func_0887DF14(D_089D6998, self->rec->name, 0x80, buf);
    {
        char *nm = self->rec->name;
        void *h = func_0887D868(D_089D6998, 0, nm, -1, 0, 0, 0, 0);
        self->rec->handle[D_08B1C55A.idx] = h;
    }
}
