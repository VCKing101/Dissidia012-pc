typedef struct HandleHolder {
    int field_00;
    void *handle;
} HandleHolder;

extern void func_088DA060(void *handle);
extern void func_088DED08(HandleHolder *holder);

void func_088DED28(HandleHolder *holder)
{
    if (holder->handle != 0) {
        func_088DA060(holder->handle);
        holder->handle = 0;
    }
    func_088DED08(holder);
}
