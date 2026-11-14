/* Adds a reference and creates the shared object on first use. */
typedef struct Ref08859DA4 {
    void *obj;
    int count;
} Ref08859DA4;

void *func_0899B9F0(int flags);
void func_0899BB68(void *obj, int a, int b, int c);

void func_08859DA4(Ref08859DA4 *r, int a, int b)
{
    r->count++;
    if (r->obj != 0) {
        return;
    }
    r->obj = func_0899B9F0(0);
    func_0899BB68(r->obj, a, b, 0);
}
