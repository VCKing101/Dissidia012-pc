struct Scene0884BF0C {
    unsigned char unk_000[0x278];
    int mode;                          /* +0x278 */
};

struct DrawMode0884BF0C {
    /* Modes other than 0 and 1 draw something else. */
    static unsigned char other(Scene0884BF0C *scene) {
        int r = 0;
        if (scene->mode != 0 && scene->mode != 1) {
            r = 1;
        }
        return r;
    }
};

extern "C" {
extern Scene0884BF0C *D_08A405E0;
int func_089A04E8(void);
void func_089A296C(int a);
void func_089A250C(void);
void func_089A2568(void);
void func_089A25C0(void);
void func_0899FDC4(int a);
void func_089A0384(int a);
void func_089A119C(int a);
void func_089A0C8C(int a);
void func_0884BE74(void *self, int a);

void func_0884BF0C(void *self)
{
    int saved = func_089A04E8();

    if (DrawMode0884BF0C::other(D_08A405E0)) {
        return;
    }
    func_089A296C(2);
    func_089A250C();
    func_089A2568();
    func_089A25C0();
    func_0899FDC4(0);
    func_089A0384(1);
    func_089A0384(4);
    func_089A0384(9);
    func_089A119C(1);
    func_089A0C8C(-1);
    func_0884BE74(self, 0);
    func_089A119C(0);
    func_0899FDC4(saved);
}
}
