/* Calls func_08B4FF80(&self[0x38 + i * 0x24], 0.0f) for five entries. */
extern void func_08B4FF80(char *p, float z);
void func_08B50AFC(char *self)
{
    int i = 0;
    char *p = self + 0x38;
    float zero = 0.0f;
    do {
        func_08B4FF80(p, zero);
        i++;
        p += 0x24;
    } while (i < 5);
}
