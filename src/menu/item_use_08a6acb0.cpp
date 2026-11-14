/* Uses the selected item: kinds 0-4 go to step 1, kind 5 to step 3; returns 1 when something happened. */
typedef struct UseOwner {
    char pad_000[0x2C];
    int field_2C;
    char pad_030[0x630 - 0x30];
    unsigned int field_630;
} UseOwner;

extern "C" {
int func_08A6AD5C(UseOwner *self, int kind);
void func_08A6B044(UseOwner *self, int kind, int index);
int func_08A6AE24(UseOwner *self);
void func_08A69FE8(UseOwner *self);

bool func_08A6ACB0(UseOwner *self)
{
    unsigned int kind = self->field_630;
    switch (kind) {
    case 0:
    case 1:
    case 2:
    case 3:
    case 4:
        if (func_08A6AD5C(self, kind) != 0) {
            self->field_2C = 1;
            func_08A6B044(self, kind, -1);
            return true;
        }
        break;
    case 5:
        if (func_08A6AE24(self) != 0) {
            func_08A69FE8(self);
            self->field_2C = 3;
            return true;
        }
        break;
    }
    return false;
}
}
