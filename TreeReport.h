#ifndef TREE_REPORT_H
#define TREE_REPORT_H

#include "LinkedTree.h"
#include "TreeAnalysis.h"
#include "BinaryTreeAnalysis.h"

#include <string>

/*  TreeReport.h contains the read-only interface for writing tree reports to a file.
    Will create a report about the tree that will contain:
    1. Tree Visualization = createTreeVisualization()
    2. Tree Analysis = printAnalysis()
    3. Binary Tree Analysis = printBinaryTreeAnalysis() 
*/
void writeTreeReport(const LinkedTree& tree, const std::string& filename);


#endif // TREE_REPORT_H