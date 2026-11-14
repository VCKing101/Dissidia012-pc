extern void func_08911A2C(void *p);
extern void func_08911E20(void *p);
extern void func_0892E7EC(void *p);
typedef struct A { char d[0x14]; } A;
typedef struct B { char d[0x28]; } B;
typedef struct C { char d[0x14]; } C;
typedef struct Set { char pad[8]; A a[8]; B b[4]; C c[1]; } Set;
void func_08911440(Set *set)
{
    int i;
    for (i = 0; i < 8; i++) {
        func_08911A2C(&set->a[i]);
    }
    for (i = 0; i < 4; i++) {
        func_08911E20(&set->b[i]);
    }
    for (i = 0; i < 1; i++) {
        func_0892E7EC(&set->c[i]);
    }
}
