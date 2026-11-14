typedef struct HandleHolder {
    int field_00;
    void *handle;
} HandleHolder;

extern void func_088DA060(void *handle);
extern void func_088D973C(HandleHolder *holder);

void func_088D9750(HandleHolder *holder)
{
    if (holder->handle != 0) {
        func_088DA060(holder->handle);
        holder->handle = 0;
    }
    func_088D973C(holder);
}
