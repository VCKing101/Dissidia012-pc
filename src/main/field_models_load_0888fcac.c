/* Loads the three field marker models and sets their draw priorities. */
extern int D_089D7830;
extern int D_09B491E8[3];

extern int func_088D0700(int loader, int flag, int file, int slot);
extern void func_088DB4E4(int model, int priority);

void func_0888FCAC(void)
{
    int one = 1;

    D_09B491E8[0] = func_088D0700(D_089D7830, one, 0x24A, 0x1A);
    D_09B491E8[1] = func_088D0700(D_089D7830, one, 0x24C, 0x1B);
    D_09B491E8[2] = func_088D0700(D_089D7830, one, 0x24E, 0x1C);
    func_088DB4E4(D_09B491E8[0], 200);
    func_088DB4E4(D_09B491E8[1], 199);
    func_088DB4E4(D_09B491E8[2], 199);
}
