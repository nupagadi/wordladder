#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <cassert>
#include <codecvt>

#include "WordLadder.h"


void FillDictionaryTest()
{
   bool isOk = false;
   std::vector<std::wstring> dictionary;

   isOk = wl::FillDictionary(dictionary, "", 3);
   assert(!isOk);

   isOk = wl::FillDictionary(dictionary, "nounlist.txt", 1);
   assert(!isOk);

   isOk = wl::FillDictionary(dictionary, "nounlist.txt", 3);
   assert(isOk);
   assert(dictionary.size() == 126);
   assert(dictionary[0] == L"act");
   assert(dictionary[50] == L"gas");
   assert(dictionary[125] == L"zoo");

   dictionary.clear();
   isOk = wl::FillDictionary(dictionary, "nounlist.txt", 2);
   assert(isOk);
   assert(dictionary.size() == 3);
   assert(dictionary[0] == L"cd");
   assert(dictionary[1] == L"ox");
   assert(dictionary[2] == L"tv");

   dictionary.clear();
   isOk = wl::FillDictionary(dictionary, "nounlist.txt", 10);
   assert(isOk);
   assert(dictionary.size() == 97);

   dictionary.clear();
   isOk = wl::FillDictionary(dictionary, "nounlist.txt", 15);
   assert(isOk);
   assert(dictionary.size() == 2);
   assert(dictionary[0] == L"anethesiologist");
   assert(dictionary[1] == L"ophthalmologist");

   dictionary.clear();
   isOk = wl::FillDictionary(dictionary, "nounlist.txt", 20);
   assert(isOk);
   assert(dictionary.size() == 0);

   std::cout << "FillDictionaryTest is OK!" << std::endl;
}

void IsNeighboursTest()
{
   assert(!wl::IsNeighbours(L"qwe", L"qwe"));
   assert(wl::IsNeighbours(L"qwe", L"qwq"));
   assert(!wl::IsNeighbours(L"qwe", L"qew"));

   std::cout << "IsNeighboursTest is OK!" << std::endl;
}

void FillNeighboursTest()
{
   bool isOk = false;
   std::vector<std::wstring> dictionary;

   isOk = wl::FillDictionary(dictionary, "word_rus.txt", 2);
   assert(isOk);
   assert(dictionary.size() == 14);

   std::vector<std::vector<size_t>> neighbours;
   wl::FillNeighbours(neighbours, dictionary);
   assert(neighbours.size() == dictionary.size());
   assert(neighbours[0][0] == 1);
   assert(neighbours[0][1] == 10);
   assert(neighbours[4][0] == 5);
   assert(neighbours[3][0] == 12);
   assert(neighbours[13][0] == 6);
   assert(neighbours[13][1] == 10);
   assert(neighbours[13][2] == 11);
   assert(neighbours[13][3] == 12);

   isOk = wl::FillDictionary(dictionary, "word_rus.txt", 3);
   assert(isOk);
   assert(dictionary.size() == 252);

   wl::FillNeighbours(neighbours, dictionary);
   assert(neighbours.size() == dictionary.size());
   assert(neighbours[0].size() == 2);
   assert(neighbours[0][0] == 4);
   assert(neighbours[0][1] == 83);
   assert(neighbours[30].size() == 8);
   assert(neighbours[30][0] == 28);
   assert(neighbours[30][1] == 29);
   assert(neighbours[30][2] == 31);
   assert(neighbours[30][3] == 46);
   assert(neighbours[30][4] == 87);
   assert(neighbours[30][5] == 163);
   assert(neighbours[30][6] == 192);
   assert(neighbours[30][7] == 212);

   assert(neighbours[1].size() == 0);

   std::cout << "FillNeighboursTest is OK!" << std::endl;
}

void CalcDistancesTest()
{
   bool isOk = false;
   std::vector<std::wstring> dictionary;

   isOk = wl::FillDictionary(dictionary, "word_rus.txt", 3);
   assert(isOk);

   std::vector<std::vector<size_t>> neighbours;
   wl::FillNeighbours(neighbours, dictionary);

   std::vector<size_t> distances;
   wl::CalcDistances(distances, neighbours, 2, 6);
   assert(distances.size() == dictionary.size());

   std::cout << "CalcDistancesTest is OK!" << std::endl;
}

void FindPathTest()
{
   bool isOk = false;
   std::vector<std::wstring> dictionary;

   isOk = wl::FillDictionary(dictionary, "word_rus.txt", 4);
   assert(isOk);

   std::cout << "dictionary.size() "<<dictionary.size() <<std::endl;

   std::vector<std::vector<size_t>> neighbours;
   wl::FillNeighbours(neighbours, dictionary);

   std::vector<size_t> distances;
   wl::CalcDistances(distances, neighbours, 430, 664);

   std::vector<size_t> path;
   wl::FindPath(path, distances, neighbours, 430, 664);

   //std::wofstream output("output.txt", std::fstream::app);
   //std::wcout.imbue(std::locale(std::locale::empty(), new std::codecvt_utf8<wchar_t>));
   std::wcout.imbue(std::locale(std::locale("Russian_Russia.866")));
   for(size_t i = 0; i < path.size(); ++i)
   {
      std::wcout << dictionary[path[i]] << std::endl;
   }


   std::cout << "FindPathTest is OK!" << std::endl;
}

void RunTests()
{
   FillDictionaryTest();
   IsNeighboursTest();
   //FillNeighboursTest();
   //CalcDistancesTest();
   FindPathTest();

   std::cout << "=================" << std::endl;
   std::cout << "All tests are OK!" << std::endl;
}