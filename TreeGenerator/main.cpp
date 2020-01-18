#include <iostream>
#include <fstream>
#include <cstdlib>
#include <ctime>

size_t size;
std::ofstream file;

int main()
{
	bool exit = false;
	do
	{
		std::cout << "Enter the number of vertices: ";
		std::cin >> size;

		std::string filepath;
		std::cout << "Enter output file path: ";
		std::cin >> filepath;

		file.open(filepath);
		if (!file)
		{
			std::cerr << "Error opening the file";
			return 1;
		}
		file << size;

		if (size > 1)
		{
			srand((unsigned)time(0));

			for (size_t i = 2; i <= size; ++i)
			{
				file << std::endl << i << " " << (rand() % (i - 1)) + 1;
			}
		}

		file.close();

		short a;
		std::cout << "Tree generated. Generate another? ('1'=yes) ";
		std::cin >> a;
		if (a != 1) exit = true;
	} while (!exit);

	return 0;
}