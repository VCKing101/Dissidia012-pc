/* Maps the mode 0..4 to a pair of ids and applies both to the object. */
extern void func_08A8D480(void *self, int slot, int id);

void func_08A8C46C(void *self, int mode)
{
    int cell[2];
    if (mode == 0) {
        cell[0] = 0x2A;
        cell[1] = 0x2D;
    } else if (mode == 1) {
        cell[0] = 0x29;
        cell[1] = 0x2D;
    } else if (mode == 2) {
        cell[0] = 0x29;
        cell[1] = 0x2E;
    } else if (mode == 3) {
        cell[0] = 0x2A;
        cell[1] = 0x2F;
    } else if (mode == 4) {
        cell[0] = 0x29;
        cell[1] = 0x2F;
    }
    func_08A8D480(self, 0x11, cell[0]);
    func_08A8D480(self, 0x10, cell[1]);
}
