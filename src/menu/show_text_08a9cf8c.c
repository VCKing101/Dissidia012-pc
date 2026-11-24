/* Calls the text routine on the global manager with the object's ids and a literal string, then the follow-up. */
extern int D_089D7F90;
extern const char D_08AA3CD4[];
extern void func_088FD928(int g, int a, int b, int c, const char *s, int d, int e, int f, int g2, int h, int i, int j);
extern void func_088FCBBC(int g);
void func_08A9CF8C(int self) {
    func_088FD928(D_089D7F90, *(int *)(self + 0x138), 5, 0x1D, D_08AA3CD4, 0x70F3,
                  *(int *)(*(int *)(self + 0x154) + 0x884), 4, 0, 0, 0, *(int *)(self + 0x140));
    func_088FCBBC(D_089D7F90);
}
