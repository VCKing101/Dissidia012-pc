/* Adds a reference and creates the shared object on first use. */
typedef struct Ref08851C78 {
    void *obj;
    int count;
} Ref08851C78;

void *func_0899B9F0(int flags);
void func_0899BB68(void *obj, int a, int b, int c);

void func_08851C78(Ref08851C78 *r, int a, int b)
{
    r->count++;
    if (r->obj != 0) {
        return;
    }
    r->obj = func_0899B9F0(0);
    func_0899BB68(r->obj, a, b, 0);
}
