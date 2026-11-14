/* Plays a voice, then applies its group settings. */
void func_088D2980(int a, int b, int c, unsigned char d, int e, int f);
void func_088D0E64(int a, unsigned char d, int e);
void func_088D1028(int a, int b, int c, unsigned char d, int e)
{
    func_088D2980(a, b, c, d, e, 0);
    func_088D0E64(a, d, e);
}
