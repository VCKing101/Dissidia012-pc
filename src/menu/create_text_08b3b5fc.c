/* Creates the text object once (allocating and initialising it) and loads the dungeon message file into it. */
extern void *func_0887B5A8(int size);
extern void func_0887B1E4(void *p);
extern void func_0887B120(void);
extern void func_0887B260(void *obj, const char *name, int id, int a);
extern void func_0887B130(void);
extern int func_08B42B40(void);
extern const char D_08B418CC[];

typedef struct S3B5FC { char pad[0xC8]; void **slot; } S3B5FC;

int func_08B3B5FC(S3B5FC *self)
{
    void *r;
    void *p;
    void *obj;
    const char *name;
    int id;
    if (*self->slot != 0) {
        return 0;
    }
    r = 0;
    p = func_0887B5A8(8);
    if (p != 0) {
        func_0887B1E4(p);
        r = p;
    }
    *self->slot = r;
    func_0887B120();
    obj = *self->slot;
    name = D_08B418CC;
    id = func_08B42B40();
    func_0887B260(obj, name, id, 0);
    func_0887B130();
    return 1;
}
