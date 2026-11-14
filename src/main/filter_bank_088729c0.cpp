/* The object at D_089D6008: two 0x60-byte slots, then two 0x50-byte filters at 0xC0. */
struct BankSlot {
    char data[0x60];
};

struct BankFilter {
    char data[0x50];
};

struct FilterBank {
    BankSlot slots[2];
    BankFilter filters[2];
};

extern "C" {

extern FilterBank *D_089D6008;
void func_08872510(BankFilter *f);
void func_08872540(BankFilter *f);

void func_088729C0(void)
{
    for (int i = 0; i < 2; i++) {
        func_08872510(&D_089D6008->filters[i]);
    }
}

void func_08872A18(void)
{
    for (int i = 0; i < 2; i++) {
        func_08872540(&D_089D6008->filters[i]);
    }
}

}
