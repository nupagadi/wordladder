#pragma once

#include <vector>
#include <string>

namespace wl
{

class PathFinder
{
   // How to use:
public:
   // 1a. Create hollow object then Reset it (3)
   PathFinder() : _status(), _dictionary(), _start(-1), _end(-1), _neighbours() {}
   // 1b. Or create a full-blown object
   PathFinder(const char* pairFile, const char* dictionaryFile);
   PathFinder(const std::wstring& startWord, const std::wstring& endWord, const std::vector<std::wstring>& dictionary);
   PathFinder(const std::wstring& startWord, const std::wstring& endWord, std::shared_ptr<std::vector<std::wstring>> dictionarySptr);

   // 2. Then check if it is ok
   operator bool() const { return  _status && IsDictionaryOk() && IsPairOk(); }
   bool IsDictionaryOk() const { return  _dictionary; }
   bool IsPairOk() const { return  _start != -1 && _end != -1; }

   // 3. You can reset it several ways
   // returns true if it is ok
   bool Reset();
   bool Reset(const char* pairFile, const char* dictionaryFile);
   // template for the sake of the perfect forwarding
   template<class T1, class T2, class T3>
      bool Reset(T1&& startWord, T2&& endWord, T3&& dictionary);
   template<class T1, class T2>
      bool Reset(T1&& startWord, T2&& endWord, std::shared_ptr<std::vector<std::wstring>> dictionarySptr);

   // 4. Find path from <start> to <end>
   // returns true if there is a way
   bool FindPath() const;

   // 3a. Can reset <start> and <end> word and find its path with the same dictionary
   template<class T1, class T2>
      bool ResetPair(T1&& startWord, T2&& endWord);

   // 4a. Then check if there is a path
   bool IsTherePath() const { return  _path && !_path->empty(); }

   // 5a. Get path as a vector of intermediate words and start and end words
   std::shared_ptr<const std::vector<size_t>> GetPath() const { return  _path; }
   // 5b. Or display it or store it into file or stringstream
   friend std::wostream& operator<<(std::wostream& stream, const PathFinder& obj);

   //6. You can use hard copy, if you don't want a light one (default copy ctor and operator=)
   PathFinder Clone() const;

private:
   bool _status; // MAKE ENUM (CLASS)
   size_t _start, _end;
   std::shared_ptr<std::vector<std::wstring>> _dictionary;

   mutable std::shared_ptr<std::vector<std::vector<size_t>>> _neighbours;
   mutable std::shared_ptr<std::vector<size_t>> _distances;
   mutable std::shared_ptr<std::vector<size_t>> _path;
};

std::wostream& operator<<(std::wostream& stream, const PathFinder& obj);

}  // namespace wl

