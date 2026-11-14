class Forward2AD9C {
public:
    char data[0x8];
    virtual int slot_008(int arg);
};

extern "C" {

int func_08A2AD9C(Forward2AD9C *self, int arg)
{
    return self->slot_008(arg);
}

}
