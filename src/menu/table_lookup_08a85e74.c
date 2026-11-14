typedef struct A6Pair2 {
    short a;
    short b;
} A6Pair2;

extern A6Pair2 D_08A86218[];

int func_08A85E74(int i, int j) {
    if (j == 0) {
        return D_08A86218[i].a;
    } else {
        return D_08A86218[i].b;
    }
}
