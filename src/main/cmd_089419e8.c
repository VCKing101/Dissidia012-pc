/* Builds a command record on the stack (kind 1, word 0x10 = argument) and passes it to func_08944930. */
extern void func_08944930(int, int, void *);
void func_089419E8(int *self, int b, int c) {
    int cmd[14];
    cmd[0x10 / 4] = c;
    cmd[0] = 1;
    func_08944930(self[0x1C / 4], b, cmd);
}
