// Aligns the second argument up to 16 and loads the word at +4 from it.
int func_08972BB4(int unused, int p) { int *q = (int *)((p + 15) & -16); return q[1]; }
