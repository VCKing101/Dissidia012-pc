/* Starts the object's handle when it has one; the result is ignored. */
typedef struct Obj088D7028 {
    int field_00;
    void *handle;
} Obj088D7028;

int func_088D7078(void *handle);
void func_088D7028(Obj088D7028 *o)
{
    if (o == 0) {
        return;
    }
    if (o->handle == 0) {
        return;
    }
    if (func_088D7078(o->handle) == 0) {
        return;
    }
    return;
}
