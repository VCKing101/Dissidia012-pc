typedef struct MenuKind { unsigned char field_00; } MenuKind;
int func_089F5BD4(void *self, MenuKind *kind) { int result = 1; if (kind->field_00 > 3 && kind->field_00 < 8) { result = 0; } return result; }
