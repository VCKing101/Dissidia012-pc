/* Steps a 0-7 ring index forwards or backwards; other values pass through. */
int func_08A80340(int index, unsigned char forward)
{
    if ((unsigned int)index >= 8) {
        return index;
    }
    if (forward) {
        return (index + 1) % 8;
    }
    return (index + 7) % 8;
}
