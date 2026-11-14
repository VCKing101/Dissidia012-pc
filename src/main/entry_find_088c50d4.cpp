/* Loaded 0x1C-byte entry whose kind byte matches; 0 when none. */
struct Header088C50D4 {
    short field_00;
    unsigned short count;
};

struct Entry088C50D4 {
    unsigned char kind;
    unsigned char slot;
    char pad_02[0x1A];
};

extern "C" {

extern Header088C50D4 *D_089D76D8;
extern Entry088C50D4 *D_089D76DC;

Entry088C50D4 *func_088C50D4(unsigned int kind)
{
    if (D_089D76DC != 0) {
        for (int i = 0; i < D_089D76D8->count; i++) {
            if (D_089D76DC[i].kind == kind) {
                return &D_089D76DC[i];
            }
        }
    }
    return 0;
}

}
