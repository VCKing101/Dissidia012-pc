/* Read the current time stamp, and unless the name is set, derive the default from it; then finish. */
extern void func_089C5D98(char *t);
extern void func_088ADDE8(int a, char *t, char *name);
extern void func_088ADC08(int a, int b, char *name);
void func_088ADE90(int a, int b, char *name, unsigned char flag)
{
    char t[16];
    func_089C5D98(t);
    if (flag != 0) {
        if (*name == 0) {
            func_088ADDE8(a, t, name);
        }
    } else {
        func_088ADDE8(a, t, name);
    }
    func_088ADC08(a, b, name);
}
