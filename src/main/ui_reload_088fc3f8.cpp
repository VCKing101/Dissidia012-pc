struct W088FC3F8 {
    char pad0[0xEF0];
    void *h0;
    char pad1[0xF24 - 0xEF4];
    void *h1;
    char pad2[0xF3C - 0xF28];
    void *h2;
    void *items[9];
    char pad3[0xF9E - 0xF40 - 0x24];
    char text[1];
};
struct Obj088FC3F8 { char pad[0x30]; W088FC3F8 *w; };
extern "C" void func_0887BA5C(void *);
extern "C" void *func_08885CC8(int, int, int);
extern "C" void func_0893476C(char *, const char *, ...);
extern "C" void func_0887DF14(void *, char *, int, char *);
extern "C" void *func_0887D868(void *, int, char *, int, int, int, int, int);
extern char D_089CD06C[];
extern void *D_089D6998;
extern "C" void func_088FC3F8(Obj088FC3F8 *self);
extern "C" void func_088FC3F8(Obj088FC3F8 *self)
{
    char buf[0x40];
    if (self->w->h0 != 0) {
        func_0887BA5C(self->w->h0);
        self->w->h0 = 0;
    }
    if (self->w->h1 != 0) {
        func_0887BA5C(self->w->h1);
        self->w->h1 = 0;
    }
    if (self->w->h2 != 0) {
        func_0887BA5C(self->w->h2);
        self->w->h2 = 0;
    }
    self->w->h0 = func_08885CC8(0, 0, 0);
    self->w->h1 = func_08885CC8(1, 0, 0);
    self->w->h2 = func_08885CC8(2, 0, 0);
    int i = 0;
    int off = 0;
    do {
        if (*(void **)((char *)self->w + off + 0xF40) != 0) {
            func_0887BA5C(*(void **)((char *)self->w + off + 0xF40));
            *(void **)((char *)self->w + off + 0xF40) = 0;
        }
        i = i + 1;
        func_0893476C(buf, D_089CD06C, i);
        func_0887DF14(D_089D6998, self->w->text, 0x80, buf);
        char *tx = self->w->text;
        *(void **)((char *)self->w + off + 0xF40) = func_0887D868(D_089D6998, 0, tx, -1, 0, 0, 0, 0);
        off = off + 4;
    } while (i < 9);
}
