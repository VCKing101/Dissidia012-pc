/* Pushes the current value to the system when both handles are set. */
typedef struct Pair088D54DC {
    int field_00;
    int field_04;
    int a;
    int b;
} Pair088D54DC;

extern int D_089D6C30;
int func_088D4C5C(void);
void func_0889F340(int sys, int value);
void func_088D54DC(Pair088D54DC *p)
{
    if (p->a != 0 && p->b != 0) {
        int sys = D_089D6C30;
        func_0889F340(sys, func_088D4C5C());
    }
}
