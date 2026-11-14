/* Updates the object's two parts when present. */
typedef struct Obj08885988 {
    char pad_00[0xC];
    void *first;
    void *second;
} Obj08885988;

void func_08886640(void *part);
void func_08884990(void *part);

void func_08885988(Obj08885988 *o)
{
    if (o->first != 0) {
        func_08886640(o->first);
    }
    if (o->second != 0) {
        func_08884990(o->second);
    }
}
