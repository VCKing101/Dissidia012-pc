struct EachRec {
    char pad_00[0x14];
};

extern "C" {

void func_08A74E30(EachRec *rec);

void func_08A751D0(EachRec *self)
{
    for (int i = 0; i < 3; i++) {
        func_08A74E30(&self[i]);
    }
}

}
