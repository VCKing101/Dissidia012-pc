/* Float parameters drawn at random between a stored minimum and maximum. */

typedef struct Agent4RangeSet {
    unsigned int kind;
    float field_04;
    float field_08;
    float field_0C;
    float field_10;
    float field_14;
    float field_18;
    float field_1C;
    float field_20;
    float field_24;
    float field_28;
    float field_2C;
    float field_30;
    float field_34;
    float field_38;
    float field_3C;
    float field_40;
    float field_44;
    float field_48;
} Agent4RangeSet;

typedef struct Agent4RangeOwner {
    int field_00;
    int field_04;
    Agent4RangeSet *ranges;
    unsigned char unknown_0C[0x30];
    float field_3C;
} Agent4RangeOwner;

/* Random float between min and max (a method; self is unused). */
float func_0881FDE4(Agent4RangeOwner *self, float min, float max);

void func_0881FC30(Agent4RangeOwner *self, float *values) {
    values[self->ranges->kind - 3] += self->field_3C;
}

float func_0881FC58(Agent4RangeOwner *self) {
    return func_0881FDE4(self, self->ranges->field_24, self->ranges->field_28);
}

float func_0881FC7C(Agent4RangeOwner *self) {
    return func_0881FDE4(self, self->ranges->field_04, self->ranges->field_08);
}

float func_0881FCA0(Agent4RangeOwner *self) {
    return func_0881FDE4(self, self->ranges->field_14, self->ranges->field_18);
}

float func_0881FCC4(Agent4RangeOwner *self) {
    return func_0881FDE4(self, self->ranges->field_0C, self->ranges->field_10);
}

float func_0881FCE8(Agent4RangeOwner *self) {
    return func_0881FDE4(self, self->ranges->field_1C, self->ranges->field_20);
}

float func_0881FD0C(Agent4RangeOwner *self) {
    return func_0881FDE4(self, self->ranges->field_3C, self->ranges->field_40);
}

float func_0881FD30(Agent4RangeOwner *self) {
    return func_0881FDE4(self, self->ranges->field_44, self->ranges->field_48);
}
