/* Starts the object's handle when it has one; the result is ignored. */
typedef struct Obj088CCB18 {
    int field_00;
    void *handle;
} Obj088CCB18;

int func_088CCB68(void *handle);
void func_088CCB18(Obj088CCB18 *o)
{
    if (o == 0) {
        return;
    }
    if (o->handle == 0) {
        return;
    }
    if (func_088CCB68(o->handle) == 0) {
        return;
    }
    return;
}
