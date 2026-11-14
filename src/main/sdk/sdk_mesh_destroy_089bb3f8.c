typedef struct SdkMesh {
    unsigned short ref;
    unsigned short pad2;
    void *buf_4;
    void *buf_8;
    void *arr_c;
    void *arr_10;
    void *arr_14;
    unsigned char n_18;
} SdkMesh;

extern void func_089BB22C(void *arr, int n);
extern void func_089BB2BC(void *arr, int n);
extern void func_089BB368(void *arr, int n);
extern void func_089BB0C8(int pool, void *p);

SdkMesh *func_089BB3F8(SdkMesh *m)
{
    if (m != 0) {
        void *arr;
        func_089BB22C(m->arr_c, 1);
        arr = m->arr_10;
        func_089BB2BC(arr, 1);
        func_089BB368(m->arr_14, m->n_18 + 1);
        func_089BB0C8(1, m->buf_4);
        func_089BB0C8(1, m->buf_8);
    }
    return m;
}
