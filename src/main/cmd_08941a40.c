/* Builds a command record on the stack (kind 0x20002, word 0x14 = argument) and passes it to func_08944930. */
extern void func_08944930(int, int, void *);
void func_08941A40(int *self, int b, int c) {
    int cmd[14];
    cmd[0x14 / 4] = c;
    cmd[0] = 0x20002;
    func_08944930(self[0x1C / 4], b, cmd);
}
