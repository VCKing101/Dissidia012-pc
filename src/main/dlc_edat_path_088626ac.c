extern char D_089C8FC0[];
extern char D_089C8F10[];              /* "%s/%s.edat" */
extern int func_08862678(char *dst, const char *a, const char *b);   /* game_path */
extern const char *func_08861B84(const char *path);
extern int func_0893476C(char *dst, const char *format, ...);         /* sprintf */

int func_088626AC(char *out, const char *name)
{
    char dir[0x100];
    int r = func_08862678(dir, D_089C8FC0, name);

    func_0893476C(out, D_089C8F10, dir, func_08861B84(name));
    return r;
}
