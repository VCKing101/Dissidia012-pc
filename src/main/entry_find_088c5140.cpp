/* Loaded 0x1C-byte entry whose slot byte matches; 0 when none. */
struct Header088C5140 {
    short field_00;
    unsigned short count;
};

struct Entry088C5140 {
    unsigned char kind;
    unsigned char slot;
    char pad_02[0x1A];
};

extern "C" {

extern Header088C5140 *D_089D76D8;
extern Entry088C5140 *D_089D76DC;

Entry088C5140 *func_088C5140(unsigned int slot)
{
    if (D_089D76DC != 0) {
        for (int i = 0; i < D_089D76D8->count; i++) {
            if (D_089D76DC[i].slot == slot) {
                return &D_089D76DC[i];
            }
        }
    }
    return 0;
}

}
