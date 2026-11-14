/* Sets an effect's source (kind 4 uses the shared default data). */
typedef struct Fx088A6A38 {
    char pad_00[0xD0];
    void *data;
    char pad_D4[8];
    unsigned int flags;
    char pad_E0[0x10];
    int kind;
    int a;
    int b;
    float c;
} Fx088A6A38;

extern char D_09B4D440[];

void func_088A6A38(Fx088A6A38 *fx, int kind, void *data, int a, int b, float c)
{
    fx->kind = kind;
    fx->flags |= 1;
    fx->a = a;
    fx->b = b;
    fx->c = c;
    if (kind == 4) {
        fx->data = D_09B4D440;
    } else {
        fx->data = data;
    }
}
