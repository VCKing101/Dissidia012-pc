/* Derives a save's default name from a time stamp: formats year, month, day (ordered by the locale's date order) plus hour, minute, second into the name. */
extern char *D_089D6DDC[];
void func_08867CA0(int *order);
void func_0893476C(char *dst, char *fmt, ...);
void func_08867DA8(char *name);

void func_088ADDE8(int a, unsigned short *t, char *name)
{
    int order[3];
    unsigned int v[3];
    char *fmt;
    func_08867CA0(order);
    fmt = D_089D6DDC[order[0]];
    v[order[0]] = t[0];
    v[order[1]] = t[1];
    v[order[2]] = t[2];
    func_0893476C(name, fmt, v[0], v[1], v[2], t[3], t[4], t[5]);
    func_08867DA8(name);
}
