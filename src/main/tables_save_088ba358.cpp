extern "C" int func_088B7A88(int, int, int);
extern "C" void func_088B7D94(void *, int, int);
extern "C" int func_088B7AB8(int, int, int, int);
extern "C" void func_088B7DA4(void *, int, int, int);
struct E98 { char b[0x1E]; };
extern "C" E98 D_09B4E7C8[3];
extern "C" unsigned char D_089D7370;
extern "C" void func_088BA358(int src);
extern "C" void func_088BA358(int src)
{
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 12; j++) {
            int v = func_088B7A88(src, i, j);
            func_088B7D94(&D_09B4E7C8[i], j, v);
        }
        for (int j = 0; j < 6; j++) {
            for (int k = 0; k < 3; k++) {
                int v = func_088B7AB8(src, i, j, k);
                func_088B7DA4(&D_09B4E7C8[i], j, k, v);
            }
        }
    }
    D_089D7370 = 1;
}
