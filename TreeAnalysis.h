#ifndef TREEANALYSIS_H
#define TREEANALYSIS_H

#include <ostream>
#include <vector>
#include "LinkedTree.h"
#include "TreeNode.h"

/*
 * TreeAnalysis is a set of read-only functions that calculate the basic properties of a LinkedTree.
1. Root node = getRootNode()
2. Number of internal nodes = countInternal()
3. Number of external (leaf) nodes  = countExternal()
4. Tree height = getHeight()
5. Internal nodes (preorder) = getInternalNodes()
6. External nodes (preorder)  = getExternalNodes()
 printAnalysis() prints all six in that order.

 A tree with only a root has height 0, an empty tree has height -1.
 Returns every node of the tree in preorder (node first, then each child from left to right)
 or an empty vector for an empty tree.
 */
std::vector<TreeNode*> preorder(const LinkedTree& tree);

/* Returns the root node, or nullptr if the tree is empty.
 */
TreeNode* getRootNode(const LinkedTree& tree);

/*Returns the number of internal nodes (nodes with at least one child).
 */
int countInternal(const LinkedTree& tree);

/*Returns the number of external nodes (leaves).
 */
int countExternal(const LinkedTree& tree);

/* Returns the height of the tree in edges or -1 for an empty tree.
 */
int getHeight(const LinkedTree& tree);

/*Returns the internal nodes in preorder.
 */
std::vector<TreeNode*> getInternalNodes(const LinkedTree& tree);

/*Returns the external nodes (leaves) in preorder.
 */
std::vector<TreeNode*> getExternalNodes(const LinkedTree& tree);

/* Prints items 1 to 6 where each node list is numbered and shows the node's preorder position and text.
 */
void printAnalysis(const LinkedTree& tree, std::ostream& out);

#endif // TREEANALYSIS_H