extern "C" {
extern void func_088DA060(void *handle);

struct Resource {
    void *handle;
    int field_04;
    int field_08;
    int field_0C;
    virtual void reset();       /* 0x08 */
};

void func_08911940(Resource *res)
{
    if (res->handle != 0) {
        func_088DA060(res->handle);
        res->handle = 0;
    }
    res->reset();
}
}
