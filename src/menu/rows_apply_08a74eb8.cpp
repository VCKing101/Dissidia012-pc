struct ApplyRow {
    char data[0x14];
};

extern "C" void func_08A74B8C(ApplyRow *row, int value);

extern "C" {
void func_08A74EB8(ApplyRow *rows, int value)
{
    for (int i = 0; i < 3; i++) {
        func_08A74B8C(&rows[i], value);
    }
}
}
