#include "main/sdk_gcc2_types.h"

extern void func_089B4868(void);
extern void func_089AC148(SdkModel *m);
extern void func_089ACAA8(void *arr, int n);
extern void func_089AC948(void *arr, int n);
extern void func_089AC75C(void *arr, int n);
extern void func_089AC598(void *arr, int n);
extern void func_089ACC90(void *arr, int n);
extern void func_089AC000(SdkModel *m);

SdkModel *func_089ACD20(SdkModel *m)
{
    if (m != 0) {
        void *arr;
        int n;
        func_089B4868();
        func_089AC148(m);
        arr = m->arr_c;
        n = m->n_20;
        func_089ACAA8(arr, n);
        arr = m->arr_10;
        n = m->n_22;
        func_089AC948(arr, n);
        arr = m->arr_14;
        n = m->n_24;
        func_089AC75C(arr, n);
        arr = m->arr_18;
        n = m->n_26;
        func_089AC598(arr, n);
        func_089ACC90(m->arr_1c, m->n_28 + 1);
        func_089ABF70(0, m->buf_44);
        func_089ABF70(0, m->buf_48);
        func_089ABF70(0, m->buf_54);
    }
    return m;
}

void func_089ACDBC(SdkModel *m)
{
    if (m != 0) {
        unsigned short ref = m->ref;
        func_089ACD20(m);
        func_089AC000(m);
        m->ref = ref;
    }
}
