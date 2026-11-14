struct Base08841EF8 {
    unsigned char unknown_000[0x4A0];
    Base08841EF8 *next;
    unsigned char unknown_4A4[0xA0];
    virtual void v00();
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void v04();
    virtual void v05();
    virtual void v06();
    virtual void v07();
    virtual void v08();
    virtual void v09();
    virtual void v10();
    virtual void v11();
    virtual void v12();
    virtual void v13();
    virtual void v14();
    virtual void v15();
    virtual void v16();
    virtual void v17();
    virtual void v18();
    virtual void v19();
    virtual void v20();
    virtual void v21();
    virtual void v22();
    virtual void v23();
    virtual void v24();
    virtual void v25();
    virtual void v26();
    virtual void v27();
    virtual void v28();
    virtual void v29();
    virtual void update30();
};

struct Root08841EF8 {
    unsigned char unknown_00[0xC];
    Base08841EF8 *head;

    static Base08841EF8 *after(Root08841EF8 *root, Base08841EF8 *obj) {
        return obj == 0 ? root->head : obj->next;
    }
};

extern "C" {
extern Root08841EF8 *D_089D4958;

void func_08841EF8(void) {
    Root08841EF8 *root = D_089D4958;
    Base08841EF8 *obj = 0;
    while ((obj = Root08841EF8::after(root, obj)) != 0) {
        obj->update30();
    }
}
}
