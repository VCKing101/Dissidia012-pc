/* Calls func_08B519A0 when p is non-null and its word at +4 is non-null. */
extern void func_08B519A0(void);
void func_08B51730(int *p)
{
    if (p == 0) { return; }
    if (p[1] == 0) { return; }
    func_08B519A0();
}
