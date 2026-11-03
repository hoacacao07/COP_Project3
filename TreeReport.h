#ifndef TREE_REPORT_H
#define TREE_REPORT_H

#include "LinkedTree.h"
#include "TreeAnalysis.h"
#include "BinaryTreeAnalysis.h"

#include <ostream>
#include <string>

// Writes a report of the tree's properties to the specified file.
void writeTreeReport(const LinkedTree& tree, const std::string& filename);


#endif // TREE_REPORT_H