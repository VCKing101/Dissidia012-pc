struct Slot60 {
    char data[0x60];
};

extern "C" {

extern Slot60 *D_089D6008;
void func_08871DAC(Slot60 *slot);

void func_088720E4(void)
{
    for (int i = 0; i < 2; i++) {
        func_08871DAC(&D_089D6008[i]);
    }
}

}
