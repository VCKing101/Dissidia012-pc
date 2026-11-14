/* Registered 0x20-byte entry with the id; 0 when none. */
struct Entry088C4F74 { short id; char pad[0x1E]; };
extern "C" {
extern Entry088C4F74 *D_089D76C4;
extern int D_089D76CC;
Entry088C4F74 *func_088C4F74(int id)
{
    if (D_089D76C4 != 0) {
        for (int i = 0; i < D_089D76CC; i++) {
            if (D_089D76C4[i].id == id) {
                return &D_089D76C4[i];
            }
        }
    }
    return 0;
}
}
