#include <iostream>

#include "PathFinder.h"


#ifdef _DEBUG

void RunTests();

int main()
{
   RunTests();
}

#else

int main(int argc,  char** argv)
{
   if (argc != 3) {
      printf("Usage: %s \"<PAIR FILE>\" \"<DICTIONARY FILE>\"\n", argv[0]);
      return EXIT_FAILURE;
   }

   wl::PathFinder pf(argv[1], argv[2]);
   if(!pf)  {
      std::cout << "Fail!" << std::endl;
      return EXIT_FAILURE;
   }

   if(!pf.FindPath())  {
      std::cout << "Fail!" << std::endl;
      return EXIT_FAILURE;
   }

   if(pf.IsTherePath())  {
      std::wcout.imbue(std::locale("Russian_Russia.866"));
      std::wcout << pf;
   }  else
      std::cout << "No way!" << std::endl;

   return EXIT_SUCCESS;
}

#endif