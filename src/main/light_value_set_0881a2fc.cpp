/* Sets a value of the light table and updates the dependent entries. */
struct LightTable0881A2FC {
    unsigned char ready;
    char pad_01[3];
    unsigned int count;
    int *values;
};

extern "C" {

void func_0881A454(LightTable0881A2FC *self, unsigned int index, unsigned char value, unsigned char flag);
void func_0881A594(LightTable0881A2FC *self, unsigned int index, unsigned char value, unsigned char flag);
void func_0881AC8C(LightTable0881A2FC *self, unsigned int index, unsigned char flag);

int func_0881A2FC(LightTable0881A2FC *self, unsigned int index, unsigned char value, unsigned char flag)
{
    if (!self->ready) {
        return -1;
    }
    if (index >= self->count) {
        return -2;
    }
    int *v = &self->values[index];
    if (*v == value) {
        return -3;
    }
    *v = value;
    func_0881A454(self, index, value, flag);
    func_0881A594(self, index, value, flag);
    func_0881AC8C(self, index, flag);
    return 0;
}

}
