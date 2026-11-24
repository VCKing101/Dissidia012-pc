/* Runs the callback for the mode's current step (0-4), stopping the menu
   sound first when that step asks for it; other steps close the screen. */
typedef void (*StepFn)(void);

typedef struct ModeCtl {
    char pad_000[0x50];
    unsigned int step;
    char pad_054[0x744 - 0x54];
    StepFn on_step[3];
    unsigned char quiet[3];
} ModeCtl;

extern int D_089D85E8;
extern int D_089E75C4;

extern void func_0888B6DC(void);
extern void func_08933C00(int sound);
extern void func_089E0BDC(int screen, int flag);

void func_08889CDC(ModeCtl *self)
{
    StepFn fn;
    int screen;
    int none;

    switch (self->step) {
    case 0:
        func_0888B6DC();
        if (self->on_step[0] != 0) {
            if (self->quiet[0]) {
                func_08933C00(D_089D85E8);
            }
            self->on_step[0]();
        }
        break;
    case 1:
        func_0888B6DC();
        if (self->on_step[1] != 0) {
            if (self->quiet[1]) {
                func_08933C00(D_089D85E8);
            }
            self->on_step[1]();
        }
        break;
    case 2:
        func_0888B6DC();
        fn = self->on_step[2];
        if (fn != 0) {
            if (self->quiet[2]) {
                func_08933C00(D_089D85E8);
                fn = self->on_step[2];
            }
            fn();
        }
        break;
    case 3:
    case 4:
        fn = self->on_step[2];
        if (fn != 0) {
            if (self->quiet[2]) {
                func_08933C00(D_089D85E8);
                fn = self->on_step[2];
            }
            fn();
        }
        break;
    default:
        func_08933C00(D_089D85E8);
        screen = D_089E75C4;
        none = 0;
        func_089E0BDC(screen, none);
        break;
    }
}
