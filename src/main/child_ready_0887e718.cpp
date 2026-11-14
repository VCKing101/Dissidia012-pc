struct Child0887E718 {
    unsigned char data[0x24];
    virtual void v00();
    virtual void v01();
    virtual int ready02();
};

struct Owner0887E718 {
    unsigned char unknown_00[0x14];
    Child0887E718 *child;
};

extern "C" {
int func_0887E718(Owner0887E718 *self) {
    return self->child->ready02() != 0;
}
}
