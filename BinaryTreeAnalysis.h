#ifndef BINARY_TREE_ANALYSIS_H
#define BINARY_TREE_ANALYSIS_H

#include "LinkedTree.h"
#include <ostream>

/* BinaryTreeAnalysis is a set of read-only functions that analyze the properties of a LinkedTree as a binary tree.
   1. Is it a Binary Tree? = isBinary()
   2. Is it a Proper Binary Tree? = isProper()
   3. Is it a Perfect Binary Tree? = isPerfect()
   4. Is it Balanced? = isBalanced()
   printBinaryTreeAnalysis() prints all four in that order.
*/

// Returns true if the tree is a binary tree (each node has at most two children).
bool isBinary(const LinkedTree& tree);

// Returns true if the tree is a proper binary tree (each node has either zero or two children).
bool isProper(const LinkedTree& tree);

// Returns true if the tree is a perfect binary tree (all leaves are at the same level).
bool isPerfect(const LinkedTree& tree);

// Returns false for non-binary trees; otherwise checks that child subtree heights
// differ by at most one at every node.
bool isBalanced(const LinkedTree& tree);

// Prints the analysis of the binary tree, including whether it is binary, proper, perfect, and balanced.
void printBinaryTreeAnalysis(const LinkedTree& tree, std::ostream& out);

#endif
 