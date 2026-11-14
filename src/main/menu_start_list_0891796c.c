typedef struct MenuState {
    char pad_000[0x5C];
    int list;                   /* 0x05C */
    char pad_060[0x770 - 0x60];
    char scroll[4];             /* 0x770 */
} MenuState;

extern void func_088E9958(int list);
extern void func_088E980C(void *scroll, int rows);
extern void func_088E991C(int a, void (*draw)(void));
extern void func_08917CAC(void);
extern MenuState *D_089D83D8;

void func_0891796C(void)
{
    func_088E9958(D_089D83D8->list);
    func_088E980C(D_089D83D8->scroll, 5);
    func_088E991C(0, func_08917CAC);
}
