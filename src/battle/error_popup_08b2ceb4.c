/* Maps the result code from func_088C9AA4 to a message id (0x19, 0x1C or 0x1D) and shows it through func_088B1230. */
extern int D_089D7280;
extern int D_089D712C;
extern int D_089D727C;
typedef struct MsgRoot { char pad[0xEE0]; int tbl; } MsgRoot;
extern int func_088C9AA4(int v);
extern void func_088B16FC(int v);
extern int func_0887AFBC(int tbl, int id, int z);
extern void func_088B1230(int a, int b, int c, int d, int e, int f);
void func_08B2CEB4(int *self)
{
    int code[1];
    int id;
    int msg;
    code[0] = func_088C9AA4(*self);
    func_088B16FC(D_089D7280);
    id = 0x1C;
    if (code[0] <= (int)0x80020321) {
        if (code[0] > (int)0x80020320) {
            id = 0x19;
        }
    } else if (code[0] == -2) {
        id = 0x1D;
    }
    msg = func_0887AFBC(((MsgRoot *)D_089D712C)->tbl, id, 0);
    func_088B1230(D_089D727C, msg, 0, 0, 0xA0, 0);
}
