struct Cursor42EF0 {
    int pad_00;
    int index;
    int count;
};

extern "C" {
extern Cursor42EF0 D_09B6DA68;
extern int func_08A3C2B8(int id);
extern int func_08A3C0C4(int index);
extern void func_0893476C(void *self, int a, int b);
}

extern "C" void func_08A42EF0(void *self, unsigned short *ids)
{
    if (D_09B6DA68.count >= D_09B6DA68.index) {
        int row = func_08A3C2B8(ids[D_09B6DA68.index]);
        func_0893476C(self, func_08A3C0C4(3), row + 0x18);
    }
}
