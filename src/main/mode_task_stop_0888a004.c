/* Stops the mode's open screen task when it has one. */
typedef struct ModeCtl {
    char pad_000[0x7A4];
    int task;
} ModeCtl;

extern ModeCtl *D_089D6A10;
extern int D_08B23058;

extern void func_08B22BAC(int p);
extern void func_089DD954(int task, int arg);

void func_0888A004(int arg)
{
    if (D_089D6A10->task == 0) {
        return;
    }
    func_08B22BAC(D_08B23058);
    func_089DD954(D_089D6A10->task, arg);
}
