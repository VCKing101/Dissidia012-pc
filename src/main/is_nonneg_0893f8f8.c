/* Returns whether the argument is within 0..0x7FFFFFFF as unsigned. */
int func_0893F8F8(unsigned a) {
    unsigned max = 0x7FFFFFFF;
    if (a > max) { return 0; }
    return 1;
}
