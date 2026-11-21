/* Types shared by agent37's main sources (0x08846CCC-0x08886744). */
#ifndef AGENT37_TYPES_H
#define AGENT37_TYPES_H

#ifdef __cplusplus
/* The file system device (D_089D5EA0->device): a vptr after 0x154 bytes of data. Only the
 * vtable slots used by the "data/<name>" wrappers (0x088633FC-0x0886350C) are named, by number. */
struct D37Device {
    char data[0x154];
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03();
    virtual void v04(); virtual void v05(); virtual void v06();
    virtual void op07(const char *path, int a, int b, int c, int d);  /* 7 */
    virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual void op12(const char *path);                          /* 12 */
    virtual void v13(); virtual void v14(); virtual void v15();
    virtual void op16(const char *path, int a, int b);            /* 16 */
    virtual void v17(); virtual void v18(); virtual void v19(); virtual void v20();
    virtual void op21(const char *path);                          /* 21 */
    virtual void v22(); virtual void v23();
    virtual void op24(const char *path, int a, int b);            /* 24 */
};
#endif

#endif
