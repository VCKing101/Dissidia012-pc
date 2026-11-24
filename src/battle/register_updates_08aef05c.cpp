/* Registers the Update, DrawTypeMap and DrawTypeField callbacks, then initialises the two 0x50-byte records at 0x09B49140. */
extern "C" {
struct Rec { char pad[0x50]; };
extern Rec D_09B49140[];
extern char D_08AEF1F0[];
extern char D_08AEF1F8[];
extern char D_08AEF204[];
extern void func_088823C4(int owner, int a, int id, void *fn, char *name);
extern void func_08AEF12C(void);
extern void func_08AEF17C(void);
extern void func_08AEF19C(void);
extern void func_08AEEFC8(Rec *rec);

void func_08AEF05C(int owner)
{
    func_088823C4(owner, -1, 0xDBEC, (void *)func_08AEF12C, D_08AEF1F0);
    func_088823C4(owner, -1, 0x11012, (void *)func_08AEF17C, D_08AEF1F8);
    func_088823C4(owner, -1, 0x18704, (void *)func_08AEF19C, D_08AEF204);
    for (int i = 0; i < 2; i++) {
        func_08AEEFC8(&D_09B49140[i]);
    }
}
}
