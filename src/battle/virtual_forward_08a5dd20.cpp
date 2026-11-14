class Forward5DD20 {
public:
    char data[0x8];
    virtual int slot_008(int arg);
};

extern "C" {

int func_08A5DD20(Forward5DD20 *self, int arg)
{
    return self->slot_008(arg);
}

}
