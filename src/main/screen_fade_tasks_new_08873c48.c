extern void *D_089D69A8;               /* the system object */
extern char D_089C9B50[];              /* "SCREEN_FADE::DrawAfterMenu" */
extern char D_089C9B6C[];              /* "SCREEN_FADE::UpdateAfterAll" */
extern char D_089C9B88[];              /* "SCREEN_FADE::DrawAfterAll" */
extern void *func_08882274(void *system);   /* its task scheduler */
extern void *func_088823C4(void *scheduler, int priority, int order, void (*fn)(void), const char *name); /* task_new */
extern void func_08873F54(void);
extern void func_08873E6C(void);
extern void func_08873E90(void);

void func_08873C48(void)
{
    void *scheduler = func_08882274(D_089D69A8);

    func_088823C4(scheduler, -1, 0x11198, func_08873F54, D_089C9B50);
    func_088823C4(scheduler, -1, 0x11190, func_08873E6C, D_089C9B6C);
    func_088823C4(scheduler, -1, 85000, func_08873E90, D_089C9B88);
}
