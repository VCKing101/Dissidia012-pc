extern char D_089C9168[];              /* "disc0:/PSP_GAME/USRDIR/%s" */
extern int func_0893476C(char *dst, const char *format, ...);   /* sprintf */
extern void func_08935D58(char *s);

void func_088635EC(void *self, char *dst, const char *name)
{
    func_0893476C(dst, D_089C9168, name);
    func_08935D58(dst);
}
