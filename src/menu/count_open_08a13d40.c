typedef struct OpenInner {
    char pad_000[0xCE8];
    unsigned char field_CE8[1];
    char pad_CE9[0xEF0 - 0xCE9];
    unsigned int field_EF0;
    char pad_EF4[0xF00 - 0xEF4];
    short field_F00[32];
    short field_F40;
} OpenInner;

typedef struct OpenOwner {
    char pad_000[0xF60];
    OpenInner *field_F60;
} OpenOwner;

void func_08A13D40(OpenOwner *self)
{
    int open = 0;
    int i = 0;
    OpenInner *inner = self->field_F60;
    if (i < inner->field_F40) {
        OpenInner *cursor = inner;
        do {
            if ((inner->field_CE8[cursor->field_F00[0] * 0x1E] & 4) == 0) {
                open++;
            }
            i++;
            cursor = (OpenInner *)((char *)cursor + 2);
        } while (i < inner->field_F40);
    }
    if (open == 0) {
        inner->field_EF0 = inner->field_EF0 | 8;
    }
}
