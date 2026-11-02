#ifndef TREE_PARSER_H
#define TREE_PARSER_H

#include <string>

class LinkedTree;

/**
 * Reads a decision-tree file and constructs a general linked tree.
 *
 * LinkedTree owns all nodes. addRoot/addChild must return the new node,
 * or throw; addChild also sets its parent pointer and appends it to the
 * parent's children in insertion order. clear() must release all nodes.
 */
class TreeParser {
public:
    /**
     * Loads one file into an initially empty, caller-owned tree.
     *
     * Root: 0 <position> <content>, or just <content> (position defaults to 1).
     * Other lines: <level> <position> <edge label> <content>.
     * Tab-separated records allow spaces inside the edge label. For the
     * space-separated variant, the edge label must be a single word.
     * Records after the root may appear in any order. Positions must be
     * positive, unique integers whose order is the preorder order.
     *
     * Returns true on success and clears errorMessage. Returns false with
     * an explanatory error on failure. A nonempty destination is rejected
     * without changing it. Other failures leave the destination empty.
     */
    static bool parseFile(const std::string& filename,
                          LinkedTree& tree,
                          std::string& errorMessage);
};

#endif
