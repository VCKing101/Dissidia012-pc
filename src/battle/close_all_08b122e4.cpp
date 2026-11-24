// Calls func_0887B944 on each of five pointers at +4..+0x14 when set, then clears the word at +0x18.
extern "C" void func_0887B944(void *);
struct Obj08B122E4 { int pad; void *p[5]; int w18; };
extern "C" void func_08B122E4(Obj08B122E4 *self)
{
    for (int i = 0; i < 5; i++) {
        if (self->p[i] != 0) {
            func_0887B944(self->p[i]);
        }
    }
    self->w18 = 0;
}
