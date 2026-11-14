/* Creates a text object for an entry and sets its two parts. */
struct Text08842F78 {
    void *info;
    char data[0xE8];
};

struct Key08842F78 {
    char pad_0[4];
    short id;
};

struct Src08842F78 {
    Key08842F78 *key;
};

struct Part08842F78 {
    char pad_0[8];
    int a;
    int b;
};

extern "C" {

extern void *D_089D4960;

Text08842F78 *func_08844130(void *heap, int size);
void func_08843C1C(Text08842F78 *self);
void *func_088470B8(int id);
void func_0884333C(Text08842F78 *self, int a, int b);
void func_088459E0(Text08842F78 *self, int a, int b);

}

struct TextNew08842F78 {
    static Text08842F78 *make(void *heap)
    {
        Text08842F78 *p = func_08844130(heap, sizeof(Text08842F78));
        Text08842F78 *r = 0;
        if (p != 0) {
            func_08843C1C(p);
            r = p;
        }
        return r;
    }
};

extern "C" {

void func_08842F78(Src08842F78 *src, Part08842F78 *body, Part08842F78 *title)
{
    Text08842F78 *t = TextNew08842F78::make(D_089D4960);
    t->info = func_088470B8(src->key->id);
    func_0884333C(t, body->a, body->b);
    func_088459E0(t, title->a, title->b);
}

}
