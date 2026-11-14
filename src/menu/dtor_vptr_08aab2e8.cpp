extern "C" {
void func_08992F04(void *object);
extern char D_08AAFE80;
}

struct DtorObj {
    char pad_0000[32000];
    char *field_7D00;

    static void destroy(DtorObj *object, unsigned int flags, char *vtable)
    {
        if (object != 0) {
            object->field_7D00 = vtable;
            if ((flags & 1) != 0) {
                func_08992F04(object);
            }
        }
    }
};

extern "C" {

void func_08AAB2E8(DtorObj *object, unsigned int flags)
{
    DtorObj::destroy(object, flags, &D_08AAFE80);
}

}
