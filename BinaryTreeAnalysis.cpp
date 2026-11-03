#include "BinaryTreeAnalysis.h"

#include <algorithm>
#include <cstdlib>

namespace {

    /* *1. isBinaryHelper checks if a node and its children satisfy the binary tree property (each node has at most two children).
       * Node is the root of the subtree to check, may be nullptr (returns true)
       * Logic: If the node is nullptr, return true (base case). If the node has more than two children, return false. 
       * Otherwise, recursively check each child.
    */
    bool isBinaryHelper(const TreeNode* node) {
        if (node == nullptr) {
            return true; 
        }

        if (node->childCount() > 2) {
            return false;
        }

        // Recursively check each child
        for (const TreeNode* child : node->children) { 
            if (!isBinaryHelper(child)) { // If any child subtree is not binary, the whole tree is not binary
                return false;
            }
        }

        return true;
    }

    /* *2. isProperHelper checks if it has the proper binary tree property
        * where each node has either zero or two children.
        * Logic: If the node has 1 child, return false. Otherwise, recursively check each child.
    */
    bool isProperHelper(const TreeNode* node) {
        if (node == nullptr) { 
            return true; 
        }

        int childCount = node->childCount(); // Get the number of children of the current node
        if (childCount != 0 && childCount != 2) { 
            return false;
        }

        // Recursively check each child
        for (const TreeNode* child : node->children) {
            if (!isProperHelper(child)) { // If any child subtree is not proper, the whole tree is not proper
                return false;
            }
        }

        return true;
    }

    // 3. isPerfectHelper checks that every internal node has two children and all leaves share a depth.

    bool isPerfectHelper(const TreeNode* node, int depth, int& leafDepth) {
        if (node == nullptr) {
            return true;
        }

        if (node->isExternal()) { // If the node is a leaf
            if (leafDepth == -1) { // If this is the first leaf encountered
                leafDepth = depth; // Set the leaf depth
            } else if (leafDepth != depth) { // If this leaf's depth doesn't match the first leaf's depth
                return false;
            }
            return true;
        }

        if (node->childCount() != 2) { // If the node is internal but doesn't have exactly two children, it's not perfect
            return false;
        }

        // Recursively check each child, increasing the depth by 1
        for (const TreeNode* child : node->children) {
            if (!isPerfectHelper(child, depth + 1, leafDepth)) { // If any child subtree is not perfect, the whole tree is not perfect
                return false;
            }
        }

        return true;
    }

    // 4. isBalancedHelper computes subtree height and checks that child subtree heights differ by at most one.
    
    bool isBalancedHelper(const TreeNode* node, int& height) {
        if (node == nullptr) { 
            height = -1; // Heights are measured in edges; an empty subtree has height -1.
            return true; 
        }

        // Initialize variables to track the heights of the left and right subtrees
        int leftHeight = -1; 
        int rightHeight = -1;

        // Initialize variables to track the balance status of the left and right subtrees
        bool leftBalanced = true; 
        bool rightBalanced = true; 

        // Recursively check the left and right children, if they exist, and update their heights
         if (node->childCount() > 0) {
             leftBalanced = isBalancedHelper(node->children[0], leftHeight);
        }

        if (node->childCount() > 1) {
            rightBalanced = isBalancedHelper(node->children[1], rightHeight); 
        }
        
        // The +1 accounts for the edge connecting the current node to its children
        height = std::max(leftHeight, rightHeight) + 1;

        // If either subtree is unbalanced, or if the height difference exceeds 1, the current subtree is unbalanced
        if (!leftBalanced || !rightBalanced) {
            return false;
        }

        // Difference of heights cannot exceed 1
        if (std::abs(leftHeight - rightHeight) > 1) { 
            return false; 
        }

        return true;
    }

}

    // Returns true if the tree is a binary tree (each node has at most two children).
    bool isBinary(const LinkedTree& tree) {
        return isBinaryHelper(tree.getRoot());
    }

    // Returns true if the tree is a proper binary tree (each node has either zero or two children).
    bool isProper(const LinkedTree& tree) {
        return isProperHelper(tree.getRoot());
    }

    // Returns true if the tree is a perfect binary tree (all leaves are at the same level).
    bool isPerfect(const LinkedTree& tree) {
        int leafDepth = -1;
        return isPerfectHelper(tree.getRoot(), 0, leafDepth);
    }

    // Returns true if a binary tree's child subtree heights differ by at most one at every node.
    bool isBalanced(const LinkedTree& tree) {
        if (!isBinary(tree)) {
            return false;
        }

        int height = 0;
        return isBalancedHelper(tree.getRoot(), height);
    }

    // 5. Prints the analysis of the binary tree, including whether it is binary, proper, perfect, and balanced.
    void printBinaryTreeAnalysis(const LinkedTree& tree, std::ostream& out) { 
        bool isBin = isBinary(tree); // Tracks whether the tree is binary

        out << "Binary Tree Analysis: " << "\n"; 
        
        out << "Is it a Binary Tree? " << (isBin ? "Yes" : "No") << "\n";

        // First check if the tree is binary before checking for proper, perfect, and balanced properties, as these properties are only relevant for binary trees.
        if(isBin) {
            out << "Proper Tree: " << (isProper(tree) ? "Yes" : "No") << "\n";
            out << "Perfect Tree: " << (isPerfect(tree) ? "Yes" : "No") << "\n";
            out << "Balanced Tree: " << (isBalanced(tree) ? "Yes" : "No") << "\n";
        }
    }
