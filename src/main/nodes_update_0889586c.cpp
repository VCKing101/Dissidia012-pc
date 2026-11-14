struct Node0889586C {
    int field_00;
    Node0889586C *next;
    virtual void v00();
    virtual void v01();
    virtual void update02();
};

struct List0889586C {
    Node0889586C *head;

    static Node0889586C *after(List0889586C *list, Node0889586C *node) {
        return node == 0 ? list->head : node->next;
    }
};

extern "C" {
int func_089A04E8(void);
void func_0899FDC4(int lock);

void func_0889586C(List0889586C *list) {
    Node0889586C *node = 0;
    int lock = func_089A04E8();
    while ((node = List0889586C::after(list, node)) != 0) {
        node->update02();
    }
    func_0899FDC4(lock);
}
}
