/* Builds a record in a local buffer, looks up an id with it and stores the (non-negative) lookup result byte in the owner. */
extern void func_088CB9E8(void *buf);
extern int func_08AA4384(int self, void *buf, int id);
extern int func_08AB16A8(int p);
extern int func_08AB1B4C(int id, int p);
void func_08AA4304(int self) {
    char buf[0x778];
    int id;
    int v;
    func_088CB9E8(buf);
    id = func_08AA4384(self, buf, *(int *)(*(int *)(self + 0x1EE4) + 0x590));
    v = 0;
    if (func_08AB16A8(*(int *)(self + 0x1EE4) + 0x710) >= 0) {
        v = func_08AB1B4C(id, *(int *)(self + 0x1EE4) + 0x1C98);
    }
    *(unsigned char *)(*(int *)(self + 0x1EE4) + 0x1E58) = v;
}
