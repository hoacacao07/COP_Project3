// Read-only operations for analyzing binary-tree properties.
#ifndef BINARY_TREE_ANALYSIS_H
#define BINARY_TREE_ANALYSIS_H

#include "LinkedTree.h"
#include <ostream>

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
 