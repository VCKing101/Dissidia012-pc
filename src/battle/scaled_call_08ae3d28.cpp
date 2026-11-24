/* Calls func_08AFE134 with a scale from D_08B4AF14 times a signed short from the save block, over 100. */
extern "C" {
struct Z { char pad[2]; short b; };
struct Y { char pad[0x1A]; Z z; };
struct X { char pad[0xC]; Y y; };
struct Blk { char pad[0x25A8]; X x; };
extern Blk D_09AFB8A0;
extern float *D_08B4AF14;
extern int D_08AE7CA0;
extern void func_08AFE134(float scale, int arg);

void func_08AE3D28(void)
{
    X *x = &D_09AFB8A0.x;
    Y *y = &x->y;
    Z *z = &y->z;
    func_08AFE134(*D_08B4AF14 * (float)(int)z->b / 100.0f, D_08AE7CA0);
}
}
