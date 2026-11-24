// Resets a stats block: two sub-records, three counters at 0x54, 22 at 0x60, a 999999999 cap at 0xB8 and three more at 0xBC.
struct Stats47 {
    char pad[0x54];
    int a[3];
    int b[0x1A];
};
extern "C" void func_088B2BCC(void *);
extern "C" void func_088B2CFC(void *);
extern "C" void func_088B2ED0(Stats47 *p)
{
    func_088B2BCC(p);
    func_088B2CFC((char *)p + 0x24);
    func_088B2CFC((char *)p + 0x3C);
    for (int i = 0; i < 3; i++) p->a[i] = 0;
    for (int i = 0; i < 0x16; i++) p->b[i] = 0;
    int cap = 999999999;
    for (int i = 0x16; i < 0x17; i++) p->b[i] = cap;
    for (int i = 0x17; i < 0x1A; i++) p->b[i] = 0;
}
