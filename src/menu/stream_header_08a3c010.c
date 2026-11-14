typedef struct MenuStreamHeader {
    char pad_00[0xC];
    unsigned short field_0C;
    unsigned short field_0E;
    unsigned short field_10;
    unsigned short field_12;
    unsigned short field_14;
} MenuStreamHeader;

extern MenuStreamHeader *D_08A3CCF0;

unsigned short func_08A3C010(void) {
    if (D_08A3CCF0 != 0) {
        return D_08A3CCF0->field_0C;
    }
    return 0;
}

unsigned short func_08A3C034(void) {
    if (D_08A3CCF0 != 0) {
        return D_08A3CCF0->field_0E;
    }
    return 0;
}

unsigned short func_08A3C058(void) {
    if (D_08A3CCF0 != 0) {
        return D_08A3CCF0->field_10;
    }
    return 0;
}

unsigned short func_08A3C07C(void) {
    if (D_08A3CCF0 != 0) {
        return D_08A3CCF0->field_12;
    }
    return 0;
}

unsigned short func_08A3C0A0(void) {
    if (D_08A3CCF0 != 0) {
        return D_08A3CCF0->field_14;
    }
    return 0;
}
