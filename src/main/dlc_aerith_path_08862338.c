extern char D_089C8F94[];
extern char D_089C8F9C[];              /* "aerith.bin" */
extern char D_089C8FA8[];              /* "dlc/aerith.bin" */
extern char D_089C8F10[];              /* "%s/%s.edat" */
extern int func_08862678(char *dst, const char *a, const char *b);   /* game_path */
extern const char *func_08861B84(const char *path);
extern int func_0893476C(char *dst, const char *format, ...);         /* sprintf */

void func_08862338(char *out)
{
    char dir[0x100];

    func_08862678(dir, D_089C8F94, D_089C8F9C);
    func_0893476C(out, D_089C8F10, dir, func_08861B84(D_089C8FA8));
}
