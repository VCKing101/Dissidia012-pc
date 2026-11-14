/* Releases the four effect slots of a bank, clears them and frees the extra block. */
struct FxItem088AB500;

struct FxBank088AB500 {
    FxItem088AB500 *items[4];
    void *extra;
};

extern "C" {

extern int D_089D6D78;

void func_088A9B18(FxItem088AB500 *item);
void func_088A5604(int heap, FxItem088AB500 *item);
void *func_08935828(void *dst, int value, int size);
void func_088A522C(void *block);

void func_088AB500(FxBank088AB500 *bank)
{
    int heap = D_089D6D78;
    for (int i = 0; i < 4; i++) {
        FxItem088AB500 *item = bank->items[i];
        if (item != 0) {
            func_088A9B18(item);
            func_088A5604(heap, item);
        }
    }
    func_08935828(bank, 0, 0x10);
    if (bank->extra != 0) {
        func_088A522C(bank->extra);
    }
}

}
