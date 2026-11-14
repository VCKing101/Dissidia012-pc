typedef struct CountHolder {
    char pad_00[0x8];
    int count;
} CountHolder;

struct IntClamp {
    static int clamp(int value, int min, int max)
    {
        if (value < min) {
            return min;
        }
        if (value > max) {
            return max;
        }
        return value;
    }
};

extern "C" {

void func_08AFE168(CountHolder *self, int add)
{
    self->count = IntClamp::clamp(self->count + add, 0, 9999);
}

}
