/* Collects the ids for which both checks pass into the owner's list at +0xD60 (count at +0xE60). */
extern int func_088F47C4(void);
extern int func_088F4554(int i);
extern int func_08AB9A64(int self, int i);
void func_08AB9AEC(int self) {
    int i;
    *(int *)(*(int *)(self + 0x138) + 0xE60) = 0;
    for (i = 0; i < func_088F47C4(); i = i + 1) {
        int r = func_088F4554(i);
        if (r != 0) {
            r = func_08AB9A64(self, i);
            if (r != 0) {
                *(int *)(*(int *)(self + 0x138) + *(int *)(*(int *)(self + 0x138) + 0xE60) * 4 + 0xD60) = i;
                *(int *)(*(int *)(self + 0x138) + 0xE60) = *(int *)(*(int *)(self + 0x138) + 0xE60) + 1;
            }
        }
    }
}
