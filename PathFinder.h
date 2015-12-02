#include <vector>
#include <string>


namespace wl
{

class PathFinder
{
   // How to use:
public:
   // 1a. Create hollow object then Reset it (3)
   PathFinder() : _status(), _dictionary(), _start(-1), _end(-1) {}
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
   bool ResetPair(const std::wstring& startWord, const std::wstring& endWord);
   bool Reset(const std::wstring& startWord, const std::wstring& endWord, const std::vector<std::wstring>& dictionary);
   bool Reset(const std::wstring& startWord, const std::wstring& endWord, std::shared_ptr<std::vector<std::wstring>> dictionarySptr);

   // 4. Find path from <start> to <end>
   // returns true if there is a way
   bool FindPath();

   // 5a. Then display path or store it into file or stringstream
   void DisplayPath();
   // 5b. Or get it as a vector
   void GetPath(std::vector<size_t>& path);

private:
   bool _status; // MAKE ENUM (CLASS)
   size_t _start, _end;
   std::shared_ptr<std::vector<std::wstring>> _dictionary;
};

}  // namespace wl