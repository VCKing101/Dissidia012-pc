struct Header294D0 {
    char pad_00[2];
    unsigned short counts[5];
    char pad_0C[4];
};

struct Item294D0 {
    char data[0x1C];
};

extern "C" {
extern Item294D0 *D_09B6D9F8[5];
extern Header294D0 *D_08A29BF0;
extern Item294D0 *D_08A29BF4;
}

extern "C" void func_08A294D0(Header294D0 *header, int count)
{
    int i;
    for (i = 0; i < 5; i++) {
        D_09B6D9F8[i] = 0;
    }
    if (count > 0) {
        Item294D0 *p;
        D_08A29BF0 = header;
        p = (Item294D0 *)(header + 1);
        D_09B6D9F8[0] = p;
        p += header->counts[0];
        D_09B6D9F8[1] = p;
        p += header->counts[1];
        D_09B6D9F8[2] = p;
        p += header->counts[2];
        D_09B6D9F8[3] = p;
        p += header->counts[3];
        D_09B6D9F8[4] = p;
        D_08A29BF4 = p + header->counts[4];
    }
}
