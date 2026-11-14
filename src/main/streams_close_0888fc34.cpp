struct Owner0888FC34 {
    int streams[2];
    int field_08;
};

extern "C" {
void func_088DA060(int stream);
void func_0887BA5C(int handle);

void func_0888FC34(Owner0888FC34 *self) {
    for (int i = 0; i < 2; i++) {
        if (self->streams[i] != 0) {
            func_088DA060(self->streams[i]);
            self->streams[i] = 0;
        }
    }
    if (self->field_08 != 0) {
        func_0887BA5C(self->field_08);
        self->field_08 = 0;
    }
}
}
