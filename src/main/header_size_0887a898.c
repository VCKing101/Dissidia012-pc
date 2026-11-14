typedef struct Header0887A898 {
    short size;
    short unknown_02;
    int full_size;
} Header0887A898;

int func_0887A8F0(Header0887A898 *header);
int func_0887A918(Header0887A898 *header);

int func_0887A898(Header0887A898 *header) {
    int size = header->size;
    if (func_0887A8F0(header) != 0) {
        size = header->full_size;
    } else if (func_0887A918(header) != 0) {
        size = header->full_size;
    }
    return size;
}
