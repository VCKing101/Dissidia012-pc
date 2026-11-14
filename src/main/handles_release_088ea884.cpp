extern "C" {
extern int D_089D6C80;
void func_088A0DFC(int bank, int handle);

void func_088EA884(int *handles, unsigned char keep)
{
    if (keep) {
        return;
    }
    for (int i = 0; i < 8; i++) {
        if (handles[i] >= 0) {
            func_088A0DFC(D_089D6C80, handles[i]);
            handles[i] = -1;
        }
    }
}
}
