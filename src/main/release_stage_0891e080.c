/* Releases the object's buffer handles and calls its done-callback with the stored argument. */
typedef struct Stage {
    char pad[0x134]; int heap; int tex; int buf; void (*done)(int); int arg;
} Stage;
extern void func_0891E5E4(void);
extern void func_0887B1F4(int p, int n);
extern void func_0885C538(int heap, int h, int z);
extern void func_0885C488(int heap);
extern void func_0888208C(int tex);
void func_0891E080(Stage *p0)
{
    Stage *self = p0;
    if (self->buf != 0) {
        int h;
        func_0891E5E4();
        h = self->buf;
        if (h != 0) {
            func_0887B1F4(h + 8, 2);
            h = self->buf;
        }
        func_0885C538(self->heap, h, 0);
        self->buf = 0;
    }
    func_0885C488(self->heap);
    func_0888208C(self->tex);
    self->heap = 0;
    self->tex = 0;
    if (self->done != 0) {
        self->done(self->arg);
    }
}
