/* Frees the buffer, closes the handle and finishes the object. */
typedef struct Obj088B38E0 {
    int handle;
    char pad_04[0x14];
    void *buffer;
} Obj088B38E0;

void func_08992F04(void *p);
void func_08882158(int handle);
void func_088B38C0(Obj088B38E0 *obj);
void func_088B38E0(Obj088B38E0 *obj)
{
    if (obj->buffer != 0) {
        func_08992F04(obj->buffer);
    }
    if (obj->handle != 0) {
        func_08882158(obj->handle);
    }
    func_088B38C0(obj);
}
