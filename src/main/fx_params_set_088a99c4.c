/* Stores up to three effect parameters; the first one is required. */
typedef struct FxParams088A99C4 FxParams088A99C4;

int func_088AA434(FxParams088A99C4 *fx, int value);
int func_088AA440(FxParams088A99C4 *fx, int value);
int func_088AA47C(FxParams088A99C4 *fx, int value);

int func_088A99C4(FxParams088A99C4 *fx, int first, int second, int third)
{
    if (first == 0) {
        return 0;
    }
    func_088AA434(fx, first);
    if (second != 0) {
        func_088AA440(fx, second);
    }
    if (third != 0) {
        func_088AA47C(fx, third);
    }
    return 1;
}
