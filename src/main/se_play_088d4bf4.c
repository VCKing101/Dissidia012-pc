/* Plays sound effect 0..40; 0 for any other index. */
extern int D_089CBE60[];
int func_088A24D4(int a, int sound, int b, int c);
int func_088D4BF4(unsigned int index)
{
    if (index >= 41) {
        return 0;
    }
    return func_088A24D4(0, D_089CBE60[index], 1, 0);
}
