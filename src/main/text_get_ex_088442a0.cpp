/* Returns the cached text object of an entry, or creates and loads it
   (variant with a full parameter set). */
struct Text088442A0 {
    char data[0xEC];
};

struct TextCache088442A0 {
    char pad_0[4];
    int heap;
};

extern "C" {

Text088442A0 *func_088444D4(TextCache088442A0 *self, int id);
void func_0884403C(Text088442A0 *text);
Text088442A0 *func_0885C6CC(int heap, int size, int align, int tag, int a, int b);
void func_08843C1C(Text088442A0 *text);
void func_088430B8(Text088442A0 *text, int id, int a, int b, int c, int d, int e, int f);

Text088442A0 *func_088442A0(TextCache088442A0 *self, int id, int a, int b, int c, int d, int e, int f)
{
    Text088442A0 *t = 0;
    Text088442A0 *cached = func_088444D4(self, id);
    if (cached != 0) {
        func_0884403C(cached);
        return cached;
    }
    int one = 1;
    Text088442A0 *p = func_0885C6CC(self->heap, sizeof(Text088442A0), 0, 0, one, 0);
    t = 0;
    if (p != 0) {
        func_08843C1C(p);
        t = p;
    }
    func_088430B8(t, id, a, b, c, d, e, f);
    return t;
}

}
