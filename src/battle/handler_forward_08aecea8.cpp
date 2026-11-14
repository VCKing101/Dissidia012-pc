class Handler {
public:
    int field_00;
    virtual void slot_008();
    virtual void slot_010();
    virtual void slot_018(int a, int b, int c);
};

extern "C" void func_08AECEA8(Handler *self, int a, int b, int c)
{
    self->slot_018(a, b, c);
}
