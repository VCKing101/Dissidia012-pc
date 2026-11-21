extern char D_089C9118[];              /* "data/%s" */
extern int func_0893476C(char *dst, const char *format, ...);         /* sprintf */
extern void func_08863EA4(void *self, const char *path, int a, int b);

/* The same call with "data/<name>" as the path. */
void func_0886330C(void *self, const char *name, int a, int b)
{
    char path[0x100];

    func_0893476C(path, D_089C9118, name);
    func_08863EA4(self, path, a, b);
}
