#include "TreeReport.h"
#include <fstream>
#include <stdexcept>
/* TreeReport.cpp contains the implementation for writing tree reports */

namespace {
    /* createTreeVisualization()
    - Use dash-based indentation 
    - Level n → 2 × n dashes (no dashes for the root, 2 dashes for level 1, 4 dashes for level 2, etc.)
    - Include edge labels in [ ]
    */
    void createTreeVisualization (const TreeNode* node, std::ostream& out) {
        if (node == nullptr) {
            return;
        }

        if(node->level > 0) { // Not the root, so print edge label and content
            // 
            for(int i =0; i < node->level * 2; i++) {
                out << "-";
            }

            out << "[" << node->edgeLabel << "] " << node->content << "\n";
        } else {
            out << node->content << "\n";
        }

        for (const TreeNode* child : node->children) {
            createTreeVisualization(child, out);
        }
        
    }
}

/* writeTreeReport() will create a report (about_tree.txt) about the tree that will contain:
1. Tree Visualization = createTreeVisualization()
2. Tree Analysis = printAnalysis()
3. Binary Tree Analysis = printBinaryTreeAnalysis()
*/
void writeTreeReport(const LinkedTree& tree, const std::string& filename) {

    std::ofstream outFile(filename);
    if (!outFile) {
        throw std::runtime_error("Error: Failed to open file: " + filename);
    }

    outFile << "--- Tree Visualization ---\n";
    createTreeVisualization(tree.getRoot(), outFile);

    printAnalysis(tree, outFile);
    printBinaryTreeAnalysis(tree, outFile);
}