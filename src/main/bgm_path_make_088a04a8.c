/* Builds the full path of a BGM file, from the install folder or the disc,
   and normalises it. */
extern char D_089CAB94[];

extern void func_08862028(char *out, const char *name);
extern void func_0893476C(char *buf, const char *fmt, const char *name);
extern void func_08935D58(char *path);

void func_088A04A8(char *out, const char *name, unsigned char installed)
{
    if (installed == 1) {
        func_08862028(out, name);
    } else {
        func_0893476C(out, D_089CAB94, name);
    }
    func_08935D58(out);
}
