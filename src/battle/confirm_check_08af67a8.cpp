class InputBase {
public:
    char data_00[0x34];
    virtual void slot_008();
    virtual int isTriggered(long long mask);
};

class InputDevice : public InputBase {
};

extern "C" {

extern long long D_08AFB910;
extern InputDevice *func_088729A0(int port);
extern void func_088A24D4(int a, int sound, int b, int c);
extern void func_08AF66CC(void *self);

typedef struct ConfirmState {
    char pad_00[0x54];
    int state;
    char pad_58[0x18];
    unsigned char ready;
} ConfirmState;

void func_08AF67A8(ConfirmState *self)
{
    if (self->ready == 1) {
        InputDevice *input = func_088729A0(0);
        if (input->isTriggered(D_08AFB910)) {
            func_088A24D4(0, 0x2711, 1, 0);
            func_08AF66CC(self);
        }
    } else {
        self->state = 4;
    }
}

}
