/* Returns the first unused of the 45 slots, or 0. */
struct Slot088A5A50 {
    char pad_00[0x64];
    unsigned char used;
    char pad_65[3];
};

struct Pool088A5A50 {
    Slot088A5A50 slots[45];
};

extern "C" {

Slot088A5A50 *func_088A5A50(Pool088A5A50 *p)
{
    for (int i = 0; i < 45; i++) {
        if (p->slots[i].used == 0) {
            return &p->slots[i];
        }
    }
    return 0;
}

}
