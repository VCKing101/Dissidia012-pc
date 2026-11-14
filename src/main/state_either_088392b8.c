typedef struct Actor088392B8 {
    int field_00;
} Actor088392B8;

int func_08838074(Actor088392B8 *self);
int func_08838160(Actor088392B8 *self);

unsigned char func_088392B8(Actor088392B8 *self) {
    return (func_08838074(self) == 1 || func_08838160(self) == 1) ? 1 : 0;
}
