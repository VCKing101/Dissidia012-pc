extern void *D_089D69A8;               /* the system object */
extern char D_089C9F58[];              /* "VSYNC::VsyncTask" */
extern void *func_08882274(void *system);   /* its task scheduler */
extern void *func_088823C4(void *scheduler, int priority, int order, void (*fn)(void), const char *name); /* task_new */
extern void func_08883EE8(void);

void func_08883F10(void)
{
    func_088823C4(func_08882274(D_089D69A8), -1, 110000, func_08883EE8, D_089C9F58);
}
