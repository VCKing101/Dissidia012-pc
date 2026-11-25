/* Formats a message from five numbers in the order the language table asks for, then shows it. */
extern char *D_089D7130[];
extern void *D_089D6998;
extern void func_088AE010(int a, int *v0, int *v1, int *v2, int *v3, int *v4);
extern void func_08867CA0(int *order);
extern void func_0893476C(char *buf, char *fmt, int a, int b, int c, int d, int e);
extern void func_08867DA8(char *buf);
extern void func_0887DF14(void *w, int a, int b, char *buf);
void func_088AE064(int unused, int a, int b, int c)
{
    int v[5];
    int order[3];
    int arr[3];
    char buf[0x40];
    char *fmt;
    func_088AE010(c, &v[0], &v[1], &v[2], &v[3], &v[4]);
    func_08867CA0(order);
    fmt = D_089D7130[order[0]];
    arr[order[0]] = v[0];
    arr[order[1]] = v[1];
    arr[order[2]] = v[2];
    func_0893476C(buf, fmt, arr[0], arr[1], arr[2], v[3], v[4]);
    func_08867DA8(buf);
    func_0887DF14(D_089D6998, a, b, buf);
}
