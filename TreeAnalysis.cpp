#include "TreeAnalysis.h"
 
#include <algorithm>
 
namespace {
 
/* visitPreorder Recursive preorder: record the node first, then visit each child from left to right (the order the parser added them).
 node is the root of the subtree to walk, may be nullptr out is the list that receives the nodes in the order they are visited */
void visitPreorder(TreeNode* node, std::vector<TreeNode*>& out)
{
    if (node == nullptr)
    {
        return;
    }
 
    out.push_back(node);                 // 1. visit the node itself
    for (TreeNode* child : node->children)
    {
        visitPreorder(child, out);       // 2. then every child, left to right
    }
}
 
/* heightOf returns the recursive height of a subtree, counted in edges. A leaf has height 0. Any other node has 1 + the tallest child height.
 node is the root of the subtree, may be nullptr (returns -1) */
int heightOf(const TreeNode* node)
{
    if (node == nullptr)
    {
        return -1;
    }
 
    int tallestChild = -1;               // stays -1 for a leaf, so leaf = 0
    for (const TreeNode* child : node->children)
    {
        tallestChild = std::max(tallestChild, heightOf(child));
    }
    return tallestChild + 1;
}
 
/* printNodeList prints a numbered list of nodes, one per line. Each line shows the list number, the node's preorder position, and its text.
 nodes is the list to print (already in preorder) out is the stream that receives the text */
void printNodeList(const std::vector<TreeNode*>& nodes, std::ostream& out)
{
    int number = 1;
    for (const TreeNode* node : nodes)
    {
        out << "   " << number << ") [" << node->position << "] "
            << node->content << "\n";
        ++number;
    }
}
 
} // namespace
 
/* preorder collects the whole tree in preorder starting from the root. */
std::vector<TreeNode*> preorder(const LinkedTree& tree)
{
    std::vector<TreeNode*> nodes;
    nodes.reserve(static_cast<std::size_t>(tree.size()));
    visitPreorder(tree.getRoot(), nodes);
    return nodes;
}
 
/* getRootNode returns the root of the tree, or nullptr if the tree is empty. */
TreeNode* getRootNode(const LinkedTree& tree)
{
    return tree.getRoot();
}
 
/* getInternalNodes filters the preorder list, keeping only nodes that have children. */
std::vector<TreeNode*> getInternalNodes(const LinkedTree& tree)
{
    std::vector<TreeNode*> internal;
    for (TreeNode* node : preorder(tree))
    {
        if (node->isInternal())
        {
            internal.push_back(node);
        }
    }
    return internal;
}
 
/* getExternalNodes filters the preorder list, keeping only leaves (nodes with no children). */
std::vector<TreeNode*> getExternalNodes(const LinkedTree& tree)
{
    std::vector<TreeNode*> external;
    for (TreeNode* node : preorder(tree))
    {
        if (node->isExternal())
        {
            external.push_back(node);
        }
    }
    return external;
}
 
/* getHeight returns the height of the whole tree in edges, or -1 if the tree is empty. */
int getHeight(const LinkedTree& tree)
{
    return heightOf(tree.getRoot());
}
 
/* countInternal counts the number of nodes that have at least one child. */
int countInternal(const LinkedTree& tree)
{
    return static_cast<int>(getInternalNodes(tree).size());
}
 
/* countExternal counts the number of leaves. */
int countExternal(const LinkedTree& tree)
{
    return static_cast<int>(getExternalNodes(tree).size());
}
 
/* printAnalysis prints items 1 to 6. Each node list is numbered and shows the node's preorder position and text. */
void printAnalysis(const LinkedTree& tree, std::ostream& out)
{
    const TreeNode* root = getRootNode(tree);
 
    out << "\n--- Tree Properties ---\n";
 
    out << "1. Root Node: ";
    if (root != nullptr)
    {
        out << "[" << root->position << "] " << root->content << "\n";
    }
    else
    {
        out << "None (empty tree)\n";
    }
 
    out << "2. Number of Internal Nodes: " << countInternal(tree) << "\n";
    out << "3. Number of External (leaf) Nodes: " << countExternal(tree) << "\n";
    out << "4. Tree Height: " << getHeight(tree) << "\n";
 
    out << "5. Internal Nodes (preorder):\n";
    printNodeList(getInternalNodes(tree), out);
 
    out << "6. External Nodes (preorder):\n";
    printNodeList(getExternalNodes(tree), out);
}