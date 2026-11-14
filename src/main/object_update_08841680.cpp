struct Base08841680 {
    unsigned char data[0x544];
    virtual void v00();
    virtual void v01();
    virtual void v02();
    virtual void update03();
};

struct Object08841680 : Base08841680 {
    unsigned char unknown_548[0xBCC];
    Object08841680 *next;
};

struct Root08841680 {
    unsigned char unknown_00[0x14];
    Object08841680 *head;

    static Object08841680 *after(Root08841680 *root, Object08841680 *obj) {
        return obj == 0 ? root->head : obj->next;
    }
};

extern "C" {
extern Root08841680 *D_089D4958;

void func_08841680(void) {
    Root08841680 *root = D_089D4958;
    Object08841680 *obj = 0;
    while ((obj = Root08841680::after(root, obj)) != 0) {
        obj->update03();
    }
}
}
