/* When the global byte is zero: optionally clears a flag byte, runs the setter and clears bit 0x200; returns 1, otherwise 0. */
extern unsigned char *D_089D7280;
extern int D_089D712C;
extern void func_088D9CD8(int a);
int func_08AB8B38(int self) {
    if (*D_089D7280 == 0) {
        if (*(unsigned char *)(*(int *)(self + 0x138) + 0xD57) == 0) {
            if (*(unsigned char *)(*(int *)(self + 0x138) + 0xD5A) != 0) {
                *(char *)(D_089D712C + 0xEFC) = 0;
            }
        }
        func_088D9CD8(*(int *)(*(int *)(self + 0x138) + 0x408));
        *(unsigned *)(*(int *)(self + 0x138) + 0xD30) = *(unsigned *)(*(int *)(self + 0x138) + 0xD30) & 0xFFFFFDFF;
        return 1;
    } else {
        return 0;
    }
}
