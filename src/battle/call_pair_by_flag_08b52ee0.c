/* Gets func_0881BAA8's result and passes it with the argument to func_0881BD58 (flag set) or func_0881BBF4. */
extern void *func_0881BAA8(void);
extern void func_0881BD58(void *, void *);
extern void func_0881BBF4(void *, void *);
void func_08B52EE0(void *arg, unsigned char flag)
{
    if (flag) {
        void *p = func_0881BAA8();
        func_0881BD58(p, arg);
    } else {
        void *p = func_0881BAA8();
        func_0881BBF4(p, arg);
    }
}
