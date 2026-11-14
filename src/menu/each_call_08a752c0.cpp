struct EachRec {
    int field_00;
};

extern "C" {

void func_088D2F30(EachRec *rec);

void func_08A752C0(EachRec *self)
{
    for (int i = 0; i < 3; i++) {
        func_088D2F30(&self[i]);
    }
}

}
