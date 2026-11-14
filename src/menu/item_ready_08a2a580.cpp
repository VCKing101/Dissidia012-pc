/* Returns 0 while a kind-3 or kind-4 item's level is below the required level plus its offset, otherwise 1. */
struct ReadyItem {
    char pad_00[0x8];
    unsigned short field_08;
    char pad_0A[0x8];
    unsigned short field_12;
    char pad_14[0x2];
    unsigned short field_16;
    unsigned short field_18;
    static bool isNone(unsigned short value) { return value == 0xFFFF; }
};

struct ReadyState {
    char pad_0000[0x151C];
    int field_151C;
    int field_1520;
};

struct ReadyOwner {
    char pad_000[0x138];
    ReadyState *field_138;
};

extern "C" {
int func_08A2A580(ReadyOwner *self, ReadyItem *item)
{
    int ready = 1;
    if (item->field_12 == 0xFFFF) {
        unsigned short level = item->field_08;
        if (!ReadyItem::isNone(level)) {
            if (item->field_18 == 3) {
                if (level < self->field_138->field_151C + item->field_16) {
                    ready = 0;
                }
            } else if (item->field_18 == 4) {
                if (level < self->field_138->field_1520 + item->field_16) {
                    ready = 0;
                }
            }
        }
    }
    return ready;
}
}
