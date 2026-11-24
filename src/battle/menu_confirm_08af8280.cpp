// Menu confirm: when the input query succeeds and the page change is accepted, plays a cue and moves to the end state.
extern "C" {
void *func_088729A0(int a);
int func_08AF839C(void *self, int a);
void func_088A24D4(int a, int b, int c, int d);
void func_08AF3D40(void *self);
extern unsigned long long D_08AFB910;
}

struct Input {
    char data[0x34];
    virtual void v0();
    virtual int query(unsigned long long mask);
};

struct Menu {
    char d0[0x24];
    int count;
    int cur;
    char d2c[0x14];
    int state;
    char d44;
    unsigned char mode;
};

extern "C" void func_08AF8280(Menu *m) {
    Input *in = (Input *)func_088729A0(0);
    if (in->query(D_08AFB910) != 0) {
        int one = 1;
        if (func_08AF839C(m, 1) == one) {
            func_088A24D4(0, 0x2711, 1, 0);
            if (m->mode == one) {
                if (m->cur == m->count - 1) {
                    func_08AF3D40(m);
                    m->state = 4;
                }
            } else if (m->count <= m->cur) {
                func_08AF3D40(m);
                m->state = 3;
            }
        }
    }
}
