/* Returns whether func_08B39D80 or func_08B39E08 reports true for the object. */
extern int func_08B39D80(void *self);
extern int func_08B39E08(void *self);
int func_08B39D30(void *self)
{
    if (func_08B39D80(self)) return 1;
    if (func_08B39E08(self)) return 1;
    return 0;
}
