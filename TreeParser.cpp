#include "TreeParser.h"
#include "LinkedTree.h"

#include <algorithm>
#include <cctype>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <vector>

namespace {

// Temporary file data, kept separate from Person 1's tree representation.
struct NodeRecord {
    int level;
    int position;
    std::string edgeLabel;
    std::string content;
    std::size_t lineNumber;
};

// Removes outside whitespace while preserving spaces inside the text.
std::string trim(const std::string& text) {
    const std::string whitespace = " \t\r\n\f\v";
    const std::size_t first = text.find_first_not_of(whitespace);
    if (first == std::string::npos) {
        return "";
    }
    const std::size_t last = text.find_last_not_of(whitespace);
    return text.substr(first, last - first + 1);
}

// Checks the complete token; "2abc" must not be accepted as the number 2.
bool looksLikeInteger(const std::string& text) {
    if (text.empty()) {
        return false;
    }
    std::size_t index = (text[0] == '+' || text[0] == '-') ? 1 : 0;
    if (index == text.size()) {
        return false;
    }
    for (; index < text.size(); ++index) {
        if (!std::isdigit(static_cast<unsigned char>(text[index]))) {
            return false;
        }
    }
    return true;
}

// Converts a complete integer token and rejects values outside the int range.
bool parseInteger(const std::string& text, int& value) {
    if (!looksLikeInteger(text)) {
        return false;
    }
    try {
        value = std::stoi(text);
        return true;
    } catch (const std::invalid_argument&) {
        return false;
    } catch (const std::out_of_range&) {
        return false;
    }
}

// Adds the filename and original line number to an input error.
std::string inputError(const std::string& filename,
                       std::size_t lineNumber,
                       const std::string& reason) {
    return "File '" + filename + "', line " +
           std::to_string(lineNumber) + ": " + reason;
}

// Reads either the supplied numbered root or a content-only root.
bool parseRoot(const std::string& line,
               NodeRecord& record,
               std::string& reason) {
    std::istringstream fields(line);
    std::string first;
    std::string second;
    fields >> first;
    const bool hasSecond = static_cast<bool>(fields >> second);

    // A leading zero plus another token, or two integer tokens, identifies
    // a numbered root. Other first lines are treated as root content.
    const bool zeroPrefix = first == "0" || first == "+0" || first == "-0";
    const bool numberedRoot = hasSecond &&
        (zeroPrefix || (looksLikeInteger(first) && looksLikeInteger(second)));

    record.edgeLabel.clear();
    if (!numberedRoot) {
        record.level = 0;
        record.position = 1;
        record.content = line;
        return true;
    }

    if (!parseInteger(first, record.level) || record.level != 0) {
        reason = "The root level must be the integer 0.";
        return false;
    }
    if (!parseInteger(second, record.position) || record.position <= 0) {
        reason = "The root position must be a positive int.";
        return false;
    }
    std::getline(fields, record.content);
    record.content = trim(record.content);
    if (record.content.empty()) {
        reason = "The root content is missing.";
        return false;
    }
    return true;
}

// Reads a non-root record, preserving multiword content and tabbed labels.
bool parseNode(const std::string& line,
               NodeRecord& record,
               std::string& reason) {
    std::string levelText;
    std::string positionText;
    const std::size_t firstTab = line.find('\t');
    const std::size_t secondTab = firstTab == std::string::npos
        ? std::string::npos : line.find('\t', firstTab + 1);
    const std::size_t thirdTab = secondTab == std::string::npos
        ? std::string::npos : line.find('\t', secondTab + 1);

    if (thirdTab != std::string::npos) {
        // The first three tabs delimit the four fields in the assignment.
        levelText = trim(line.substr(0, firstTab));
        positionText = trim(line.substr(firstTab + 1, secondTab - firstTab - 1));
        record.edgeLabel = trim(line.substr(secondTab + 1, thirdTab - secondTab - 1));
        record.content = trim(line.substr(thirdTab + 1));
    } else {
        // The supplied sample files use spaces instead of tabs.
        std::istringstream fields(line);
        if (!(fields >> levelText >> positionText >> record.edgeLabel)) {
            reason = "Expected level, position, edge label, and node content.";
            return false;
        }
        std::getline(fields, record.content);
        record.content = trim(record.content);
    }

    if (!parseInteger(levelText, record.level) || record.level <= 0) {
        reason = "A non-root level must be a positive int; only the first node is the root.";
        return false;
    }
    if (!parseInteger(positionText, record.position) || record.position <= 0) {
        reason = "The preorder position must be a positive int.";
        return false;
    }
    if (record.edgeLabel.empty() || record.content.empty()) {
        reason = "Both the edge label and node content are required.";
        return false;
    }
    return true;
}

} // namespace

