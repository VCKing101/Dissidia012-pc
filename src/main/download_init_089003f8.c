/* Starts the download thread; keeps a negative error code. */
typedef struct GameGlobals {
    char pad_0000[0x2CD1];
    unsigned char downloading;
} GameGlobals;

extern int D_089D8134;
extern unsigned char D_089D5FD5;
extern GameGlobals *D_089D5EC0;
extern int func_0890001C(int arg);

void func_089003F8(void)
{
    int result = func_0890001C(0);

    if (result < 0) {
        D_089D8134 = result;
    }
    D_089D5FD5 = 0;
    D_089D5EC0->downloading = 0;
}
