/* Registered 0x14-byte entry with the id; 0 when none. */
struct Entry088C4FDC { short id; char pad[0x12]; };
extern "C" {
extern Entry088C4FDC *D_089D76C8;
extern int D_089D76D0;
Entry088C4FDC *func_088C4FDC(int id)
{
    if (D_089D76C8 != 0) {
        for (int i = 0; i < D_089D76D0; i++) {
            if (D_089D76C8[i].id == id) {
                return &D_089D76C8[i];
            }
        }
    }
    return 0;
}
}
