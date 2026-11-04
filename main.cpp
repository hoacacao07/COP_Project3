
#include <exception>
#include <iostream>
#include <string>
 
#include "LinkedTree.h"
#include "TreeParser.h"
#include "TreeAnalysis.h"
#include "BinaryTreeAnalysis.h"
#include "TreeReport.h"
 
namespace {
 
/*
 findFirstSibling returns the first node that shares a parent with the given
 node, or nullptr if the node is the root or an only child.
 node is the node whose sibling is wanted
 */
const TreeNode* findFirstSibling(const TreeNode* node)
{
    if (node->parent == nullptr)
    {
        return nullptr;
    }
 
    for (const TreeNode* child : node->parent->children)
    {
        if (child != node)
        {
            return child;
        }
    }
    return nullptr;
}
 
/*
 displayNodeInformation prints the selected node's content, one ancestor
 (its parent), one descendant (its first child), and one sibling.
 node is the node chosen by the user, must not be nullptr
 */
void displayNodeInformation(const TreeNode* node)
{
    const TreeNode* sibling = findFirstSibling(node);
 
    std::cout << "Node's content: " << node->content << "\n";
 
    std::cout << "Ancestor: "
              << (node->parent != nullptr ? node->parent->content : "None") << "\n";
 
    std::cout << "Descendant: "
              << (!node->children.empty() ? node->children[0]->content : "None") << "\n";
 
    std::cout << "Sibling: "
              << (sibling != nullptr ? sibling->content : "None") << "\n";
}
 
/*
 chooseFilename decides which tree file to load.
 If a filename was given on the command line it is used. Otherwise the user
 picks one of the two sample files or types any other filename.
 Returns false if the user's choice could not be read or is invalid.
 */
bool chooseFilename(int argc, char* argv[], std::string& filename)
{
    if (argc > 1)
    {
        filename = argv[1];
        return true;
    }
 
    int choice = 0;
    std::cout << "\nChoose a tree to load:\n";
    std::cout << "1. Car Decision Tree (tree-car.txt)\n";
    std::cout << "2. Investment Decision Tree (tree-investment.txt)\n";
    std::cout << "3. Enter another filename\n";
    std::cout << "Enter your choice: ";
 
    if (!(std::cin >> choice))
    {
        return false;
    }
 
    if (choice == 1)
    {
        filename = "tree-car.txt";
    }
    else if (choice == 2)
    {
        filename = "tree-investment.txt";
    }
    else if (choice == 3)
    {
        std::cout << "Filename: ";
        if (!(std::cin >> filename))
        {
            return false;
        }
    }
    else
    {
        return false;
    }
    return true;
}
 
} // namespace
 
int main(int argc, char* argv[])
{
    std::cout << "=====================================\n";
    std::cout << "       DECISION TREE EXPLORER\n";
    std::cout << "=====================================\n";
 
    std::string filename;
    if (!chooseFilename(argc, argv, filename))
    {
        std::cout << "Invalid choice. Exiting program.\n";
        return 1;
    }
 
    // Build the tree from the selected file.
    LinkedTree tree;
    std::string errorMessage;
 
    if (!TreeParser::parseFile(filename, tree, errorMessage))
    {
        std::cout << "\nError loading tree: " << errorMessage << "\n";
        return 1;
    }
 
    std::cout << "\nTree loaded successfully!\n";
    std::cout << "Total number of nodes: " << tree.size() << "\n";
 
    // Write the report file; this throws if the file cannot be created.
    try
    {
        writeTreeReport(tree, "about_tree.txt");
        std::cout << "\nTree report created: about_tree.txt\n";
    }
    catch (const std::exception& exception)
    {
        std::cout << "\n" << exception.what() << "\n";
        return 1;
    }
 
    // Print the general and binary tree analysis to the console.
    printAnalysis(tree, std::cout);
    printBinaryTreeAnalysis(tree, std::cout);
 
    // Interactive node exploration.
    std::string userInput;
    std::cout << "\n";
 
    while (true)
    {
        std::cout << "\nWhich node would you like to explore (enter position or \"exit\"): ";
 
        if (!(std::cin >> userInput))  // end of input: stop instead of looping forever
        {
            break;
        }
 
        if (userInput == "exit" || userInput == "EXIT")
        {
            std::cout << "Goodbye!\n";
            break;
        }
 
        try
        {
            std::size_t charactersRead = 0;
            int position = std::stoi(userInput, &charactersRead);
            const TreeNode* selectedNode = tree.findNode(position);
 
            // The whole input must be a number, and the node must exist.
            if (charactersRead != userInput.length() || selectedNode == nullptr)
            {
                std::cout << "Invalid input. Please try again.\n";
            }
            else
            {
                displayNodeInformation(selectedNode);
            }
        }
        catch (...)  // stoi throws for non-numbers and out-of-range values
        {
            std::cout << "Invalid input. Please try again.\n";
        }
    }
 
    return 0;
}