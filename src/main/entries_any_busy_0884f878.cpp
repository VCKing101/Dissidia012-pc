/* An object with its vptr after 0x544 bytes of data; virtual 26 says whether it is busy. */
struct Obj0884F878 {
    char data[0x544];
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03();
    virtual void v04(); virtual void v05(); virtual void v06(); virtual void v07();
    virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15();
    virtual void v16(); virtual void v17(); virtual void v18(); virtual void v19();
    virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23();
    virtual void v24(); virtual void v25();
    virtual int busy();                /* 26 */
};

struct Entry0884F878 {
    unsigned char unk_00[8];
    Obj0884F878 *obj;                  /* +0x08 */
    unsigned char unk_0C[6];
    short id;                          /* +0x12 0 = unused */
    unsigned char unk_14[0x20 - 0x14];

    static unsigned char unused(Entry0884F878 *e) { return e->id == 0; }
};

/* 1 when one of the 16 used entries has a busy object. */
extern "C" int func_0884F878(Entry0884F878 *entries)
{
    int i = 0;

    for (Entry0884F878 *e = &entries[i]; i < 16; e++, i++) {
        if (!Entry0884F878::unused(e)) {
            if (entries[i].obj->busy()) {
                return 1;
            }
        }
    }
    return 0;
}
