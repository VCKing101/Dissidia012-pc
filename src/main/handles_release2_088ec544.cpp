/* Releases the two handles at the start of the record. */
struct HandlePair {
    void *handles[2];
};

extern "C" {
extern void func_088DA060(void *handle);

void func_088EC544(HandlePair *p)
{
    for (int i = 0; i < 2; i++) {
        if (p->handles[i] != 0) {
            func_088DA060(p->handles[i]);
            p->handles[i] = 0;
        }
    }
}
}
