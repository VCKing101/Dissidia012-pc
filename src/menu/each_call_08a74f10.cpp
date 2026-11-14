struct EachRec {
    char pad_00[0x14];
};

extern "C" {

void func_08A74BA8(EachRec *rec);

void func_08A74F10(EachRec *self)
{
    for (int i = 0; i < 3; i++) {
        func_08A74BA8(&self[i]);
    }
}

}
