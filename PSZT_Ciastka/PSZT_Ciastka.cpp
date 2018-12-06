// PSZT_Ciastka.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include <iostream>
#include <ctime>
#include <cmath>
#include <map>
#include <vector>
#include <algorithm>

using namespace std;

bool contain(int index, vector<int> tab) {
	for (int j = 0; j < tab.size(); ++j) {
		if (tab[j] == index)
			return true;
	}
	return false;
}

int power(int a, int b) {
	if (b == 0) {
		return 1;
	}

	return a * power(a, b - 1);
}

bool* genPopulationMember(const int N, const int max) {
	bool* member;
	member = new bool[N * 4];

	for (int i = 0; i < N * 4; ++i) {
		member[i] = ((int)rand()) % 2;
	}
	
	return member;
}

void printFenotype(bool* member, int tab[], int N) {
	int firstGen;
	int secondGen;
	for (int i = 0; i < N; ++i) {
		int first = tab[i];

		firstGen = 0;
		for (int j = 0; j < 4; j++) {
			firstGen += power(2, 3 - j) * member[j + (i * 4)];
		}
		cout << firstGen << " ";

	}
	cout << endl;
}


int adaptationFunction(bool* member, int tab[], int N) {
	int memberMark = 0;
	int firstGen;
	int secondGen;
	for (int i = 0; i < N-1; ++i) {
		int first = tab[i];
		int second = tab[i + 1];

		firstGen = 0;
		secondGen = 0;
		for (int j = 0; j < 4 ; j++) {
			firstGen += power(2, 3-j) * member[j + (i*4)];
			secondGen += power(2, 3-j) * member[j + 4 + (i*4)];
		}

		if (first > second && firstGen <= secondGen)
			memberMark += 200;
		else if (first < second && firstGen >= secondGen)
			memberMark += 200;
		else if (firstGen == 0 || secondGen == 0)
			memberMark += 200;
		
		memberMark += firstGen;
	}

	memberMark += secondGen;
	
	return memberMark;
}

bool isCorrectPopulation(bool* member, int tab[], int N) {
	int firstGen;
	int secondGen;
	for (int i = 0; i < N - 1; ++i) {
		int first = tab[i];
		int second = tab[i + 1];

		firstGen = 0;
		secondGen = 0;
		for (int j = 0; j < 4; j++) {
			firstGen += power(2, 3 - j) * member[j + (i * 4)];
			secondGen += power(2, 3 - j) * member[j + 4 + (i * 4)];
		}

		if (first > second && firstGen <= secondGen)
			return false;
		else if (first < second && firstGen >= secondGen)
			return false;
		else if (firstGen == 0 || secondGen == 0)
			return false;
	}

	return true;
}

void reproduction(bool* firstMember, bool* secondMember, int N, vector<bool*> &reproducedMembers, int tab[]) {
	bool * reproducedMember1, * reproducedMember2 ;
	reproducedMember1 = reproducedMember2 = new bool[N*4];

	vector<int> indexTab;

	int numberOfIndexs = 0;
	while (true) {
		//int index = (((int)rand() % (N/4))*4);
		int index = ((int)rand() % N);
		int tmp = index % 4;
		index -= tmp;
		if (!contain(index, indexTab)) {
			indexTab.push_back(index);
			++numberOfIndexs;
		}
		if (numberOfIndexs == N/5)
			break;
	}

	sort(indexTab.begin(), indexTab.end());

	int iteration = 0;
	for (int i = 0; i < N*4; i++)
	{
		if (i < indexTab[iteration]) {
			if (iteration % 2 == 0) {
				reproducedMember1[i] == firstMember[i];
				reproducedMember2[i] == secondMember[i];
			}
			else {
				reproducedMember1[i] == secondMember[i];
				reproducedMember2[i] == firstMember[i];
			}
		}
		else
		{
			++iteration;
			--i;
		}
	}

	reproducedMembers.push_back(reproducedMember1);
	reproducedMembers.push_back(reproducedMember2);
}

void mutate(vector<bool*> &reproducedMembers, int N) {
	for (int k = 0; k < reproducedMembers.size(); ++k) {
		if (rand() % 10 == 7) {
			for (int i = 0; i < N * 4; i += 4) {
				if (rand() % 10 == 7) {
					for (int j = 0; j < 4; ++j) {
						reproducedMembers[k][i+j] = ((int)rand()) % 2;
					}
				}
			}
		}
	}
}

int main()
{
	time_t t;
	srand(time(&t));

	int tab[10] = { 1, 1, 2, 3, 4, 1, 6, 1, 1, 1};

	bool* population[1000];

	vector <pair<int, bool*>> memberWithMarkMap;

	for (int i = 0; i < 1000; ++i) {
		population[i] = genPopulationMember(10, 10);

		memberWithMarkMap.push_back(pair<int, bool*>(adaptationFunction(population[i], tab, 10), population[i]));
	}

	int bestResultForIteration = 10000;
	bool* theBestResult;
	int iteration = 0;

	while (true) {

		vector <bool*> reproducedMembers;
		for (int i = 0; i < 1000 - 1; ++i) {
			reproduction(memberWithMarkMap[i].second, memberWithMarkMap[i + 1].second, 10, reproducedMembers, tab);
		}

		mutate(reproducedMembers, 10);

		for (int i = 0; i < reproducedMembers.size(); ++i) {
			memberWithMarkMap.push_back(pair<int, bool*>(adaptationFunction(reproducedMembers[i], tab, 10), reproducedMembers[i]));
		}

		sort(memberWithMarkMap.begin(), memberWithMarkMap.end());
		memberWithMarkMap.resize(1000);

		for (int i = 0; i < memberWithMarkMap.size(); ++i) {
			if (isCorrectPopulation(memberWithMarkMap[i].second, tab, 10)) {
				int result = memberWithMarkMap[i].first;
				if (result < bestResultForIteration) {
					bestResultForIteration = result;
					iteration = 0;
					cout << bestResultForIteration << endl;
					i = memberWithMarkMap.size();
					break;
				}
				else if (result == bestResultForIteration) {
					++iteration;
					//cout << bestResultForIteration << endl;
					if (iteration == 100000) {
						cout << bestResultForIteration << endl;
						theBestResult = memberWithMarkMap[i].second;
						printFenotype(theBestResult, tab, 10);
						return 0;
					}
				}
			}
			//cout << " for: " << bestResultForIteration << endl;
		}


		
	}

}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
