extern char D_089C9148[];              /* "disc0:/PSP_GAME/USRDIR/data/%s" */
extern int func_0893476C(char *dst, const char *format, ...);   /* sprintf */
extern void func_08935D58(char *s);

void func_088635B4(void *self, char *dst, const char *name)
{
    func_0893476C(dst, D_089C9148, name);
    func_08935D58(dst);
}
