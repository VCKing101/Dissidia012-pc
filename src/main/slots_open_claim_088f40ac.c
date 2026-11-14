/* Opens the slot table and claims a slot for `arg`; returns the open result. */
extern int func_088F2E68(void *slots);
extern int func_088F3704(void *slots, int arg);

int func_088F40AC(void *slots, int arg)
{
    int result = func_088F2E68(slots);

    func_088F3704(slots, arg);
    return result;
}
