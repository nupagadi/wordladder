#pragma once

#include <vector>
#include <string>


namespace wl
{

// puts words from file fileName to vector dictionary;
// pass non empty fileName and wordLength > 1;
// file fileName should not contain duplicates
bool FillDictionary(std::vector<std::wstring>& dictionary, const std::string& fileName, size_t wordLength);

// checks if words differ by one letter only
// pass two strings of the same length
bool IsNeighbours(const std::wstring& lh, const std::wstring& rh);

// fills every word neighbour
void FillNeighbours(std::vector<std::vector<size_t>>& neighbours, const std::vector<std::wstring>& dictionary);

// calculates distances from word <start> to others;
// stops when distance to <end> is calculated
void CalcDistances(std::vector<size_t> &distances, const std::vector<std::vector<size_t>>& neighbours, size_t start, size_t end);

// find path from <start> word to <end>
void FindPath(std::vector<size_t>& path, const std::vector<size_t> &distances, const std::vector<std::vector<size_t>>& neighbours, size_t start, size_t end);

}
