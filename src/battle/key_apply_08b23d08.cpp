extern "C" {

extern unsigned long long D_08B27870;

class KeyTarget {
public:
    char data[0x34];
    virtual int test(unsigned long long key);
    virtual void apply(unsigned long long key);
};

struct KeyHolder {
    KeyTarget *target;
};

void func_08B23D08(KeyHolder *holder)
{
    holder->target->apply(D_08B27870);
}

}
