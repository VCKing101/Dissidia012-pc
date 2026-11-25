typedef struct Slot102 { int h; unsigned char a; unsigned char b; char pad[2]; } Slot102;
typedef struct Sel102 {
    char pad0[0x14];
    char *names;
    int count;
    int cur;
    unsigned state;
    char pad1[8];
    Slot102 slot[8];
    int w6c;
    int w70;
    int sel;
} Sel102;
extern int func_089359E8(char *s);
extern void func_089359A4(char *dst, char *src);
extern void func_08935D58(char *s);
extern char *func_08867984(char *s, int a);
extern void func_08862C40(char *s, int a);
extern void func_088629DC(char *s, int h, int w, void (*cb)(void), void *self, int one);
extern int func_088EB054(Sel102 *s);
extern char D_089D5F1C;
extern void func_088EB0B4(void);

void func_088EADEC(Sel102 *self) {
    char buf[256];
    switch (self->state) {
    case 0:
        if (self->cur < self->count - 1) {
            if (func_089359E8(self->names + self->cur * 0x40) != 0) {
                if (self->slot[self->sel].a != 0) {
                    self->state = 1;
                }
            }
        }
        break;
    case 1: {
        Slot102 *s;
        func_089359A4(buf, self->names + self->cur * 0x40);
        self->cur = self->cur + 1;
        func_08935D58(buf);
        func_089359A4(buf, func_08867984(buf, 1));
        D_089D5F1C = 1;
        func_08862C40(buf, 1);
        s = &self->slot[self->sel];
        func_088629DC(buf, s->h, self->w70, func_088EB0B4, self, 1);
        D_089D5F1C = 0;
        s->a = 0;
        s->b = 0;
        self->state = 2;
        break;
    }
    case 2:
        break;
    case 3:
        self->slot[self->sel].b = 1;
        self->sel = func_088EB054(self);
        self->state = 4;
        break;
    case 4:
        self->state = 0;
        break;
    }
}
