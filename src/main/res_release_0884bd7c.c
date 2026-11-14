/* Releases both buffers of a resource and clears it. */
typedef struct Res0884BD7C {
    void *data;
    int size;
    void *extra;
    int extra_size;
} Res0884BD7C;

void func_08842138(void *p);

void func_0884BD7C(Res0884BD7C *r)
{
    if (r->extra != 0) {
        func_08842138(r->extra);
        r->extra = 0;
    }
    r->extra_size = 0;
    r->size = 0;
    if (r->data != 0) {
        func_08842138(r->data);
    }
    r->data = 0;
}
