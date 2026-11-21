extern void *D_089D69A8;               /* the system object */
extern char D_089C9BF0[];
extern char D_089C9BF8[];              /* "ExecCapture" */
extern int D_089D6030;
extern void *func_08882274(void *system);   /* its task scheduler */
extern void *func_088823C4(void *scheduler, int priority, int order, void (*fn)(void), const char *name); /* task_new */
extern void func_08875364(void);
extern void func_08875540(void);

void func_088754C4(void)
{
    func_088823C4(func_08882274(D_089D69A8), -1, 20000, func_08875364, D_089C9BF0);
    func_088823C4(func_08882274(D_089D69A8), -1, 0x1B1FC, func_08875540, D_089C9BF8);
    D_089D6030 = 0;
}
