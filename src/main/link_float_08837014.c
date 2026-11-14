typedef struct Agent4Link {
    unsigned char unknown_0000[0x18];
    float value;
} Agent4Link;

typedef struct Agent4Linked {
    unsigned char unknown_0000[0x3A0];
    Agent4Link *link;
} Agent4Linked;

float func_08837014(Agent4Linked *self) {
    return self->link != 0 ? self->link->value : 0.0f;
}
