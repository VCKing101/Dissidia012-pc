/* Creates an 8-byte object (func_0887B5A8 + func_0887B1E4) into field 0x15C, then func_0887B120, func_0887B260(obj, "text/JP/mess_pk_worldmap_0.bin", arg, 0) and func_0887B130. */
typedef struct Obj089115a4 { unsigned char pad[0x15C]; void *msg; } Obj089115a4;
extern char D_089CE230[];
extern void *func_0887B5A8(int size);
extern void func_0887B1E4(void *p);
extern void func_0887B120(void);
extern void func_0887B260(void *p, char *name, int arg, int z);
extern void func_0887B130(void);
void func_089115A4(Obj089115a4 *self, int arg)
{
    void *r = 0;
    void *p = func_0887B5A8(8);
    if (p) {
        func_0887B1E4(p);
        r = p;
    }
    self->msg = r;
    func_0887B120();
    func_0887B260(self->msg, D_089CE230, arg, 0);
    func_0887B130();
}
