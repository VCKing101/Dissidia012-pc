typedef struct Agent4Link {
    unsigned char unknown_0000[0xC];
    float field_0C;
    float field_10;
    float field_14;
} Agent4Link;

typedef struct Agent4Linked {
    unsigned char unknown_0000[0x3A0];
    Agent4Link *link;
} Agent4Linked;

float func_088370FC(Agent4Linked *self) {
    Agent4Link *link = self->link;
    if (link != 0) {
        return link->field_0C + link->field_14;
    } else {
        return 0.0f;
    }
}
