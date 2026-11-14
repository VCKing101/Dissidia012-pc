extern "C" {

extern unsigned long long D_08B27868;

class KeyTarget {
public:
    char data[0x34];
    virtual int test(unsigned long long key);
    virtual void apply(unsigned long long key);
};

struct KeyHolder {
    KeyTarget *target;
};

int func_08B23CBC(KeyHolder *holder)
{
    return holder->target->test(D_08B27868) == 1;
}

}
