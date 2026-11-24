/* Builds a command record on the stack (kind 8, word 0x1C = argument) and passes it to func_08944930. */
extern void func_08944930(int, int, void *);
void func_08941A9C(int *self, int b, int c) {
    int cmd[14];
    cmd[0x1C / 4] = c;
    cmd[0] = 8;
    func_08944930(self[0x1C / 4], b, cmd);
}
