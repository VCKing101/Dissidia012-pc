/* True when the menu state word at 0x09B6D86C is 2. */
struct St { int a, mode; };
extern struct St D_09B6D868;
int func_08B0DF10(void)
{
    int two = 2;
    struct St *s = &D_09B6D868;
    int m = s->mode;
    if (m == two) { return 1; } else { return 0; }
}
