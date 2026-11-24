/* Allocates a 0x270800-byte block and registers it under the name "CROOM_T_BUF" with the allocator. */
extern int func_0888204C(int size);
extern int func_0885C40C(int block, int size, const char *name, int n);
extern const char D_08A87DBC[];
void func_08A86AA0(int self) {
    int size = 0x270800;
    int blk = func_0888204C(size);
    *(int *)(self + 0x134) = blk;
    *(int *)(self + 0x138) = func_0885C40C(blk, size, D_08A87DBC, 0x40);
}
