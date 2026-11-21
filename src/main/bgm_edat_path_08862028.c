extern char D_089C8EF0[];
extern char D_089C8F1C[];              /* "bgm/%s" */
extern char D_089C8F10[];              /* "%s/%s.edat" */
extern int func_08862678(char *dst, const char *a, const char *b);   /* game_path */
extern const char *func_08861B84(const char *path);
extern int func_0893476C(char *dst, const char *format, ...);         /* sprintf */

void func_08862028(char *out, const char *name)
{
    char dir[0x100];
    char path[0x80];

    func_08862678(dir, D_089C8EF0, name);
    func_0893476C(path, D_089C8F1C, name);
    func_0893476C(out, D_089C8F10, dir, func_08861B84(path));
}
