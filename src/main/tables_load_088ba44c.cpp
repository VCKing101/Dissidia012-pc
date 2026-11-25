extern "C" int func_088B7DC0(void *, int, int);
extern "C" void func_088B75E8(int, int, int, int);
extern "C" int func_088B7DF0(void *, int, int, int);
extern "C" void func_088B7618(int, int, int, int, int);
struct E98 { char b[0x1E]; };
extern "C" E98 D_09B4E7C8[3];
extern "C" unsigned char D_089D7370;
extern "C" void func_088BA44C(int dst);
extern "C" void func_088BA44C(int dst)
{
    if (D_089D7370 == 1) {
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 12; j++) {
                int v = func_088B7DC0(&D_09B4E7C8[i], i, j);
                func_088B75E8(dst, i, j, v);
            }
            for (int j = 0; j < 6; j++) {
                for (int k = 0; k < 3; k++) {
                    int v = func_088B7DF0(&D_09B4E7C8[i], i, j, k);
                    func_088B7618(dst, i, j, k, v);
                }
            }
        }
    }
}