// Parses and validates all records before constructing any linked nodes.
bool TreeParser::parseFile(const std::string& filename,
                           LinkedTree& tree,
                           std::string& errorMessage) {
    errorMessage.clear();
    if (!tree.empty()) {
        errorMessage = "The destination tree must be empty. Use a separate tree for each file.";
        return false;
    }

    try {
        std::ifstream input(filename.c_str());
        if (!input.is_open()) {
            errorMessage = "Cannot open file '" + filename + "'. Check its path and read permission.";
            return false;
        }

        std::vector<NodeRecord> records;
        std::string line;
        std::size_t lineNumber = 0;

        while (std::getline(input, line)) {
            ++lineNumber;
            // Accept a UTF-8 BOM if a text editor added one to the file.
            if (lineNumber == 1 && line.compare(0, 3, "\xEF\xBB\xBF") == 0) {
                line.erase(0, 3);
            }
            line = trim(line);
            if (line.empty()) {
                continue;
            }

            NodeRecord record;
            record.lineNumber = lineNumber;
            std::string reason;
            const bool valid = records.empty()
                ? parseRoot(line, record, reason)
                : parseNode(line, record, reason);
            if (!valid) {
                errorMessage = inputError(filename, lineNumber, reason);
                return false;
            }
            records.push_back(record);
        }

        if (input.bad()) {
            errorMessage = "An I/O error occurred while reading file '" + filename + "'.";
            return false;
        }
        if (records.empty()) {
            errorMessage = "File '" + filename + "' is empty or contains only whitespace.";
            return false;
        }

        // Car-file rows are not in preorder, so sort by the stored position.
        std::sort(records.begin(), records.end(),
                  [](const NodeRecord& left, const NodeRecord& right) {
                      return left.position < right.position;
                  });

        if (records.front().level != 0) {
            errorMessage = "File '" + filename + "': the root must have the smallest preorder position.";
            return false;
        }

        // Validate depth changes before allocating any tree nodes.
        // pathSize is the number of nodes on the current root-to-node path.
        std::size_t pathSize = 1;
        std::size_t maximumLevel = 0;
        for (std::size_t index = 1; index < records.size(); ++index) {
            const NodeRecord& record = records[index];
            if (record.position == records[index - 1].position) {
                errorMessage = inputError(filename, record.lineNumber,
                    "Duplicate preorder position " + std::to_string(record.position) + ".");
                return false;
            }
            const std::size_t level = static_cast<std::size_t>(record.level);
            if (level > pathSize) {
                errorMessage = inputError(filename, record.lineNumber,
                    "Level " + std::to_string(record.level) +
                    " skips a parent level in preorder.");
                return false;
            }
            pathSize = level + 1;
            maximumLevel = std::max(maximumLevel, level);
        }

        // Keep the path from the root to the most recently constructed node.
        std::vector<TreeNode*> path;
        path.reserve(maximumLevel + 1);

        TreeNode* root = tree.addRoot(records.front().content,
                                      records.front().level,
                                      records.front().position);
        if (root == nullptr) {
            throw std::runtime_error("LinkedTree::addRoot returned a null pointer.");
        }
        path.push_back(root);

        for (std::size_t index = 1; index < records.size(); ++index) {
            const NodeRecord& record = records[index];
            const std::size_t level = static_cast<std::size_t>(record.level);

            // A node at level L has a parent at level L - 1.
            // Remove the previous sibling and any deeper finished subtree.
            path.resize(level);
            TreeNode* parent = path.back();
            TreeNode* node = tree.addChild(parent, record.edgeLabel, record.content,
                                           record.level, record.position);
            if (node == nullptr) {
                throw std::runtime_error("LinkedTree::addChild returned a null pointer.");
            }
            path.push_back(node);
        }
        return true;
    } catch (const std::exception& exception) {
        // The target was empty at entry; discard any partially built tree.
        tree.clear();
        errorMessage = "Cannot load file '" + filename + "': " + exception.what();
        return false;
    } catch (...) {
        tree.clear();
        errorMessage = "Cannot load file '" + filename + "': an unexpected error occurred.";
        return false;
    }
}

