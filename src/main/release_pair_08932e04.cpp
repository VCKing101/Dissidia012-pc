/* Releases the two handles at 0x2C and 0x30 through func_088CF970(D_089D7830, handle). */
extern "C" int D_089D7830;
extern "C" void func_088CF970(int, int);
struct Rel08932E04 {
    static void rel(int h) { func_088CF970(D_089D7830, h); }
};
extern "C" void func_08932E04(int *self) {
    Rel08932E04::rel(self[0x2C / 4]);
    Rel08932E04::rel(self[0x30 / 4]);
}
