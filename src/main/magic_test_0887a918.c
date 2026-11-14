typedef struct Header0887A918 {
    int magic;
} Header0887A918;

int func_0887A918(Header0887A918 *header) {
    if (header->magic == 0x38465455) {
        return 1;
    }
    return 0;
}
