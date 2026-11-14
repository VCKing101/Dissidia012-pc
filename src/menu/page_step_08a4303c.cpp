struct Cursor4303C {
    int pad_00;
    int index;
    int count;
};

extern "C" {
extern Cursor4303C D_09B6DA68;
extern int D_08A457F0;
extern void func_08A43538(int v);
extern void func_08A43094(void);
extern void func_088E991C(int a, int b);
}

extern "C" void func_08A4303C(void)
{
    if (D_09B6DA68.count >= D_09B6DA68.index) {
        func_08A43538(D_08A457F0);
    } else {
        func_08A43094();
        func_088E991C(2, 0);
    }
}
