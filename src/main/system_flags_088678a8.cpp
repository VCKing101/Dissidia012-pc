struct Agent2SystemInfo {
    unsigned char unknown_0000[0xC];
    unsigned short field_000C;
};

extern "C" Agent2SystemInfo D_09B26AB8;

extern "C" bool func_088678A8(int bit) {
    Agent2SystemInfo *info = &D_09B26AB8;
    if (info->field_000C & (1 << bit)) return true;
    return false;
}
