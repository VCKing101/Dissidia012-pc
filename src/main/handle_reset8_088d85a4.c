/* Releases the handle at 8, then resets the rest. */
typedef struct HandleHolder8 {
    int field_00;
    int field_04;
    void *handle;
} HandleHolder8;

extern void func_088DA060(void *handle);
extern void func_088D8594(HandleHolder8 *holder);

void func_088D85A4(HandleHolder8 *holder)
{
    if (holder->handle != 0) {
        func_088DA060(holder->handle);
        holder->handle = 0;
    }
    func_088D8594(holder);
}
