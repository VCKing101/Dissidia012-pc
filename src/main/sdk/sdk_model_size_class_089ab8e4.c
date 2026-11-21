/* Model library: size class of a small allocation (3: up to 4 bytes, 2: up to 16, 1: up to 64, 0: larger),
 * the same rule as the GMO texture library's func_089BAA3C. */
int func_089AB8E4(unsigned int size)
{
    if (size < 5)
        return 3;
    if (size < 17)
        return 2;
    return size < 65;
}
