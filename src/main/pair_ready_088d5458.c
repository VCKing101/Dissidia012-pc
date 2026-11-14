/* 1 when both handles are set and the system is idle. */
typedef struct Pair088D5458 {
    int field_00;
    int field_04;
    int a;
    int b;
} Pair088D5458;

extern int D_089D6C30;
int func_0889F35C(int sys);
int func_088D5458(Pair088D5458 *o)
{
    if (o->a != 0 && o->b != 0) {
        if (func_0889F35C(D_089D6C30) == 0) {
            return 1;
        }
    }
    return 0;
}
