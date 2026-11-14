struct Entry088931DC {
    short id;
    short field_02;
    unsigned char field_04;
    unsigned char field_05;
    unsigned char value;
    unsigned char unknown_07;
    unsigned char field_08;
    unsigned char unknown_09;
};

struct Owner088931DC {
    unsigned char unknown_000[0x612];
    Entry088931DC entries[16];
};

extern "C" {
unsigned char func_088931DC(Owner088931DC *self, int id) {
    for (int i = 0; i < 16; i++) {
        if (self->entries[i].id == id) {
            return self->entries[i].value;
        }
    }
    return 0xFF;
}
}
