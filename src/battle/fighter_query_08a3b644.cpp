class Fighter3B644 {
public:
    char data_000[0x544];
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
    virtual void slot_070();
    virtual void slot_078();
    virtual void slot_080();
    virtual void slot_088();
    virtual void slot_090();
    virtual void slot_098();
    virtual void slot_0A0();
    virtual void slot_0A8();
    virtual void slot_0B0();
    virtual void slot_0B8();
    virtual void slot_0C0();
    virtual void slot_0C8();
    virtual void slot_0D0();
    virtual void slot_0D8();
    virtual void slot_0E0();
    virtual void slot_0E8();
    virtual void slot_0F0();
    virtual void slot_0F8();
    virtual void slot_100();
    virtual void slot_108();
    virtual void slot_110();
    virtual void slot_118();
    virtual void slot_120();
    virtual void slot_128();
    virtual void slot_130();
    virtual void slot_138();
    virtual int slot_140();
};

struct Owner3B644 {
    Fighter3B644 *fighter;
};

extern "C" {

extern int D_08A405E0;
extern void func_08A3DBD0(int id, int value);

void func_08A3B644(Owner3B644 *self)
{
    if (self->fighter->slot_140()) {
        func_08A3DBD0(D_08A405E0, 1);
    }
}

}
