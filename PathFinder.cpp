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

bool PathFinder::ResetPair(const std::wstring& startWord, const std::wstring& endWord)
{
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

bool PathFinder::Reset(const std::wstring& startWord, const std::wstring& endWord, std::shared_ptr<std::vector<std::wstring>> dictionarySptr)
{
   Reset();
   _dictionary = dictionarySptr;
   if(!_dictionary)
      return  _status = false;
   if(!ResetPair(startWord, endWord))
      return  _status = false;
   return  _status = true;
}
bool PathFinder::Reset(const std::wstring& startWord, const std::wstring& endWord, const std::vector<std::wstring>& dictionary)
{
   Reset();
   _dictionary.reset(new std::vector<std::wstring>(dictionary.cbegin(), dictionary.cend()));
   if(!_dictionary)
      return  _status = false;
   if(!ResetPair(startWord, endWord))
      return  _status = false;
   return  _status = true;
}


}  // namespace wl