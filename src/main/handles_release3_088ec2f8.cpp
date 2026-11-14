/* Releases the three handles at the start of the record. */
struct HandleTriple {
    void *handles[3];
};

extern "C" {
extern void func_088DA060(void *handle);

void func_088EC2F8(HandleTriple *t)
{
    for (int i = 0; i < 3; i++) {
        if (t->handles[i] != 0) {
            func_088DA060(t->handles[i]);
            t->handles[i] = 0;
        }
    }
}
}
