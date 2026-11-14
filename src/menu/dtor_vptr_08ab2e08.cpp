extern "C" {
void func_08992F04(void *object);
extern char D_08AB2F48;
}

struct DtorObj {
    char pad_0000[0x588];
    char *field_588;

    static void destroy(DtorObj *object, unsigned int flags, char *vtable)
    {
        if (object != 0) {
            object->field_588 = vtable;
            if ((flags & 1) != 0) {
                func_08992F04(object);
            }
        }
    }
};

extern "C" {

void func_08AB2E08(DtorObj *object, unsigned int flags)
{
    DtorObj::destroy(object, flags, &D_08AB2F48);
}

}
