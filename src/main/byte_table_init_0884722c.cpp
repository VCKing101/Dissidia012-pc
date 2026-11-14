struct BytePair {
    unsigned char a;
    unsigned char b;
};

/* 16 pairs followed by four 16-byte rows at 0x20, 0x30, 0x40, 0x50. */
struct ByteTable {
    BytePair pairs[16];
    unsigned char rows[4][16];
};

extern "C" {

void func_0884722C(ByteTable *self)
{
    for (int i = 0; i < 16; i++) {
        self->pairs[i].a = 0xFF;
        self->pairs[i].b = 0;
    }
    for (int i = 0; i < 16; i++) {
        *((unsigned char *)self + 0x20 + i) = 0xFF;
        *((unsigned char *)self + 0x30 + i) = 0xFF;
        *((unsigned char *)self + 0x40 + i) = 0xFF;
        *((unsigned char *)self + 0x50 + i) = 0xFF;
    }
}

}
