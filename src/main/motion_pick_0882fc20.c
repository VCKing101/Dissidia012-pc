typedef struct Agent4MotionEntry {
    short field_00;
    unsigned short motion;
} Agent4MotionEntry;

typedef struct Agent4MotionUser {
    unsigned char unknown_0000[0x4C];
    Agent4MotionEntry *current;
    short current_id;
} Agent4MotionUser;

int func_0882F664(Agent4MotionUser *self, int id);
void func_0882F4E4(Agent4MotionUser *self, int motion);
void func_0882F504(Agent4MotionUser *self, int motion);

void func_0882FC20(Agent4MotionUser *self, int id) {
    if (id >= 0 && id == self->current_id) {
        func_0882F4E4(self, self->current->motion);
    } else {
        func_0882F4E4(self, func_0882F664(self, id));
    }
}

void func_0882FC84(Agent4MotionUser *self, int id) {
    if (id >= 0 && id == self->current_id) {
        func_0882F504(self, self->current->motion);
    } else {
        func_0882F504(self, func_0882F664(self, id));
    }
}
