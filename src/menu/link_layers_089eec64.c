/* Attaches the panel's layers to their parents; the mode picks layer 0's depth. */
typedef struct LayerPanel {
    void *field_00;
    void *field_04;
    void *field_08;
    void *field_0C;
    void *field_10;
    void *field_14;
    void *field_18;
    int field_1C;
} LayerPanel;

extern void func_089EED14(void *child, void *parent, int depth);
extern void func_089EED6C(void *child, void *parent, int depth);

void func_089EEC64(LayerPanel *self)
{
    func_089EED14(self->field_10, self->field_14, 0);
    func_089EED14(self->field_0C, self->field_14, 0);
    func_089EED14(self->field_04, self->field_0C, 4);
    func_089EED14(self->field_08, self->field_0C, 3);
    switch (self->field_1C) {
    case 0:
        func_089EED14(self->field_00, self->field_0C, 4);
        break;
    case 1:
        func_089EED14(self->field_00, self->field_0C, 3);
        break;
    }
    func_089EED6C(self->field_18, self->field_0C, 5);
}
