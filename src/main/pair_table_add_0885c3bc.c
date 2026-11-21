typedef struct Pair0885C3BC {
    int a;
    int b;
} Pair0885C3BC;

extern int D_089D4E18;
extern Pair0885C3BC D_09B17D28[16];

int func_0885C3BC(int a, int b)
{
    if (D_089D4E18 >= 0x10) {
        return 0;
    }
    D_09B17D28[D_089D4E18].a = a;
    D_09B17D28[D_089D4E18].b = b;
    D_089D4E18++;
    return 1;
}
