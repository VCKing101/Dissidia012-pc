extern "C" {
extern int D_09B49208[];

int func_08890A80(int count) {
    for (int i = 0; i < count; i++) {
        if (D_09B49208[i] > 0) {
            return 0;
        }
    }
    return 1;
}
}
