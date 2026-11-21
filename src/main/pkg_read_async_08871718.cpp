struct Device08871718 {
    char data[0x154];
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03();
    virtual void v04(); virtual void v05(); virtual void v06(); virtual void v07();
    virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15();
    virtual void v16(); virtual void v17(); virtual void v18(); virtual void v19();
    virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23();
    virtual void v24(); virtual void v25(); virtual void v26(); virtual void v27();
    virtual void seek_async(int file, unsigned int offset, int whence, void (*done)(void *), void *arg);   /* 28 */
};

struct Entry08871718 {
    unsigned int crc;                  /* +0x00 */
    unsigned int sector;               /* +0x04 low 20 bits */
    unsigned int size;                 /* +0x08 low 28 bits */
};

struct Package08871718 {
    int file;                          /* +0x00 */
    Device08871718 *device;            /* +0x04 */
    unsigned char unk_08[0x5C - 0x8];
    Entry08871718 *entry;              /* +0x5C */
    void *dst;                         /* +0x60 */
    unsigned int size;                 /* +0x64 */
    int a;                             /* +0x68 */
    int b;                             /* +0x6C */
    unsigned char done;                /* +0x70 */
    unsigned char raw;                 /* +0x71 */
};

extern "C" {
unsigned int func_088716F0(Package08871718 *pkg, Entry08871718 *entry);   /* unpacked size */
void func_08871AB0(void *pkg);                                          /* seek done */

/* Start an asynchronous PACKAGE.BIN read of `entry` into dst: seek to its sector (0x800-byte
 * sectors), the read follows in func_08871AB0. 0 without an entry. */
int func_08871718(Package08871718 *pkg, Entry08871718 *entry, void *dst, int unused, int a, int b, unsigned char raw)
{
    if (entry == 0) {
        return 0;
    }
    unsigned int offset = (entry->sector & 0xFFFFF) * 0x800;
    unsigned int size;

    pkg->entry = entry;
    pkg->dst = dst;
    if (raw != 0) {
        size = entry->size & 0x0FFFFFFF;
    } else {
        size = func_088716F0(pkg, entry);
    }
    pkg->size = size;
    pkg->a = a;
    pkg->b = b;
    pkg->raw = raw;
    pkg->done = 0;
    pkg->device->seek_async(pkg->file, offset, 0, func_08871AB0, pkg);
    return 1;
}
}
