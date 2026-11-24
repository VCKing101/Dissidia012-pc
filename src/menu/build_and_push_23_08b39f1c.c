/* Builds a 52-byte record on the stack, func_08922720(rec, {0x23, a}), func_0891FBEC(D_089D8528, rec), func_08922654(rec, 2). */
extern void *D_089D8528;
extern void func_0892262C(char *rec);
extern void func_08922720(char *rec, int *cmd);
extern void func_0891FBEC(void *o, char *rec);
extern void func_08922654(char *rec, int n);
void func_08B39F1C(int unused, int a)
{
    int cmd[12];
    char rec[0x34];
    cmd[0] = 0x23;
    cmd[1] = a;
    func_0892262C(rec);
    func_08922720(rec, cmd);
    func_0891FBEC(D_089D8528, rec);
    func_08922654(rec, 2);
}
