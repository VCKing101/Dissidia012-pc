typedef struct Ref08856B44 {
    void *data;
    int refs;
} Ref08856B44;

void func_0899BB1C(void *p);

void func_08856B44(Ref08856B44 *refs, int index) {
    Ref08856B44 *ref = &refs[index];
    int count = ref->refs - 1;
    ref->refs = count;
    if (count <= 0) {
        if (ref->data != 0) {
            func_0899BB1C(ref->data);
        }
        ref->data = 0;
        ref->refs = 0;
    }
}
