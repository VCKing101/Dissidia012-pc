/* Selects the list node with this id; true when found. */
typedef struct SelectNode {
    char pad_00[0x4];
    int field_04;
    char pad_08[0x28 - 0x8];
    struct SelectNode *field_28;
} SelectNode;

typedef struct SelectNodes {
    SelectNode *field_00;
    int field_04;
    int field_08;
    SelectNode *field_0C;
} SelectNodes;

typedef struct SelectNodeList {
    char pad_000[0x240];
    SelectNodes field_240;
} SelectNodeList;

int func_08A94724(void *self, SelectNodeList *list, int id)
{
    int found = 0;
    SelectNodes *nodes = &list->field_240;
    int i = 0;
    int result = 0;
    SelectNode *node = 0;
    for (; i < nodes->field_08; i++) {
        node = node == 0 ? nodes->field_00 : node->field_28;
        if (node->field_04 == id) {
            found = 1;
            break;
        }
    }
    if (found == 1) {
        list->field_240.field_0C = node;
        result = 1;
    }
    return result;
}
