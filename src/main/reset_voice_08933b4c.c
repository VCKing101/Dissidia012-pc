/* Runs the stop handler for the object's current mode (if any) and clears it, then frees the buffer handle at +4. */
typedef struct Voice { signed char mode; char pad[3]; int handle; } Voice;
extern void (*D_089D85DC[])(void);
extern void func_0893335C(void);
void func_08933B4C(Voice *p)
{
    int neg = -1;
    if (p->mode != neg) {
        D_089D85DC[p->mode]();
        p->mode = neg;
    }
    if (p->handle != 0) {
        func_0893335C();
        p->handle = 0;
    }
}
