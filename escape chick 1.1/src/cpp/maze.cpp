#include "myheader.h"

merge_find::merge_find(int Size) {
    this->size = Size;
    set = new int[Size];
    value = new int[Size];
    init();
}

merge_find::~merge_find() {
    delete[] set;
    delete[] value;
}

void merge_find::init() {
    cnt = size;
    for (int i = 0; i < size; i++)
        set[i] = i, value[i] = 1;
}

int merge_find::path_find(int index) {
    return set[index] = set[index] == index ? index : path_find(set[index]);
}

bool merge_find::merge(int set1, int set2) {
    int i = path_find(set1), j = path_find(set2);
    if (i == j)
        return false;
    cnt--;
    if (value[i] < value[j]) {
        value[j] += value[i];
        set[i] = j;
    } else {
        value[i] += value[j];
        set[j] = i;
    }
    return true;
}

int merge_find::get_cnt() {
    return cnt;
}