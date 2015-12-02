#include <fstream>
#include <iterator>
#include <vector>
#include <queue>
#include <stack>
#include <algorithm>
#include <string>
#include <cassert>
#include <codecvt>


namespace wl
{

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
   distances.resize(neighbours.size(), -1);
   std::vector<bool> isChecked(neighbours.size());

   std::queue<size_t> queue;
   queue.push(start);
   distances[start] = 0;
   isChecked[start] = true;
   
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

            // if end is checked, we know the path
            if(isChecked[end])  return;

            // store to operate its neighbours later
            queue.push(neighbours[v][i]);
         }
      }
   }
}

void FindPath(std::stack<size_t>& links, const std::vector<size_t> &distances, const std::vector<std::vector<size_t>>& neighbours, size_t start, size_t end)
{
   assert(links.empty());
   
   size_t pos = end;
   while(distances[pos])
   {
      links.push(pos);
      for (size_t i = 0; i < neighbours[pos].size(); ++i)
      {
         if(distances[neighbours[pos][i]] == distances[pos]-1)
         {
            pos = neighbours[pos][i];
            break;
         }
      }
   }
   links.push(start);
}

}  // namespace wl