/* Stores the sound mode and whether output is muted. */
extern int D_089D6C9C;
extern unsigned char D_089D6CA0;
extern unsigned char D_089D6CA1;

void func_088A28E8(int mode, unsigned char level)
{
    D_089D6C9C = mode;
    D_089D6CA0 = level;
    D_089D6CA1 = level == 0;
}
