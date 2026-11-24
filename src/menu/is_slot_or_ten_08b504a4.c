/* Returns 1 when idx is in 0..9 or equals 10, else 0. */
int func_08B504A4(int unused, int idx)
{
    if (idx >= 0 && idx < 10) { return 1; }
    else if (idx == 10) { return 1; }
    else { return 0; }
}
