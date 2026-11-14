struct EachRec {
    char pad_00[0x14];
};

extern "C" {

void func_08A74DE0(EachRec *rec);

void func_08A75188(EachRec *self)
{
    for (int i = 0; i < 3; i++) {
        func_08A74DE0(&self[i]);
    }
}

}
