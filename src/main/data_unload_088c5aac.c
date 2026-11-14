/* Closes the loaded data when it owns its handle and forgets every loaded pointer. */
extern int D_089D76F8;
extern int D_089D76FC;
extern int D_089D7700;
extern int D_089D7704;
extern int D_089D7708;
extern int D_089D770C;
extern int D_089D7710;
extern unsigned char D_089D7714;
void func_08882158(int handle);
void func_088C5AAC(void)
{
    if (D_089D76F8 != 0) {
        if (D_089D7714 == 1) {
            func_08882158(D_089D76F8);
            D_089D7714 = 0;
        }
        D_089D76F8 = 0;
        D_089D76FC = 0;
        D_089D7700 = 0;
        D_089D7704 = 0;
        D_089D7708 = 0;
        D_089D770C = 0;
        D_089D7710 = 0;
    }
}
