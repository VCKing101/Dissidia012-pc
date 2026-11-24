/* Releases up to eleven handles held in the object, runs the sub-object cleanup, then the mixer reset. */
extern void *D_089D7830;
extern void func_08AF3C94(void *p);
extern void func_088DAB44(void);
extern void func_088D0370(void *mixer, int id, unsigned char flag);

void func_08AF7050(int *self)
{
    int i;
    func_08AF3C94(self + 0x58);
    i = 0;
    do {
        if (*self != 0) {
            func_088DAB44();
            *self = 0;
        }
        i = i + 1;
        self = self + 1;
    } while (i < 0xB);
    {
    int id = 0x17;
    int zero = 0;
    func_088D0370(D_089D7830, id, zero);
    }
}
