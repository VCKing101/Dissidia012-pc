struct Node088082CC {
    unsigned char unknown_0000[0x6C];
    Node088082CC *next;
};

extern "C" Node088082CC *D_089D2800;

struct NodeList088082CC {
    static Node088082CC *after(Node088082CC *node) {
        return node == 0 ? D_089D2800 : node->next;
    }
    static bool contains(Node088082CC *target) {
        Node088082CC *node = 0;
        while ((node = after(node)) != 0) {
            if (node == target) {
                return true;
            }
        }
        return false;
    }
};

extern "C" {
unsigned char func_088082CC(Node088082CC *target) {
    int result = 0;
    if (target != 0) {
        if (NodeList088082CC::contains(target)) {
            result = 1;
        }
    }
    return result;
}
}
