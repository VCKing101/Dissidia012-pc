extern "C" {
extern unsigned int *D_089D726C;
}

struct Id48610 {
    static int IsKnown(unsigned int id) {
        return (D_089D726C[id >> 5] & (1 << (id & 0x1F))) != 0;
    }
};

extern "C" int func_08A48610(int self, unsigned int id)
{
    if (id == 0xFFFF) {
        return 0;
    }
    if (id == 0xFFFFFFFF) {
        return 0;
    }
    int known = Id48610::IsKnown(id);
    if (known == 1) {
        return 1;
    }
    return -1;
}
