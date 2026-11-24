/* Starts the field retry main task. */
typedef struct RetryCtl {
    char pad_000[0x100];
    char task[4];
} RetryCtl;

extern RetryCtl *D_089D6A14;
extern char D_089CA0C8[];

extern void func_08865088(void *task, int id, int priority, void (*fn)(void), const char *name);
extern void func_0888B89C(void);

void func_0888B858(void)
{
    func_08865088(D_089D6A14->task, -1, 1000, func_0888B89C, D_089CA0C8);
}
