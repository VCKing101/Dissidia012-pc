class InputBase {
public:
    char data_00[0x34];
    virtual void slot_008();
    virtual int isTriggered(long long mask);
};

class InputDevice : public InputBase {
};

typedef struct ConfirmPanel {
    char pad_00[0x4];
    char field_04[0x24];
    int state;
} ConfirmPanel;

extern "C" {

extern long long D_08AFC600;
extern InputDevice *func_088729A0(int port);
extern void func_088EC6BC(void *fade, void (*callback)(ConfirmPanel *), ConfirmPanel *owner);
extern void func_08AFC4DC(ConfirmPanel *owner);

void func_08AFC2EC(ConfirmPanel *self)
{
    InputDevice *input = func_088729A0(0);
    if (input->isTriggered(D_08AFC600)) {
        func_088EC6BC(self->field_04, func_08AFC4DC, self);
        self->state = 4;
    }
}

}
