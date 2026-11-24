/* Clears the 0x108-byte record (if given), looks up info for id with func_088C50D4 and fills its halfwords at 4/6 and its handle (func_088F4900(0x40, &r[8], info, flag)) at 0. */
typedef struct Rec088f4454 { void *handle; unsigned short a; unsigned short b; unsigned char body[0x100]; } Rec088f4454;
typedef struct Info088f4454 { unsigned char pad0[8]; unsigned short a; unsigned char pad1[0x1A - 0xA]; unsigned short b; } Info088f4454;
extern Info088f4454 *func_088C50D4(int id);
extern void *func_088F4900(int a, void *body, Info088f4454 *info, unsigned char flag);
extern void func_08935828(void *p, int v, int n);
void func_088F4454(Rec088f4454 *r, int id, unsigned char flag)
{
    Info088f4454 *info;
    if (r != 0) {
        func_08935828(r, 0, 0x108);
    }
    info = func_088C50D4(id);
    if (info != 0) {
        r->a = info->a;
        r->b = info->b;
        r->handle = func_088F4900(0x40, r->body, info, flag);
    }
}
