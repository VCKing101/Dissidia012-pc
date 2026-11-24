/* Resumes the mode's open screen task when it has one. */
typedef struct ModeCtl {
    char pad_000[0x7A4];
    int task;
} ModeCtl;

extern ModeCtl *D_089D6A10;
extern int D_08B23058;

extern void func_08B2286C(int p);
extern void func_089DD8EC(int task, int arg);

void func_0888A064(int arg)
{
    if (D_089D6A10->task == 0) {
        return;
    }
    func_08B2286C(D_08B23058);
    func_089DD8EC(D_089D6A10->task, arg);
}
