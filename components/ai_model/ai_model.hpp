#ifndef MY_XGB_CLASSIFICATION_TREE_HPP
#define MY_XGB_CLASSIFICATION_TREE_HPP

#include <fstream>
#include <iostream>
#include <iomanip>
#include <vector>
#include <cmath>
#include <stdexcept>
#include <string>

struct TreeNode {
    int feature_id;
    double split_point;
    double gain;
    TreeNode* left;
    TreeNode* right;

    TreeNode() : feature_id(0), split_point(0.0), gain(0.0), left(nullptr), right(nullptr) {}
};

// Function to load a tree from a binary file
TreeNode* loadTreeFromBinary(std::ifstream& bin_file);

// Function to load the model from a binary file
std::vector<TreeNode*> loadModelFromBinary(const std::string& filepath);

// Function to convert logits to probabilities
double logitsToProba(double x);

// Class for classification tree modeling
class MyXGBClassificationTree {
public:
    int max_depth;
    double reg_lambda;
    double prune_gamma;
    TreeNode* root;

    MyXGBClassificationTree(int md, double rl, double pg);

    double predictRecursively(TreeNode* node, const std::vector<double>& x) const;

    std::vector<double> predict(const std::vector<std::vector<double>>& x_test) const;
};

// Function to print the tree structure
void printTree(TreeNode* node, int depth = 0);

// Class to manage multiple classification trees
class MyXGBClassifier {
public:
    std::vector<MyXGBClassificationTree> models;
    double learning_rate;
    double base_score;

    MyXGBClassifier();

    void loadModels(const std::string& filepath);

    std::vector<double> predict(const std::vector<std::vector<double>>& x_test, bool proba = false);
};

#endif // MY_XGB_CLASSIFICATION_TREE_HPP
