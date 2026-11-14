struct EachRec {
    char pad_00[0x14];
};

extern "C" {

void func_08A74D84(EachRec *rec);

void func_08A75140(EachRec *self)
{
    for (int i = 0; i < 3; i++) {
        func_08A74D84(&self[i]);
    }
}

}
