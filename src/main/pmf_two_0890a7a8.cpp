class C0890A7A8 {
public:
    char pad0[4];
    char sub[0x18C];
    int cur;
    int prev;
    void a();
};
typedef void (C0890A7A8::*H0890A7A8)();
struct Tbl0890A7A8 { H0890A7A8 h[8]; };
extern "C" {
void func_08935524(void *, const void *, int);
void func_089038F4(void *);
extern const Tbl0890A7A8 D_089D82E0;
extern const Tbl0890A7A8 D_089D8320;

void func_0890A7A8(C0890A7A8 *self)
{
    Tbl0890A7A8 enter;
    Tbl0890A7A8 tick;
    int unused[1];
    func_08935524(&enter, &D_089D82E0, sizeof(enter));
    func_08935524(&tick, &D_089D8320, sizeof(tick));
    int k = self->cur;
    if (k != self->prev) {
        if (enter.h[k]) {
            (self->*enter.h[k])();
            k = self->cur;
        }
        self->prev = k;
    }
    func_089038F4(self->sub);
    if (tick.h[self->cur]) {
        (self->*tick.h[self->cur])();
    }
}
}
