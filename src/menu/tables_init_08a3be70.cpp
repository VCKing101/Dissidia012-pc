struct Header3BE70 {
    char pad_00[0x16];
    unsigned short sizes[9];
    char pad_28[8];
};

extern "C" {
extern Header3BE70 *D_08A3CCF0;
extern char *D_09B6DA98[9];
extern char *D_08A3CD00;
extern void func_08A3BF2C(Header3BE70 *header);
}

extern "C" void func_08A3BE70(Header3BE70 *h0, int c0)
{
    int count = c0;
    Header3BE70 *header = h0;
    func_08A3BF2C(header);
    if (count > 0) {
        char *p;
        D_08A3CCF0 = header;
        p = (char *)header + 0x28;
        D_09B6DA98[0] = p;
        p += header->sizes[0];
        D_09B6DA98[1] = p;
        p += header->sizes[1];
        D_09B6DA98[2] = p;
        p += header->sizes[2];
        D_09B6DA98[3] = p;
        p += header->sizes[3];
        D_09B6DA98[4] = p;
        p += header->sizes[4];
        D_09B6DA98[5] = p;
        p += header->sizes[5];
        D_09B6DA98[6] = p;
        p += header->sizes[6];
        D_09B6DA98[7] = p;
        p += header->sizes[7];
        D_09B6DA98[8] = p;
        p += header->sizes[8];
        D_08A3CD00 = p;
    }
}
