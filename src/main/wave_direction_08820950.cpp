/* Switches a wave's direction, keeping its relative position; fails with
   -2 when the new span is empty. */
struct Wave08820950 {
    unsigned char ready;
    char pad_01[0x1C - 1];
    unsigned char direction;
    char pad_1D[3];
    float position;
    float length;
};

extern "C" {

float func_08820C74(Wave08820950 *self);
float func_08820C98(Wave08820950 *self);

int func_08820950(Wave08820950 *self, unsigned char direction)
{
    if (!self->ready) {
        return -1;
    }
    if (self->direction != direction) {
        float rest = 1.0f - self->position / self->length;
        float length = direction == 1 ? func_08820C74(self) : func_08820C98(self);
        if (length <= 0.0f) {
            return -2;
        }
        self->direction = direction;
        self->length = length;
        self->position = length * rest;
    }
    return 0;
}

}
