extern unsigned int D_09B6B3D8;

void func_0890EA18(int id)
{
    int index = id - 100;
    if (index >= 0 && index < 32) {
        D_09B6B3D8 &= ~(1 << index);
    }
}
