/* Starts the object's handle when it has one; the result is ignored. */
typedef struct Obj088CD704 {
    int field_00;
    void *handle;
} Obj088CD704;

int func_088CD754(void *handle);
void func_088CD704(Obj088CD704 *o)
{
    if (o == 0) {
        return;
    }
    if (o->handle == 0) {
        return;
    }
    if (func_088CD754(o->handle) == 0) {
        return;
    }
    return;
}
