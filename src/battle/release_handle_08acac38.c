/* Releases the handle stored in the first word of the object. */
extern void *D_089D7830;
extern void func_088CF314(void *mixer, void *handle, unsigned char flags);

void func_08ACAC38(void **self)
{
    int zero = 0;
    func_088CF314(D_089D7830, *self, zero);
}
