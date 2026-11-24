/* Looks up the reward file by name, loads it into a buffer, parses it and stores size and data pointer. */
extern void *func_088FE7C8(const char *name, int a);
extern void *func_0885C6CC(int a, void *b, int c, int d, int e, int f);
extern void *func_088FE708(const char *name, void *buf, void *id, int a);
extern void func_088FE944(const char *name, void *buf, void *r);
extern void func_08935970(void *buf, const char *fmt);
extern const char D_08B42688[];
extern const char D_08B426AC[];

typedef struct F423D0 { char pad[8]; short size; char pad2[6]; char data[1]; } F423D0;
typedef struct S423D0 { int owner; int size; void *data; F423D0 *file; } S423D0;

void func_08B423D0(S423D0 *self)
{
    void *id = func_088FE7C8(D_08B42688, 0);
    void *r;
    F423D0 *f;
    self->file = func_0885C6CC(self->owner, id, 0x40, 0, 1, 0);
    r = func_088FE708(D_08B42688, self->file, id, 0);
    func_088FE944(D_08B42688, self->file, r);
    f = self->file;
    func_08935970(f, D_08B426AC);
    self->size = f->size;
    self->data = f->data;
}
