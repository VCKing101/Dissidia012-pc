typedef struct Object08882F28 {
    int field_00;
} Object08882F28;

void func_08882FF4(Object08882F28 *self);
void func_089359A4(Object08882F28 *self, int arg);

Object08882F28 *func_08882F28(Object08882F28 *self, int arg) {
    func_08882FF4(self);
    func_089359A4(self, arg);
    return self;
}
