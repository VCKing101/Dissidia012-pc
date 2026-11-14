/* Formats two values into the shared text buffer and returns it. */
extern "C" {

extern char D_08B633F0[];
extern char D_089C88D8[];

int func_0893476C(char *dst, const char *format, int a, int b);

char *func_08844FA4(int a, int b)
{
    func_0893476C(D_08B633F0, D_089C88D8, a, b);
    return D_08B633F0;
}

}
