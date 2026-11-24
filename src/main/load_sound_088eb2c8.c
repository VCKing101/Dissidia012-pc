/* Loads "sound/snd_region_enc.scd" once: when D_089D7EC8 is -1 and func_088FE7C8(name, 0) gives a size, allocates a buffer with func_088A0128, fills it with func_088FE708 and registers it with func_088A15B8(D_089D6C88, buf, size) into D_089D7EC8. */
extern int D_089D7EC8;
extern int D_089D6C88;
extern char D_089CCA0C[];
extern int func_088FE7C8(char *name, int a);
extern int func_088A0128(void);
extern void func_088FE708(char *name, int buf, int size, int a);
extern int func_088A15B8(int h, int buf, int size);
void func_088EB2C8(void)
{
    int size;
    int buf;
    if (D_089D7EC8 == -1) {
        size = func_088FE7C8(D_089CCA0C, 0);
        if (size != 0) {
            buf = func_088A0128();
            if (buf != 0) {
                func_088FE708(D_089CCA0C, buf, size, 0);
                D_089D7EC8 = func_088A15B8(D_089D6C88, buf, size);
            }
        }
    }
}
