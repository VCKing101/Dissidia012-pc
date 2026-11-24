/* Formats the event name, and when the pack exists registers it with the screen and starts the event or flags it. */
extern void func_0893476C(char *buf, const char *fmt, int arg);
extern int func_089359E8(char *s);
extern int func_08862C40(char *name, int a);
extern void func_089359A4(char *dst, char *src);
extern void func_08B3BD20(void *self);
extern int func_088FEA4C(void);
extern void func_08B38E18(void *self);
extern const char D_08B418AC[];

typedef struct B38D34 { char pad[4]; unsigned f4; char pad8[0x34]; int f3C; char name[1]; } B38D34;
typedef struct S38D34 { char pad[0x44]; unsigned flags; char pad48[0x7C]; B38D34 *b; } S38D34;

void func_08B38D34(S38D34 *self, int id)
{
    char buf[128];
    int r;
    func_0893476C(buf, D_08B418AC, id);
    func_089359E8(buf);
    r = func_089359E8(buf);
    if (r == 0) {
        return;
    }
    r = func_08862C40(buf, 1);
    if (r == 0) {
        return;
    }
    func_089359A4(self->b->name, buf);
    self->b->f3C = r;
    self->flags = self->flags | 4;
    self->b->f4 = self->b->f4 | 1;
    func_08B3BD20(self);
    if (func_088FEA4C() != 0) {
        self->flags = self->flags | 0x80000;
    } else {
        func_08B38E18(self);
    }
}
