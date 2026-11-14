/* Releases the handle at 4, then resets the rest. */
typedef struct HandleHolder {
    int field_00;
    void *handle;
} HandleHolder;

extern void func_088DA060(void *handle);
extern void func_088D7BCC(HandleHolder *holder);

void func_088D7BF0(HandleHolder *holder)
{
    if (holder->handle != 0) {
        func_088DA060(holder->handle);
        holder->handle = 0;
    }
    func_088D7BCC(holder);
}
