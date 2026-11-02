#ifndef LINKEDTREE_H
#define LINKEDTREE_H

#include <string>
#include "TreeNode.h"

/*
 * LinkedTree
 * ----------
 * A general tree built from dynamically allocated TreeNode objects.
 * The tree owns every node it creates and frees all of them in its destructor.
 *
 * Typical use (Person 2, file construction):
 *     LinkedTree tree;
 *     TreeNode* root = tree.addRoot("Are you nervous?", 0, 1);
 *     TreeNode* child = tree.addChild(root, "Yes", "Savings account", 1, 2);
 */
class LinkedTree {
public:
    // Creates an empty tree.
    LinkedTree();

    // Frees every node in the tree.
    ~LinkedTree();

    // The tree owns raw pointers, so copying is disabled to avoid double deletes.
    LinkedTree(const LinkedTree&) = delete;
    LinkedTree& operator=(const LinkedTree&) = delete;

    /*
     * Creates the root node and returns a pointer to it.
     * Throws std::logic_error if the tree already has a root.
     */
    TreeNode* addRoot(const std::string& content, int level = 0, int position = 1);

    /*
     * Creates a new node, attaches it as the last child of parent, and returns it.
     * Throws std::invalid_argument if parent is nullptr, if level is not parent->level + 1,
     * or if another node already uses the same preorder position.
     */
    TreeNode* addChild(TreeNode* parent,
                       const std::string& edgeLabel,
                       const std::string& content,
                       int level,
                       int position);

    // Returns the root node, or nullptr if the tree is empty.
    TreeNode* getRoot() const;

    // Returns the node with the given preorder position, or nullptr if none exists.
    TreeNode* findNode(int position) const;

    // Returns the total number of nodes in the tree.
    int size() const;

    // Returns true if the tree has no nodes.
    bool empty() const;

    // Deletes every node and resets the tree to empty.
    void clear();

private:
    TreeNode* root;   // root of the tree, nullptr when empty
    int nodeCount;    // number of nodes currently in the tree

    // Recursive preorder search used by findNode.
    static TreeNode* findNodeInSubtree(TreeNode* node, int position);

    // Recursive postorder deletion of a node and all of its descendants.
    static void destroySubtree(TreeNode* node);
};

#endif // LINKEDTREE_H
