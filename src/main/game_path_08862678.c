extern const char *D_089D5E90;         /* "ms0:/PSP/GAME/..." */
extern const char *D_089D6010;
extern char D_089C8F54[];              /* "%s/%s" */
extern int func_0893476C(char *dst, const char *format, ...);

int func_08862678(char *dst)
{
    func_0893476C(dst, D_089C8F54, D_089D5E90, D_089D6010);
    return 1;
}
