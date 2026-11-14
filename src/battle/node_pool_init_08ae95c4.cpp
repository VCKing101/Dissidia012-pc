typedef struct PoolNode {
    char pad_00[0x2C];
    PoolNode *next;
    char pad_30[0x1C];
} PoolNode;

typedef struct NodePool {
    PoolNode *head;
    int field_04;
    int field_08;
    PoolNode nodes[4];
    PoolNode *free[4];
    int free_count;
    char pad_150[0xB0];
    int field_200;
    unsigned char field_204;
    unsigned char field_205;
} NodePool;

extern "C" {

NodePool *func_08AE95C4(NodePool *self)
{
    self->field_04 = 0;
    self->head = 0;
    self->field_08 = 0;
    self->free_count = 0;
    while (self->free_count < 4) {
        self->free[self->free_count] = &self->nodes[3 - self->free_count];
        self->free_count++;
    }
    self->field_200 = 0;
    self->field_204 = 0;
    self->field_205 = 0;
    return self;
}

}
