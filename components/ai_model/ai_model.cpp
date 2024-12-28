
#include "ai_model.hpp"

  

// Function to load a tree from a binary file  
TreeNode* loadTreeFromBinary(std::ifstream& bin_file) {  
    // Read a flag to determine the node type  
    unsigned char flag;  
    if (!bin_file.read(reinterpret_cast<char*>(&flag), sizeof(flag))) {  
        return nullptr; // No more data, return nullptr  
    }  

    // 0: non-leaf node, 1: leaf node  
    if (flag == 1) {  
        // If it's a leaf node, read the leaf value  
        double leaf_value;  
        if (!bin_file.read(reinterpret_cast<char*>(&leaf_value), sizeof(leaf_value))) {  
            return nullptr; // If unable to read the leaf value  
        }  
        
        TreeNode* leaf_node = new TreeNode();  
        leaf_node->feature_id = 0; // We do not use feature_id for leaf nodes  
        leaf_node->split_point = leaf_value; // Use split_point to store leaf value  
        leaf_node->gain = 0; // Gain is meaningless for leaf nodes  
        return leaf_node;  
    } else {  
        // If it's a normal node, read additional information  
        TreeNode* node = new TreeNode();  
        
        if (!bin_file.read(reinterpret_cast<char*>(&node->feature_id), sizeof(node->feature_id))) return nullptr;  
        if (!bin_file.read(reinterpret_cast<char*>(&node->split_point), sizeof(node->split_point))) return nullptr;  
        if (!bin_file.read(reinterpret_cast<char*>(&node->gain), sizeof(node->gain))) return nullptr;  

        // Recursively read left and right child trees  
        node->left = loadTreeFromBinary(bin_file);  
        node->right = loadTreeFromBinary(bin_file);  

        return node;  
    }  
}  

// Function to load the model from a binary file  
std::vector<TreeNode*> loadModelFromBinary(const std::string& filepath) {  
    std::ifstream bin_file(filepath, std::ios::binary);  
    if (!bin_file.is_open()) {  
        throw std::runtime_error("Cannot open binary file.");  
    }  

    unsigned int num_trees;  
    if (!bin_file.read(reinterpret_cast<char*>(&num_trees), sizeof(num_trees))) {  
        throw std::runtime_error("Cannot read the number of trees.");  
    }  

    std::vector<TreeNode*> trees(num_trees);  
    for (unsigned int i = 0; i < num_trees; ++i) {  
        trees[i] = loadTreeFromBinary(bin_file);  
    }  

    bin_file.close();  
    return trees;  
}  

// Function to convert logits to probabilities  
double logitsToProba(double x) {  
    return 1.0 / (1.0 + std::exp(-x));  
}  

// Class for classification tree modeling  
class MyXGBClassificationTree {  
public:  
    int max_depth;  
    double reg_lambda;  
    double prune_gamma;  
    TreeNode* root;  

    MyXGBClassificationTree(int md, double rl, double pg)  
        : max_depth(md), reg_lambda(rl), prune_gamma(pg), root(nullptr) {}  

    double predictRecursively(TreeNode* node, const std::vector<double>& x) const {  
        if (node == nullptr) return 0.0;  

        if (node->left == nullptr && node->right == nullptr) {  
            return node->split_point;  
        }  

        if (x[node->feature_id] <= node->split_point) {  
            return predictRecursively(node->left, x);  
        } else {  
            return predictRecursively(node->right, x);  
        }  
    }  

    std::vector<double> predict(const std::vector<std::vector<double>>& x_test) const {  
        std::vector<double> predictions(x_test.size(), 0.0);  

        for (size_t i = 0; i < x_test.size(); ++i) {  
            predictions[i] = predictRecursively(root, x_test[i]);  
        }  

        return predictions;  
    }  
};  

// Function to print the tree structure  
void printTree(TreeNode* node, int depth = 0) {  
    if (node == nullptr) return;  

    // Print information about the node  
    if (node->left == nullptr && node->right == nullptr) {  
        std::cout << std::string(depth * 2, ' ') << "Leaf: " << node->split_point << std::endl;  
    } else {  
        std::cout << std::string(depth * 2, ' ') << "Node: feature_id=" << node->feature_id   
                  << ", split_point=" << std::setprecision(6) << node->split_point   
                  << ", gain=" << std::setprecision(6) << node->gain << std::endl;  
        printTree(node->left, depth + 1);  
        printTree(node->right, depth + 1);  
    }  
}  

// Class to manage multiple classification trees  
class MyXGBClassifier {  
public:  
    std::vector<MyXGBClassificationTree> models;  
    double learning_rate;  
    double base_score;  

    MyXGBClassifier() : learning_rate(0.3), base_score(0.5) {}  

    void loadModels(const std::string& filepath) {  
        // Load models from the file  
        std::vector<TreeNode*> tree_nodes = loadModelFromBinary(filepath);  

        // Initialize trees  
        for (auto&& tree_node : tree_nodes) {  
            models.emplace_back(MyXGBClassificationTree(0, 0.0, 0.0));  // Set dummy parameters for max_depth, reg_lambda, and prune_gamma  
            models.back().root = tree_node;  
        }  
        std::cout << "Finished loading " << models.size() << " models from trees." << std::endl;  
    }  

    std::vector<double> predict(const std::vector<std::vector<double>>& x_test, bool proba = false) {  
        std::vector<double> Fm(x_test.size(), base_score);  

        for (const auto& model : models) {  
            auto model_predictions = model.predict(x_test);  
            for (size_t i = 0; i < model_predictions.size(); ++i) {  
                Fm[i] += learning_rate * model_predictions[i];  
            }  
        }  

        std::vector<double> probabilities;  
        for (const auto& value : Fm) {  
            probabilities.push_back(logitsToProba(value)); 
 
        }  

        std::cout << "Predictions before probability conversion:\n";  
        for (const auto& value : Fm) {  
            std::cout << std::fixed << std::setprecision(4) << value << " ";  
        }  
        std::cout << std::endl;  

        std::cout << "Predicted probabilities:\n";  
        for (const auto& prob : probabilities) {  
            std::cout << std::fixed << std::setprecision(4) << ((prob > 0.5) ? 1 : 0) << " ";  
        }  
        std::cout << std::endl;  

        return probabilities;  
    }  
};  

// Main function  
int main() {  
    MyXGBClassifier classifier;  

    std::string model_file_path = "model_0.bst";  // Path to your binary file  
    try {  
        classifier.loadModels(model_file_path);  // Load models from the binary file  

        std::cout << "Successfully loaded models with tree structures:\n";  
        for (const auto& model : classifier.models) {  
            printTree(model.root);  // Print each tree structure from the classifier  
        }  
        
        std::vector<std::vector<double>> x_test = {  
            {21.23506965774815,70.6707597967895,20.530236045490057,831.6871648444841},  
            {78.19459112971967,60.912151814530944,39.69170019991617,746.310768603845}  
        };  

        auto predictions = classifier.predict(x_test);  

        std::cout << "Predictions from the model: ";  
        for (double pred : predictions) {  
            std::cout << pred << " ";  
        }  
        std::cout << std::endl;  
    } catch (const std::exception& e) {  
        std::cerr << "Error: " << e.what() << std::endl;  
    }  

    return 0;  
}
