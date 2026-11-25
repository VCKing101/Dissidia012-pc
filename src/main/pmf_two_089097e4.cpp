class C089097E4 {
public:
    char pad0[4];
    char sub[0x18C];
    int cur;
    int prev;
    void a();
};
typedef void (C089097E4::*H089097E4)();
struct Tbl089097E4 { H089097E4 h[6]; };
extern "C" {
void func_08935524(void *, const void *, int);
void func_089038F4(void *);
extern const Tbl089097E4 D_089D8280;
extern const Tbl089097E4 D_089D82B0;

void func_089097E4(C089097E4 *self)
{
    Tbl089097E4 enter;
    Tbl089097E4 tick;
    int unused[1];
    func_08935524(&enter, &D_089D8280, sizeof(enter));
    func_08935524(&tick, &D_089D82B0, sizeof(tick));
    int k = self->cur;
    if (k != self->prev) {
        (self->*enter.h[k])();
        self->prev = self->cur;
    }
    func_089038F4(self->sub);
    (self->*tick.h[self->cur])();
}
}
