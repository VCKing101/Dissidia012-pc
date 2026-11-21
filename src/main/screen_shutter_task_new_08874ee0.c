extern void *D_089D69A8;               /* the system object */
extern char D_089C9BC8[];              /* "SCREEN_SHUTTER::Main" */
extern void *func_08882274(void *system);   /* its task scheduler */
extern void *func_088823C4(void *scheduler, int priority, int order, void (*fn)(void), const char *name); /* task_new */
extern void func_088751B8(void);

void func_08874EE0(void)
{
    func_088823C4(func_08882274(D_089D69A8), -1, 0x11192, func_088751B8, D_089C9BC8);
}
