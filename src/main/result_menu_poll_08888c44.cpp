/* Updates the result menu and, once it is idle or finished (state 0 or 4),
   runs the follow-up step. */
extern "C" {

struct ResultMenu {
    char pad_00[0xB0];
    int state;

    static unsigned char IsDone(ResultMenu *m)
    {
        int done = 0;
        if (m->state == 0 || m->state == 4) {
            done = 1;
        }
        return done;
    }
};

extern ResultMenu *D_08B0CF20;

int func_08B05DEC(ResultMenu *menu);
void func_0888A7E0(void);

void func_08888C44(void)
{
    func_08B05DEC(D_08B0CF20);
    if (ResultMenu::IsDone(D_08B0CF20)) {
        func_0888A7E0();
    }
}

}
