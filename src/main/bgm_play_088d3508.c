/* Plays background track 0..2; -1 for any other index. */
extern int D_089CBE50[];

void func_088A24D4(int a, int track, int b, int c);

int func_088D3508(unsigned int index)
{
    if (index >= 3) {
        return -1;
    }
    func_088A24D4(0, D_089CBE50[index], 1, 0);
    return 0;
}
