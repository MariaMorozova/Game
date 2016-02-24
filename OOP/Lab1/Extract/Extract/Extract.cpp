// Extract.cpp: ���������� ����� ����� ��� ����������� ����������.
//

#include "stdafx.h"
#include <iostream>
#include <fstream>

//const  MAX_SIZE = 2147483648;

int main(int argc, char *argv[])
{
	//��� ������ ������� ������� ��������� ���������� �������� ����� � �������� (�������� �� ������)

	if (argc != 5)
	{
		std::cout << "�� ������ ���������� ����������. \n ������ ��������� ������: extract.exe <input file> <output file> <start position> <fragment size>";
		return 1;
	}
	else
	{
		std::ifstream fileIn(argv[1], std::ios::in | std::ios::binary);
		std::ofstream fileOut(argv[2], std::ios::out | std::ios::binary | std::ios::trunc);
		//trunc - ��������� ������� �� ����

		if (!fileIn.is_open())
		{
			std::cout << "���� �� ����� ���� ������!\n";
			return 1;
		}
		else
		{
			std::ifstream::pos_type size = 0;
			fileIn.seekg(0, std::ios::end);
			size = fileIn.tellg();

			int startPosition = std::atoi(argv[3]);
			fileIn.seekg(startPosition, std::ios::beg);

			int fragmentSize = std::atoi(argv[4]);

			while (!fileIn.eof() && fragmentSize)
			{
				fileOut.put(fileIn.get()); //������ �������� ����
				fragmentSize--;
			}
			fileIn.close();
			fileOut.close();
		}
	}

    return 0;
}