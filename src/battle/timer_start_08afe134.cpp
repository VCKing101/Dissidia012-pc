class TimerBase {
public:
    int field_00;
    int field_04;
    int field_08;
    int field_0C;
    float duration;
    char pad_14[0x10];
    virtual void slot_008();
    virtual void slot_010();
    virtual void slot_018();
    virtual void slot_020();
    virtual void slot_028();
    virtual void slot_030();
    virtual void slot_038();
    virtual void slot_040();
    virtual void slot_048();
    virtual void slot_050();
    virtual void slot_058();
    virtual void slot_060();
    virtual void slot_068();
    virtual void start();
};

class TimerTask : public TimerBase {
};

extern "C" {

void func_08AFE134(float duration, TimerTask *self)
{
    self->field_08 = 0;
    self->duration = duration;
    self->start();
}

}
