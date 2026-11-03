#include "TreeReport.h"
#include <fstream>
/*Open/create about_tree.txt from your code.
[ ] Print the root node.
[ ] Print internal and external node counts.
[ ] Print tree height.
[ ] Print preorder internal and external nodes.
[ ] Generate the tree visualization with two dashes per level and edge labels in brackets.
[ ] Print the binary-tree analysis.*/

void writeTreeReport(const LinkedTree& tree, const std::string& filename) {
    std::ofstream outFile(filename);
    if (!outFile.is_open()) {
        throw std::runtime_error("Unable to open file: " + filename);
    }

    if (!outFile) {
        throw std::runtime_error("Unable to open file: " + filename);
    }

    // print Tree analysis report by calling the printTreeAnalysis function
    printAnalysis(tree, outFile);

    // Generate the tree visualization with two dashes per level and edge labels in brackets.
    std::function<void(TreeNode*, int)> printTreeVisualization = [&](TreeNode* node, int level) {
        if (!node) return;
        outFile << std::string(level * 2, '-') << "[" << node->edgeLabel << "] " << node->content << "\n";
        for (TreeNode* child : node->children) {
            printTreeVisualization(child, level + 1);
        }
    };

    printTreeVisualization(tree.getRoot(), 0);  
        
    printBinaryTreeAnalysis(tree, outFile);

}