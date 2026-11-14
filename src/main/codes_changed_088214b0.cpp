/* true when any item code or the main code differs from 100. */
struct Item088214B0 {
    signed char field_00;
    signed char code;
    signed char field_02;
};

struct Set088214B0 {
    signed char field_00;
    signed char count;
    char pad_02[0xA];
    Item088214B0 *items;
    char pad_10;
    signed char code;
};

extern "C" {

bool func_088214B0(Set088214B0 *set)
{
    for (int i = 0; i < set->count; i++) {
        if (set->items[i].code != 100) {
            return true;
        }
    }
    if (set->code != 100) {
        return true;
    }
    return false;
}

}
