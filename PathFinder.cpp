#include <fstream>
#include <codecvt>
#include <algorithm>

#include "WordLadder.h"
#include "PathFinder.h"


namespace wl
{

PathFinder::PathFinder(const char* pairFile, const char* dictionaryFile) : _status(), _dictionary(), _start(-1), _end(-1) 
{
   if(!Reset(pairFile, dictionaryFile))
      _status = false;
}

PathFinder::PathFinder(const std::wstring& startWord, const std::wstring& endWord, 
   const std::vector<std::wstring>& dictionary) : _status(), _dictionary(), _start(-1), _end(-1) 
{
   if(!Reset(startWord, endWord, dictionary))
      _status = false;
}

PathFinder::PathFinder(const std::wstring& startWord, const std::wstring& endWord, 
   std::shared_ptr<std::vector<std::wstring>> dictionarySptr) : _status(), _dictionary(), _start(-1), _end(-1) 
{
   if(!Reset(startWord, endWord, dictionarySptr))
      _status = false;
}


bool PathFinder::Reset()
{
   _status = 0;
   _dictionary.reset();
   _start = _end = -1;

   return true;
}

bool PathFinder::Reset(const char* pairFile, const char* dictionaryFile)
{
   Reset();

   std::wifstream pair(pairFile);
   if(!pair)  {
      return  _status = false;
   }
   // for unicode
   pair.imbue(std::locale(std::locale::empty(), new std::codecvt_utf8<wchar_t>));

   std::wstring start, end;
   pair >> start >> end;
   if(start.empty() || start.size() != end.size())  {
      return  _status = false;
   }

   _dictionary.reset(new std::vector<std::wstring>);
   if(!wl::FillDictionary(*_dictionary, dictionaryFile, start.size()))  {
      _dictionary.reset();
      return  _status = false;
   }

   if(!ResetPair(start, end))
      return  _status = false;

   return  _status = true;
}

template<class T1, class T2>
bool PathFinder::ResetPair(T1&& startWord, T2&& endWord)
{
   if(!_dictionary || startWord.size() != (*_dictionary)[0].size())
      return false;

   // seek for the first word in the _dictionary
   auto iter = std::find_if(_dictionary->cbegin(), _dictionary->cend(), 
      [&startWord, &endWord](const std::wstring& word)  { return  word == startWord || word == endWord; }
   );
   if(iter == _dictionary->cend())  {
      return  _status = false;
   }

   size_t pos = iter - _dictionary->cbegin();
   if(startWord == endWord)  {
      _start = _end = pos;
      return  _status = true;
   }

   // seek for the second word in the _dictionary
   const std::wstring* pSecondStr = nullptr;
   size_t* pSecondPos = nullptr;
   if(startWord == *iter)  {
      pSecondStr = &endWord;
      pSecondPos = &_end;
      _start = pos;
   }
   else  {
      pSecondStr = &startWord;
      pSecondPos = &_start;
      _end = pos;
   }

   iter = std::find(iter+1, _dictionary->cend(), *pSecondStr);
   if(iter == _dictionary->cend())  {
      return  _status = false;
   }

   *pSecondPos = iter - _dictionary->cbegin();
   return  _status = true;
}
// explicit template instantiation
template
   bool PathFinder::ResetPair<std::wstring, std::wstring>(std::wstring&& startWord, std::wstring&& endWord);

template<class T1, class T2>
bool PathFinder::Reset(T1&& startWord, T2&& endWord, std::shared_ptr<std::vector<std::wstring>> dictionarySptr)
{
   Reset();
   _dictionary = dictionarySptr;
   if(!_dictionary)
      return  _status = false;
   if(!ResetPair(std::forward<T1>(startWord), std::forward<T2>(endWord)))
      return  _status = false;
   return  _status = true;
}
// explicit template instantiation
template
   bool PathFinder::Reset<std::wstring, std::wstring>(std::wstring&& startWord, std::wstring&& endWord, 
      std::shared_ptr<std::vector<std::wstring>> dictionarySptr);

template<class T1, class T2, class T3>
bool PathFinder::Reset(T1&& startWord, T2&& endWord, T3&& dictionary)
{
   Reset();
   _dictionary.reset(new std::vector<std::wstring>);
   if(!_dictionary)
      return  _status = false;
   // vector is moved if we pass a right value reference to the method
   *_dictionary = dictionary;
   if(!ResetPair(std::forward<T1>(startWord), std::forward<T2>(endWord)))
      return  _status = false;
   return  _status = true;
}
// explicit template instantiation
template
   bool PathFinder::Reset<std::wstring, std::wstring, std::vector<std::wstring>>(
            std::wstring&& startWord, std::wstring&& endWord, std::vector<std::wstring>&& dictionary);

bool PathFinder::FindPath() const
{
   if(_start == _end)  {
      if(_path = std::make_shared<std::vector<size_t>>())
         return true;
      else  return false;
   }

   _neighbours.reset(new std::vector<std::vector<size_t>>());
   if(!_neighbours)      return false;
   _distances.reset(new std::vector<size_t>());
   if(!_distances)      return false;

   FillNeighbours(*_neighbours, *_dictionary);

   CalcDistances(*_distances, *_neighbours, _start, _end);

   _path = std::make_shared<std::vector<size_t>>();
   if(!_path)      return false;
   wl::FindPath(*_path, *_distances, *_neighbours, _start, _end);

   return true;
}

std::wostream& operator<<(std::wostream& stream, const PathFinder& obj)
{
   for(size_t i = 0; i < obj._path->size(); ++i)
      stream << (*obj._dictionary)[(*obj._path)[i]] << std::endl;

   return stream;
}

PathFinder PathFinder::Clone() const
{
   PathFinder obj;
   obj._status = _status;
   obj._start = _start;
   obj._end = _end;

   obj._dictionary = nullptr;
   obj._neighbours = nullptr;
   obj._distances = nullptr;
   obj._path = nullptr;
   if(_dictionary)      obj._dictionary = std::make_shared<std::vector<std::wstring>>(*_dictionary);
   if(_neighbours)      obj._neighbours = std::make_shared<std::vector<std::vector<size_t>>>(*_neighbours);
   if(_distances)       obj._distances = std::make_shared<std::vector<size_t>>(*_distances);
   if(_path)            obj._path = std::make_shared<std::vector<size_t>>(*_path);

   return std::move(obj);
}

}  // namespace wl