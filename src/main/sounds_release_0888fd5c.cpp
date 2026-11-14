extern "C" {
extern int D_09B491E8[3];
void func_088DAB44(int sound);

void func_0888FD5C(void) {
    for (int i = 0; i < 3; i++) {
        if (D_09B491E8[i] != 0) {
            func_088DAB44(D_09B491E8[i]);
            D_09B491E8[i] = 0;
        }
    }
}
}
