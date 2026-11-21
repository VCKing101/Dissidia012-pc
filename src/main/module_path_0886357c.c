extern char D_089C9120[];              /* "disc0:/PSP_GAME/USRDIR/data/module/%s" */
extern int func_0893476C(char *dst, const char *format, ...);   /* sprintf */
extern void func_08935D58(char *s);

void func_0886357C(void *self, char *dst, const char *name)
{
    func_0893476C(dst, D_089C9120, name);
    func_08935D58(dst);
}
