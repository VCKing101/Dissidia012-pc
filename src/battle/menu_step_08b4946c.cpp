// Menu step handler: polls a list, moves to state 1 on selection change, else key masks start state 3 or 2/3.
struct Mgr08B4946C {
    char pad[0x34];
    virtual int v0(unsigned long long k);
    virtual int v1(unsigned long long k);
};
struct Menu08B4946C {
    int state;
    char pad0[0x20];
    Mgr08B4946C *mgr;
    int mode;
    int list;
    int sel;
};
extern "C" {
extern unsigned long long D_08B49880;
extern unsigned long long D_08B49888;
void func_088D688C(void *, int);
void func_088D4BF4(int);
void func_08B49328(void *);
void func_08B49598(void *, int);
void func_08B4946C(Menu08B4946C *self)
{
    int old;
    self->mode = 0;
    old = self->sel;
    func_088D688C(&self->list, 0);
    if (old != self->sel) {
        func_08B49598(self, self->sel);
        self->mode = 1;
    } else {
        if (self->mgr->v1(D_08B49880)) {
            func_088D4BF4(2);
            self->mode = 3;
            func_08B49328(self);
            self->state = 1;
        } else if (self->mgr->v1(D_08B49888)) {
            if (self->sel == 0) {
                func_088D4BF4(2);
                self->mode = 3;
            } else {
                func_088D4BF4(1);
                self->mode = 2;
            }
            func_08B49328(self);
            self->state = 1;
        }
    }
}
}
