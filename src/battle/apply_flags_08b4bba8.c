/* Calls one of three setters for the horizontal flag bits (1, 2, 4) and one of three for the vertical bits (8, 0x10, 0x20), first match wins. */
extern void func_0887B970(void *o);
extern void func_0887B98C(void *o);
extern void func_0887B958(void *o);
extern void func_0887B9A4(void *o);
extern void func_0887B9C4(void *o);
extern void func_0887B9E4(void *o);
void func_08B4BBA8(int a0, void *obj, unsigned flags) {
    if (flags & 1) {
        func_0887B970(obj);
    } else if (flags & 2) {
        func_0887B98C(obj);
    } else if (flags & 4) {
        func_0887B958(obj);
    }
    if (flags & 8) {
        func_0887B9A4(obj);
    } else if (flags & 0x10) {
        func_0887B9C4(obj);
    } else if (flags & 0x20) {
        func_0887B9E4(obj);
    }
}
