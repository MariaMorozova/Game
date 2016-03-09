// ArrayOfNumbers.cpp: ���������� ����� ����� ��� ����������� ����������.
// ������ ������� ������� ������ ���� ������� �� ����������� ������� ��������� �������

#include "stdafx.h"
#include <iostream>
#include <fstream>
#include <numeric>
#include <functional>
#include "ProcessVector.h"

int main()
{
	std::ifstream fileIn("Input.txt");
	std::ofstream fileOut("Output.txt");

	if (!fileIn.is_open())
	{
		std::cout << "���� �� ����� ���� ������\n";
		return 1;
	}

	std::vector<double> vectOfNumbers;
	float number;

	while (!fileIn.eof())
	{
		fileIn >> number;
		vectOfNumbers.push_back(number);
	}
	fileIn.close();

	ProcessVector(vectOfNumbers);

	for (auto i = vectOfNumbers.begin(); i != vectOfNumbers.end(); i++)
		fileOut << *i << " ";

	fileOut.close();
	vectOfNumbers.clear();

	return 0;
}


/*
void ProcessVector1(std::vector<double> & numbers)
{
	double minElement = *std::min_element(numbers.begin(), numbers.end());

	for (auto i = numbers.begin(); i != numbers.end(); i++)
		*i *= minElement;
}
*/