#ifndef TREENODE_H
#define TREENODE_H

#include <string>
#include <vector>

/*
 * TreeNode
 * --------
 * One node of a general (n-ary) decision tree stored as a linked structure.
 * Each node keeps a pointer to its parent and a list of pointers to its children,
 * so a node can have any number of children (e.g. Yes / Sometimes / No).
 *
 * Memory for nodes is owned and released by LinkedTree, never by the node itself.
 */
struct TreeNode {
    std::string content;              // question or answer text shown in the node
    std::string edgeLabel;            // label on the edge from the parent (empty for the root)
    int level;                        // depth of the node, root is level 0
    int position;                     // preorder position read from the input file
    TreeNode* parent;                 // nullptr for the root
    std::vector<TreeNode*> children;  // children in the order they were added (left to right)

    /*
     * Builds a node with no children.
     * nodeContent   text stored in the node
     * label         edge label from the parent
     * nodeLevel     depth of the node
     * nodePosition  preorder position
     * parentNode    pointer to the parent, nullptr for the root
     */
    TreeNode(const std::string& nodeContent,
             const std::string& label,
             int nodeLevel,
             int nodePosition,
             TreeNode* parentNode)
        : content(nodeContent),
          edgeLabel(label),
          level(nodeLevel),
          position(nodePosition),
          parent(parentNode) {}

    // Returns true if this node has no parent.
    bool isRoot() const { return parent == nullptr; }

    // Returns true if this node is a leaf (external node).
    bool isExternal() const { return children.empty(); }

    // Returns true if this node has at least one child (internal node).
    bool isInternal() const { return !children.empty(); }

    // Returns the number of children of this node.
    int childCount() const { return static_cast<int>(children.size()); }
};

#endif // TREENODE_H
