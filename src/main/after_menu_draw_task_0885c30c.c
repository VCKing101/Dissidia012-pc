extern void *D_089D69A8;               /* the task scheduler */
extern char D_089C8E10[];              /* "AFTER_MENU_DRAW::Update" */
extern void func_0885C348(void);
extern void *func_0888227C(void *scheduler, int id, int priority, void (*fn)(void), const char *name);

void func_0885C30C(void)
{
    func_0888227C(D_089D69A8, -1, 0x11197, func_0885C348, D_089C8E10);
}
