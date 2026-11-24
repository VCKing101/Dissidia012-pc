/* Builds a command record on the stack (kind 16, word 0x20 = argument) and passes it to func_08944930. */
extern void func_08944930(int, int, void *);
void func_08941A70(int *self, int b, int c) {
    int cmd[14];
    cmd[0x20 / 4] = c;
    cmd[0] = 16;
    func_08944930(self[0x1C / 4], b, cmd);
}
