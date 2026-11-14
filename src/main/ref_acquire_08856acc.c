/* Adds a reference to slot index and creates its object on first use. */
typedef struct Ref08856ACC {
    void *obj;
    int count;
} Ref08856ACC;

void *func_0899B9F0(int flags);
void func_0899BB68(void *obj, int a, int b, int c);

void func_08856ACC(Ref08856ACC *refs, int index, int a, int b)
{
    Ref08856ACC *r = &refs[index];
    r->count++;
    if (r->obj != 0) {
        return;
    }
    r->obj = func_0899B9F0(0);
    func_0899BB68(r->obj, a, b, 0);
}
