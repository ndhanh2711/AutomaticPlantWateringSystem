#ifndef MY_XGB_H
#define MY_XGB_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

// Định nghĩa Node
typedef struct {
    float threshold;    // Ngưỡng để phân loại
    uint32_t left_idx;  // Chỉ số con trái
    uint32_t right_idx; // Chỉ số con phải
    int feature_idx;    // Chỉ số đặc trưng để so sánh
    float leaf_value;   // Giá trị tại lá (nếu là nút lá)
    bool is_leaf;       // Cờ kiểm tra nút là lá
} Node;

// Định nghĩa Tree
typedef struct {
    Node* nodes;        // Danh sách các nút trong cây
    uint32_t node_count; // Số lượng nút trong cây
} Tree;

// Định nghĩa Classifier
typedef struct {
    Tree* trees;        // Danh sách các cây trong bộ phân loại
    uint32_t tree_count; // Số lượng cây
    uint32_t feature_count; // Số lượng đặc trưng
} Classifier;

// Hàm để đọc một bộ phân loại từ tệp nhị phân
Classifier* classifierFromBinaryFile(const char* filename);

// Hàm để giải phóng bộ nhớ của một bộ phân loại
void classifierFree(Classifier* clf);

// Hàm để thực hiện dự đoán với bộ phân loại
float classifierPredict(Classifier* clf, float* features);

#endif // MY_XGB_H
