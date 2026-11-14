/* Rule queries for a side: whether an item is listed, whether the side's
 * fighter may still act, and an item's value. */
struct RuleEntry {
    unsigned char data[0x28];
};

struct RuleInfo {
    unsigned char pad_00[0xC];
    unsigned short first;
    unsigned short count;
    unsigned char pad_10[0x48];
};

struct RuleItem {
    unsigned char pad_00[0x3D];
    signed char id;
    unsigned char pad_3e;
    signed char value;
};

struct ActUnit {
    unsigned char pad_0000[0x10D0];
    int acting;
};

struct RuleBook {
    unsigned char open;
    unsigned char pad_001[0x487];
    RuleEntry *entries;
};

extern "C" {

extern int func_088916A4(RuleEntry *entry, RuleInfo *info);
extern int func_08891894(RuleEntry *entry, RuleItem *item, int index);
extern int func_08891684(RuleEntry *entry);
extern int func_08891944(RuleEntry *entry);
extern ActUnit *func_08A22C78(int side);
extern int func_08B3390C(ActUnit *unit);

float func_08A22830(RuleBook *book, unsigned int side, int id)
{
    if (!book->open) {
        return 0.0f;
    }
    if (side >= 2 || id < 0) {
        return 0.0f;
    }
    RuleEntry *entry = &book->entries[side];
    RuleInfo info;
    if (func_088916A4(entry, &info) != 0) {
        return 0.0f;
    }
    int found = 0;
    RuleItem item;
    for (int i = 0; i < info.count; i++) {
        if (func_08891894(entry, &item, info.first + i) != 0) {
            continue;
        }
        if (item.id < 0) {
            continue;
        }
        if (item.id != id) {
            continue;
        }
        found = 1;
        break;
    }
    if (!found) {
        return 0.0f;
    }
    return (float)item.value;
}

}
