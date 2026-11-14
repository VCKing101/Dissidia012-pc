/* Inverse of a random rate drawn from the first range (1 when not positive). */
typedef struct Range08820B54 {
    float min;
    float max;
} Range08820B54;

typedef struct Ranges08820B54 {
    int field_00;
    Range08820B54 first;
    int field_0C;
    int field_10;
    Range08820B54 second;
} Ranges08820B54;

typedef struct Owner08820B54 {
    int field_00;
    Ranges08820B54 *ranges;
} Owner08820B54;

float func_08820CBC(Owner08820B54 *self, float lo, float hi);

float func_08820B54(Owner08820B54 *self)
{
    float rate = func_08820CBC(self, self->ranges->first.min, self->ranges->first.max);
    if (rate <= 0.0f) {
        rate = 1.0f;
    }
    return 1.0f / rate;
}
