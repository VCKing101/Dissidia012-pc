/* Sends the save notice when the save manager is idle. */
typedef struct SaveCtl {
    char pad_000[0x278];
    int busy;
} SaveCtl;

extern SaveCtl *D_08A405E0;
extern int D_089D6040;

extern void func_08875B40(int notice);

void func_0888B1A0(void)
{
    if (!(unsigned char)(D_08A405E0->busy != 0)) {
        func_08875B40(D_089D6040);
    }
}
