// Calls the world setup twice with the global world block, then looks up a record and uses its first byte when found.
extern "C" char *D_089D83D8;
extern "C" void func_08916BB4(char *w, int a);
extern "C" void func_088650A4(char *w);
extern "C" unsigned char *func_08912D20(int b, int c, int d);
extern "C" void func_08917B80(int v);
extern "C" void func_08916E50(int a, int b, int c, int d)
{
    char *w = D_089D83D8;
    func_08916BB4(w, a);
    func_088650A4(w + 4);
    unsigned char *r = func_08912D20(b, c, d);
    switch ((int)r) { case 0: break; default: func_08917B80(*r); break; }
}
