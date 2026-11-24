/* Calls func_08B545E0 when the object from func_08B55934 has a nonzero field 4. */
struct Q { int a; int b; };
extern struct Q *func_08B55934(void);
extern void func_08B545E0(void *);
void func_08B55568(void *arg)
{
    struct Q *q = func_08B55934();
    if (q->b == 0) {
        return;
    }
    func_08B545E0(arg);
}
