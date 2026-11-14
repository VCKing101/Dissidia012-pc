extern char D_08A93B74[];
int func_0893476C(char *buf, char *fmt, int a, int b);
void func_08A8D868(int a, int b, char *buf);

void func_08A8D9F4(int p1, int p2, int p3, int p4) {
    char buf[16];
    int i = 0;
    do {
        buf[i] = 0;
        i++;
    } while (i < 16);
    func_0893476C(buf, D_08A93B74, p3, p4);
    func_08A8D868(p1, p2, buf);
}
