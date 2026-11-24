/* Runs func_08AF2948, clears word +0xD8 and sets byte +5 of D_09B6CDA0, then func_08AF2470(a0, 0). */
typedef struct { char p0[5]; unsigned char f5; char p1[0xD8 - 6]; int w; } Obj;
extern Obj D_09B6CDA0;
extern void func_08AF2948(void);
extern void func_08AF2470(int a, int b);
void func_08AF242C(int a) {
    func_08AF2948();
    D_09B6CDA0.w = 0;
    D_09B6CDA0.f5 = 1;
    func_08AF2470(a, 0);
}
