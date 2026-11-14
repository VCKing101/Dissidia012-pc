short func_0886F9E4(const signed char *data, int length) {
    short hash = 0;
    for (; length != 0; length--) {
        short shifted = hash << 1;
        hash = shifted ^ *data;
        data++;
    }
    return hash;
}
