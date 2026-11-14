/* Shows every message of a zero-terminated id list while tutorials are active. */
extern "C" {
extern int func_088FE640(void);
extern void *func_08867984(int id, int arg);
extern void func_08883400(void *message, int a, int b);

void func_088FE6A0(int *ids)
{
    if (func_088FE640() == 0) {
        return;
    }
    for (int *p = ids; *p != 0; p++) {
        func_08883400(func_08867984(*p, 0), 0, 0);
    }
}
}
