/* Shows one message while tutorials are active. */
extern int func_088FE640(void);
extern void *func_08867984(int id, int arg);
extern void func_088835C8(void *message, int a, int b);

void func_088FE944(int id, int a, int b)
{
    if (func_088FE640() == 0) {
        return;
    }
    func_088835C8(func_08867984(id, 0), a, b);
}
