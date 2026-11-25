// Runs func_08ADFD3C on the sub-object at +0x2A8 of both entries in the battle table.
extern int D_089D69F8;
extern "C" void func_08ADFD3C(int p);
extern "C" void func_08AE3C14(void)
{
    for (int i = 0, off = 0; i < 2; i++, off += 4) {
        func_08ADFD3C(*(int *)(D_089D69F8 + off + 0x18) + 0x2A8);
    }
}
