extern "C" {
int func_08A7F104(int self, int a, int b);

bool func_08A81204(int self) {
    bool ok = true;
    int r = func_08A7F104(self, 2, -1);
    if (r < 0) {
        ok = false;
    }
    return ok;
}
}
