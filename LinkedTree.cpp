#include "LinkedTree.h"

#include <stdexcept>

// Constructor, starts with an empty tree
LinkedTree::LinkedTree() : root(nullptr), nodeCount(0) {}

// Destructor, releases every node so no memory is leaked
LinkedTree::~LinkedTree()
{
    clear();
}

/*
 addRoot
 Creates the root node of the tree.
 content   text of the root node
 level     depth of the root (normally 0)
 position  preorder position of the root (normally 1)
 Returns a pointer to the new root.
 */
TreeNode *LinkedTree::addRoot(const std::string &content, int level, int position)
{
    if (root != nullptr)
    {
        throw std::logic_error("addRoot failed, the tree already has a root");
    }

    root = new TreeNode(content, "", level, position, nullptr);
    nodeCount = 1;
    return root;
}

/*
 addChild
 Creates a new node and links it as the last child of parent.
 parent     node that will receive the new child
 edgeLabel  label on the edge from parent to the new node (e.g. "Yes")
 content    text of the new node
 level      depth of the new node, must equal parent->level + 1
 position   preorder position of the new node, must be unique
 Returns a pointer to the new child.
 */
TreeNode *LinkedTree::addChild(TreeNode *parent,
                               const std::string &edgeLabel,
                               const std::string &content,
                               int level,
                               int position)
{
    if (parent == nullptr)
    {
        throw std::invalid_argument("addChild failed, parent is null");
    }
    if (level != parent->level + 1)
    {
        throw std::invalid_argument("addChild failed, child level must be parent level + 1");
    }
    if (findNode(position) != nullptr)
    {
        throw std::invalid_argument("addChild failed, position " +
                                    std::to_string(position) + " is already used");
    }

    TreeNode *child = new TreeNode(content, edgeLabel, level, position, parent);
    parent->children.push_back(child);
    ++nodeCount;
    return child;
}

/*
 getRoot
 Returns the root node, or nullptr if the tree is empty.
 */
TreeNode *LinkedTree::getRoot() const
{
    return root;
}

/*
 findNode
 Searches the whole tree for the node with the given preorder position.
 Returns the node if found, otherwise nullptr.
 */
TreeNode *LinkedTree::findNode(int position) const
{
    return findNodeInSubtree(root, position);
}

/*
 size
 Returns the number of nodes in the tree.
 */
int LinkedTree::size() const
{
    return nodeCount;
}

/*
 empty
 Returns true if the tree has no nodes.
 */
bool LinkedTree::empty() const
{
    return nodeCount == 0;
}

/*
 clear
 Deletes every node and resets the tree to the empty state.
 */
void LinkedTree::clear()
{
    destroySubtree(root);
    root = nullptr;
    nodeCount = 0;
}

/*
 findNodeInSubtree
 Recursive preorder search. Visits the node first, then each child left to right.
 node      root of the subtree to search
 position  preorder position to look for
 Returns the matching node, or nullptr if it is not in this subtree.
 */
TreeNode *LinkedTree::findNodeInSubtree(TreeNode *node, int position)
{
    if (node == nullptr)
    {
        return nullptr;
    }
    if (node->position == position)
    {
        return node;
    }

    for (TreeNode *child : node->children)
    {
        TreeNode *match = findNodeInSubtree(child, position);
        if (match != nullptr)
        {
            return match;
        }
    }
    return nullptr;
}

/*destroySubtree
 Recursive postorder deletion. Deletes all children first, then the node itself,
 so no pointer is used after it has been freed.
 node  root of the subtree to delete
 */
void LinkedTree::destroySubtree(TreeNode *node)
{
    if (node == nullptr)
    {
        return;
    }

    for (TreeNode *child : node->children)
    {
        destroySubtree(child);
    }
    delete node;
}
