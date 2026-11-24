/* Returns whether bit 0 of the word pointed to by *p is set. */
int func_08B58C24(unsigned **p)
{
    if ((**p & 1) != 0) { return 1; } else { return 0; }
}
