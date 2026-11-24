/* Stores five parameters into the record at D_09B6DA18 then calls func_088650A4(a0 + 0x100). */
typedef struct { int a; int b; unsigned char c; int d; int e; } Rec;
extern Rec D_09B6DA18;
extern void func_088650A4(int p);
void func_08AC481C(int p, int b, int a, unsigned char c, int d, int e) {
    D_09B6DA18.b = b;
    D_09B6DA18.a = a;
    D_09B6DA18.c = c;
    D_09B6DA18.d = d;
    D_09B6DA18.e = e;
    func_088650A4(p + 0x100);
}
