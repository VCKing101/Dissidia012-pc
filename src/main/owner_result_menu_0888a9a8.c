/* Opens the result step for the current owner: the retry path when the
   owner can continue, else the matching end screen page. */
typedef struct Owner Owner;

extern Owner *D_089D8140;
extern int D_089D6A14;
extern int D_089D85E8;
extern int D_089E9D38;
extern unsigned char D_089D5F20;

extern int func_08900A40(Owner *owner);
extern void func_0888B8FC(int task, int flag);
extern void func_0888B970(int task);
extern void func_08933C00(int sound);
extern int func_08900E9C(Owner *owner);
extern void func_089E7DF0(int screen, int page, int flag);

void func_0888A9A8(void)
{
    Owner *owner = D_089D8140;

    if (func_08900A40(owner) == 1) {
        func_0888B8FC(D_089D6A14, 0);
        func_0888B970(D_089D6A14);
    } else {
        func_08933C00(D_089D85E8);
        if (func_08900E9C(owner) == 1) {
            func_089E7DF0(D_089E9D38, 4, 0);
        } else if (D_089D5F20 == 1) {
            func_089E7DF0(D_089E9D38, 2, 0);
        } else {
            func_089E7DF0(D_089E9D38, 1, 0);
        }
    }
}
