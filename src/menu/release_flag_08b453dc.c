/* If the flag byte at +8 is set, calls func_0899C6F8 on the handle at +0 with 0xFFFF and clears the flag. */
typedef struct S { int handle; int pad; unsigned char flag; } S;
extern void func_0899C6F8(int handle, int v);
void func_08B453DC(S *self)
{
    if (self->flag) {
        func_0899C6F8(self->handle, 0xFFFF);
        self->flag = 0;
    }
}
