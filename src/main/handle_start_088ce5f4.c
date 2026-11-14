/* Starts the object's handle when it has one; the result is ignored. */
typedef struct Obj088CE5F4 {
    int field_00;
    void *handle;
} Obj088CE5F4;

int func_088CE644(void *handle);
void func_088CE5F4(Obj088CE5F4 *o)
{
    if (o == 0) {
        return;
    }
    if (o->handle == 0) {
        return;
    }
    if (func_088CE644(o->handle) == 0) {
        return;
    }
    return;
}
