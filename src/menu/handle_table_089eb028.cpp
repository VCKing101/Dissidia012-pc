struct MenuHandleTable {
    int field_00;
    int field_04[14];
};

extern "C" {
int func_088DAA68(int source);
void func_088DA060(int handle);

void func_089EB028(MenuHandleTable *self, int source) {
    for (int i = 0; i < 14; i++) {
        self->field_04[i] = func_088DAA68(source);
    }
}

void func_089EB07C(MenuHandleTable *self) {
    for (int i = 0; i < 14; i++) {
        if (self->field_04[i] != 0) {
            func_088DA060(self->field_04[i]);
            self->field_04[i] = 0;
        }
    }
}
}
