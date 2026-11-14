/* Once the page is ready, either closes the dialog (accepted) or runs virtual step 4; true when ready. */
class AnswerPage {
public:
    char pad_00[0x8];
    void *field_08;
    virtual void v0();
    virtual void v1();
    virtual void v2();
    virtual void v3();
    virtual void v4();
};

class AnswerPageData : public AnswerPage {
public:
    char pad_10[0x88 - 0x10];
    int field_88;
};

extern "C" void *D_08A4FFF4;
extern "C" int func_08A66C48(void *part);
extern "C" int func_08A4DFD8(void *dialog, int value);
extern "C" void func_08A4DE34(void *dialog);

extern "C" {
int func_08A6574C(AnswerPageData *self)
{
    if (func_08A66C48(self->field_08) == 0) {
        return 0;
    }
    if (func_08A4DFD8(D_08A4FFF4, self->field_88) == 0) {
        self->v4();
    } else {
        func_08A4DE34(D_08A4FFF4);
    }
    return 1;
}
}
