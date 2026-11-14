/* True when the battle table holds the entry for this key. */
extern void *D_089D4788;
extern int func_08A641E8(int key);
extern int func_08811168(void *table, int entry);

int func_08A6420C(int key)
{
    void *table = D_089D4788;
    return func_08811168(table, func_08A641E8(key)) != 0;
}
