extern "C" int func_089E9858(int value, int limit, unsigned int digit)
{
    if (digit >= 10) {
        return -1;
    }
    int scale = 1;
    for (int i = 0; i < (int)digit; i++) {
        scale *= 10;
    }
    if (limit < scale && digit != 0) {
        return -1;
    }
    if (value < scale && digit != 0) {
        return 10;
    }
    return (value / scale) % 10;
}
