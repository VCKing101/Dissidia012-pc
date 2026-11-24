/* Calls func_088A9684 on the battle object, then func_088A7234 when slot's field at +0xC is set. */
struct SlotRec { int a, b, c, handle; };
extern struct SlotRec D_09B6C288[];
extern void *D_089D6DC8;
extern void func_088A9684(void *p);
extern void func_088A7234(void);

void func_08AFCA28(int slot)
{
    func_088A9684(D_089D6DC8);
    if (D_09B6C288[slot].handle != 0) {
        func_088A7234();
    }
}
