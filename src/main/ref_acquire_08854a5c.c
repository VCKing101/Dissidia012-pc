/* Adds a reference and creates the shared object on first use. */
typedef struct Ref08854A5C {
    void *obj;
    int count;
} Ref08854A5C;

void *func_0899B9F0(int flags);
void func_0899BB68(void *obj, int a, int b, int c);

void func_08854A5C(Ref08854A5C *r, int unused, int a, int b)
{
    r->count++;
    if (r->obj != 0) {
        return;
    }
    r->obj = func_0899B9F0(0);
    func_0899BB68(r->obj, a, b, 0);
}
