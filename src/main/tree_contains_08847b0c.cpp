struct TreeNode {
    int field_00;
};

struct Tree {
    char pad_00[0x10];
    TreeNode header;
};

struct TreeOwner {
    char pad_00[0x34];
    Tree tree;
};

extern "C" {

TreeNode *func_08847CF8(Tree *t, TreeNode *end, int *key);

bool func_08847B0C(TreeOwner *o, int key)
{
    Tree *t = &o->tree;
    return func_08847CF8(t, &t->header, &key) != &t->header;
}

}
