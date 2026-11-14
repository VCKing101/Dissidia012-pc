/* Creates a text object for an entry and records its id in the first
   free slot of the cache. */
struct Text088443A0 {
    void *info;
    char data[0xE8];
};

struct Heap088443A0 {
    char pad_00[0x60];
    signed char busy;
};

struct TextCache088443A0 {
    char pad_00[4];
    int heap;
    char pad_08[8];
    int ids[16];
    int a;
    int b;
    char pad_58[8];
    unsigned char dirty;
};

extern "C" {

extern Heap088443A0 *D_089D4960;

void *func_088444D4(TextCache088443A0 *self, int id);
Text088443A0 *func_0885C6CC(int heap, int size, int align, int tag, int a, int b);
void func_08843C1C(Text088443A0 *text);
void *func_088470B8(int id);

}

struct Heap088443A0Ops {
    static bool busy() { return D_089D4960->busy != 0; }
};

extern "C" {

Text088443A0 *func_088443A0(TextCache088443A0 *self, int id, int a, int b)
{
    Text088443A0 *t = 0;
    func_088444D4(self, id);
    for (int i = 0; i < 16; i++) {
        if (self->ids[i] < 0) {
            self->ids[i] = id;
            break;
        }
    }
    if (!Heap088443A0Ops::busy()) {
        self->dirty = 1;
    }
    int one = 1;
    Text088443A0 *p = func_0885C6CC(self->heap, sizeof(Text088443A0), 0, 0, one, 0);
    t = 0;
    if (p != 0) {
        func_08843C1C(p);
        t = p;
    }
    self->a = a;
    self->b = b;
    t->info = func_088470B8(id);
    return t;
}

}
