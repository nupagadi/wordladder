#include <fstream>
#include <iterator>
#include <vector>
#include <queue>
#include <stack>
#include <algorithm>
#include <string>
#include <cassert>
#include <codecvt>



#include <iostream>



#include "WordLadder.h"


namespace wl
{

// pass two strings of the same lenght
bool IsNeighbours(const std::wstring& lh, const std::wstring& rh)
{
   assert(lh.size() == rh.size());
   size_t cnt = 0;
   for(size_t i = 0; i < lh.size(); ++i)
      cnt += lh[i] != rh[i];
   return cnt == 1;
}

bool FillDictionary(std::vector<std::wstring> &dictionary, const std::string& fileName, size_t wordLength)
{
   dictionary.clear();
   if(fileName.empty() || wordLength < 2)
      return false;
   std::wifstream file(fileName);
   if(!file)  return false;
   // for unicode
   file.imbue(std::locale(std::locale::empty(), new std::codecvt_utf8<wchar_t>));

   std::istream_iterator<std::wstring, wchar_t, std::char_traits<wchar_t> > input(file), eof;
   dictionary.reserve(10000);
   // get words of wordLength length only
   std::copy_if(input, eof, std::back_inserter(dictionary), [wordLength](const std::wstring& str){ return str.size() == wordLength; });
   dictionary.shrink_to_fit();
   return true;
}

void FillNeighbours(std::vector<std::vector<size_t>>& neighbours, const std::vector<std::wstring>& dictionary)
{
   neighbours.clear();
   neighbours.resize(dictionary.size());

   for(size_t i = 0; i < dictionary.size(); ++i)
   {
      for(size_t j = i; j < neighbours.size(); ++j)
      {
         if(IsNeighbours(dictionary[i], dictionary[j]))
         {
            neighbours[i].push_back(j);
            neighbours[j].push_back(i);
         }
      }
   }
}

void CalcDistances(std::vector<size_t> &distances, const std::vector<std::vector<size_t>>& neighbours, size_t start, size_t end)
{
   // CHECK START END!!!!!!!!!!

   distances.resize(neighbours.size(), -1);
   std::vector<bool> isChecked(neighbours.size());

   std::queue<size_t> queue;
   queue.push(start);
   distances[start] = 0;
   isChecked[start] = true;
   
   size_t checkedNum = 0;

   // till there are words to operate
   while (!queue.empty())
   {
      size_t v = queue.front();
      queue.pop();
      // operate all its neighbours
      for (size_t i = 0; i < neighbours[v].size(); ++i)
      {
         // if the distance is unknown
         if (!isChecked[neighbours[v][i]] )
         {
            // calculate it
            distances[neighbours[v][i]] = distances[v] + 1;
            // and mark as checked
            isChecked[neighbours[v][i]] = true;

            ++checkedNum;

            // if end is checked, we know the path
            if(isChecked[end])  return;

            // store to operate its neighbours later
            queue.push(neighbours[v][i]);
         }
      }
   }
}

void FindPath(std::vector<size_t>& path, const std::vector<size_t> &distances, const std::vector<std::vector<size_t>>& neighbours, size_t start, size_t end)
{
   path.clear();

   // check if there is a way from <start> to <end>
   if(distances[end] == -1)  return;

   path.resize(distances[end]+1);

   // find path from the end to the beginning
   size_t pos = end;
   while(distances[pos])
   {
      path[distances[pos]] = pos;
      // finding the neighbour...
      for (size_t i = 0; i < neighbours[pos].size(); ++i)
      {
         // ... which is closer by one step to the beginning
         if(distances[neighbours[pos][i]] == distances[pos]-1)
         {
            // than find among its neighbours
            pos = neighbours[pos][i];
            break;
         }
      }
   }
   path[distances[start]] = start;
}


PathFinder::PathFinder(const char* pairFile, const char* dictionaryFile) : _status(), _dictionary(), _start(-1), _end(-1) 
{
   std::wifstream pair(pairFile);
   if(!pair)  {
      _status = false;
      return;
   }
   // for unicode
   pair.imbue(std::locale(std::locale::empty(), new std::codecvt_utf8<wchar_t>));

   std::wstring start, end;
   pair >> start >> end;
   if(start.empty() || start.size() != end.size())  {
      _status = false;
      return;
   }

   _dictionary.reset(new std::vector<std::wstring>);
   if(!wl::FillDictionary(*_dictionary, dictionaryFile, start.size()))  {
      _status = false;
      _dictionary.reset();
      return;
   }

   // seek for the first word in the _dictionary
   auto iter = std::find_if(_dictionary->cbegin(), _dictionary->cend(), 
      [&start, &end](const std::wstring& word)  { return  word == start || word == end; }
   );
   if(iter == _dictionary->cend())  {
      _status = false;
      return;
   }

   size_t pos = iter - _dictionary->cbegin();
   if(start == end)  {
      _start = _end = pos;
      return;
   }

   // seek for the first word in the _dictionary
   const std::wstring* pSecondStr = nullptr;
   size_t* pSecondPos = nullptr;
   if(start == *iter)  {
      pSecondStr = &end;
      pSecondPos = &_end;
      _start = pos;
   }
   else  {
      pSecondStr = &start;
      pSecondPos = &_start;
      _end = pos;
   }

   iter = std::find(iter+1, _dictionary->cend(), *pSecondStr);
   if(iter == _dictionary->cend())  {
      _status = false;
      return;
   }

   *pSecondPos = iter - _dictionary->cbegin();
   _status = true;
}

PathFinder::PathFinder(const std::wstring& startWord, const std::wstring& endWord, 
   const std::vector<std::wstring>& dictionary) : _status(), _dictionary(), _start(-1), _end(-1) 
{}

PathFinder::PathFinder(const std::wstring& startWord, const std::wstring& endWord, 
   std::shared_ptr<const std::vector<std::wstring>> dictionarySptr) : _status(), _dictionary(), _start(-1), _end(-1) 
{}


}  // namespace wl