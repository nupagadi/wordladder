#include <iostream>
#include <fstream>
#include <iterator>
#include <vector>
#include <algorithm>
#include <string>
#include <cassert>
#include <codecvt>


bool IsNeighbour(const std::string lh, const std::string rh)
{
   assert(lh.size() == rh.size());
   size_t cnt = 0;
   for(size_t i = 0; i < lh.size(); ++i)
      cnt += lh[i] != rh[i];
   return cnt == 1;
}

int main()
{
//    vec.reserve(2328);

   std::wifstream file("nounlist.txt");
//    std::wifstream file("word_rus.txt");
   file.imbue(std::locale(std::locale::empty(), new std::codecvt_utf8<wchar_t>));
   std::istream_iterator<std::wstring, wchar_t, std::char_traits<wchar_t> > input(file), eof;
//    std::vector<std::wstring> vec(input, eof);
   std::vector<std::wstring> vec_3L;
   std::copy_if(input, eof, std::back_inserter(vec_3L), [](const std::wstring& str){ return str.size() == 3; });
   //    assert(vec.size() == 34010);
//    assert(vec.size() == 34010);
//    assert(vec[1] == vec[2].substr(0, 4));

   assert(!IsNeighbour("qwe", "qwe"));
   assert(IsNeighbour("qwe", "qwq"));
   assert(!IsNeighbour("qwe", "qew"));

}