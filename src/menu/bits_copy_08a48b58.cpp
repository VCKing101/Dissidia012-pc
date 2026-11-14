/* For every link whose source bit is set, sets the target bit. */
struct BitLink {
    short field_00;
    short field_02;
    short field_04;
};

struct BitLinksHead {
    char pad_0[0x8];
    short field_08;
};

struct BitLinks {
    BitLinksHead *field_00;
    BitLink *field_04;
    static bool test(unsigned int *bits, unsigned int id) { return (bits[id >> 5] & (1 << (id & 0x1F))) != 0; }
    static void set(unsigned int *bits, unsigned int id) { bits[id >> 5] |= 1 << (id & 0x1F); }
};

extern "C" {
void func_08A48B58(void *self, unsigned int *target, unsigned int *source, BitLinks *links)
{
    int count = links->field_00->field_08;
    for (int i = 0; i < count; i++) {
        int from = links->field_04[i].field_04;
        if (from >= 0 && BitLinks::test(source, from) == true) {
            BitLinks::set(target, links->field_04[i].field_00);
        }
    }
}
}
