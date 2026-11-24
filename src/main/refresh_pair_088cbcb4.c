// Looks up an id via func_088B97D0/func_088BCA2C, runs func_088B74A4 on the 0xC6-byte row selected by obj[0x773], then func_088BBE78(id, obj+0x10).
extern int func_088B97D0(int);
extern int func_088BCA2C(int);
extern void func_088B74A4(char *);
extern void func_088BBE78(int, char *);
void func_088CBCB4(char *obj)
{
    char *row = obj + obj[0x773] * 0xC6 + 0x10;
    int b = func_088BCA2C(func_088B97D0(obj[0]));
    func_088B74A4(row);
    func_088BBE78(b, obj + 0x10);
}
