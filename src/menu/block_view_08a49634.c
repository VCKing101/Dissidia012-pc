typedef struct BlockHeader {
    int field_00;
    int field_04;
    short field_08;
    short field_0A;
    short field_0C;
    short field_0E;
} BlockHeader;

typedef struct BlockRecord {
    short field_00;
    short field_02;
    short field_04;
} BlockRecord;

typedef struct BlockView {
    BlockHeader *field_00;
    BlockRecord *field_04;
    BlockRecord *field_08;
    BlockRecord *field_0C;
    int *field_10;
} BlockView;

void func_08A49634(BlockView *self, BlockHeader *header)
{
    BlockRecord *first;
    BlockRecord *second;
    BlockRecord *third;
    self->field_00 = header;
    first = (BlockRecord *)(header + 1);
    self->field_04 = first;
    second = first + header->field_08;
    self->field_08 = second;
    third = second + header->field_0A;
    self->field_0C = third;
    self->field_10 = (int *)third + header->field_0C;
}
