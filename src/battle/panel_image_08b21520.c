extern char D_08B22F08[];
extern char D_08B22F30[];
extern char D_08B22F60[];
extern char D_08B22F90[];
extern char D_08B22FB8[];

char *func_08B21520(unsigned int index)
{
    char *names[5];

    if (index == 0 || index > 5) {
        return 0;
    }
    names[0] = D_08B22F08;
    names[1] = D_08B22F30;
    names[2] = D_08B22F60;
    names[3] = D_08B22F90;
    names[4] = D_08B22FB8;
    return names[(int)index - 1];
}
