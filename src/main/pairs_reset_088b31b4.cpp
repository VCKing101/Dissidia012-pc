/* Resets the 19 0x2C-byte vector pairs. */
struct Pair088B31B4 {
    char data[0x2C];
};

extern "C" {

void func_088B3174(Pair088B31B4 *pair);

void func_088B31B4(Pair088B31B4 *pairs)
{
    for (int i = 0; i < 19; i++) {
        func_088B3174(&pairs[i]);
    }
}

}
