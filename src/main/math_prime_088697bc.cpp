extern "C" int func_088697BC(int value) {
    if (value < 2) return 0;
    if (value == 2) return 1;
    if (value & 1) {
        for (int divisor = 3; divisor * divisor <= value; divisor += 2) {
            if (value % divisor == 0) return 0;
        }
        return 1;
    }
    return 0;
}
