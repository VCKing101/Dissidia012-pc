typedef struct Stream { int base; int start; int field_08; int field_0C; int field_10; int cur; int field_18; int offset; unsigned char mode; } Stream;
void func_0885C508(Stream *self)
{
    self->cur = self->start;
    self->field_18 = 0;
    self->offset = self->base - (int)&self->cur;
    self->mode = self->field_0C;
    self->field_10 = 0;
    self->field_08 = 0;
}
