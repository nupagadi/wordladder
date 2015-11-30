#pragma once

#include <vector>
#include <string>
#include <stack>


namespace wl
{

// puts words from file fileName to vector dictionary;
// pass non empty fileName and wordLength > 1;
// file fileName should not contain duplicates
bool FillDictionary(std::vector<std::wstring>& dictionary, const std::string& fileName, size_t wordLength);

// checks if words differ by one letter only
bool IsNeighbours(const std::wstring& lh, const std::wstring& rh);

// fill every word neighbour
void FillNeighbours(std::vector<std::vector<size_t>>& neighbours, const std::vector<std::wstring>& dictionary);

// calculate distances from word <start> to others;
// stops when distance to <end> is calculated
void CalcDistances(std::vector<size_t> &distances, const std::vector<std::vector<size_t>>& neighbours, size_t start, size_t end);

void FindPath(std::stack<size_t>& links, const std::vector<size_t> &distances, const std::vector<std::vector<size_t>>& neighbours, size_t start, size_t end);
}
