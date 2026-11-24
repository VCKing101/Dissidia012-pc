/* Initialises a "FRNDCARD" record: tag, version 0x20101007, sub-object at 0xC, and clears the rest of its fields. */
typedef struct Rec088f4b68 {
    char tag[8]; unsigned version; unsigned char sub[0xC]; int a18, a1c, a20; short h24; char b26, b27; short h28, h2a; int a2c; short h30;
    unsigned char pad[0x4A - 0x32]; short h4a;
} Rec088f4b68;
extern char D_089CCD20[];
extern void func_08935524(void *dst, void *src, int n);
extern void func_088F4B24(void *sub);
void func_088F4B68(Rec088f4b68 *r)
{
    func_08935524(r, D_089CCD20, 8);
    r->version = 0x20101007;
    func_088F4B24(r->sub);
    r->a18 = 0;
    r->a1c = 0;
    r->a20 = 0;
    r->h24 = 0;
    r->b26 = 0;
    r->b27 = 0;
    r->h28 = 0;
    r->a2c = 0;
    r->h30 = 0;
    r->h4a = 0;
    r->h2a = 0;
}
