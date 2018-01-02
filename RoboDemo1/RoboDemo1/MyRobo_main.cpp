//Anna 
//Bengt
//Christian
//Diana
#include <stdio.h>
#include <iostream>
#include <vector>
#include <sstream>
#include <set>
#include <stack>
#include <string>

//---------------------------
//data structures
typedef struct pointtag {
	int x;
	int y;
	pointtag(int i, int j) : x(i), y(j) {}
}pt;

//---------------------------
typedef struct pathTag {
	int pathid;
	char source;	//source home
	char destination; //destnation home
	int cost; //cost of path
			  //	std::vector<std::set<int, int>> gridlist;

			  //methods
	std::vector<pt> data;

}path;
//---------------------------
typedef struct hometag {
	int id;
	pt p;
	char legend;
	std::vector<path> map;
	hometag(pt pt, char leg) : p(pt.x, pt.y) {}

	//methos
	bool AddPath(path route);
}home;

inline bool hometag::AddPath(path route) {

	if (route.data.empty())
		return false;

	map.push_back(route);

	return true;

}

//---------------------------
typedef struct persontag {
	char legend;
	//std::string name; optional
	//stack of set to use for the path
	std::stack<pt>  curpath;

	//methods
	persontag(char leg) : legend(leg) {}

	std::vector<pt> FindPath( pt psrc, pt pdest,
								std::vector<std::vector<int>> map,
									int width, int height,
											std::vector<int> legend)
	{
		//(legend[4] = o,  legend[5] = ., legend[6] = #) !!!!



		do {
			
		} while (true);
	}
}person;
//---------------------------

int main(void)
{
	int ncases(0), nlines(0);
	int n(0);
	int m(0);
	int mloop(0);
	int k(0), l(0);
	int hid(0);
	int A(0), B(0), C(0), D(0), O(0), dot(0), ob(0);
	std::string strline("");
	std::vector<std::vector<int>> inmap;
	std::vector<std::vector<char>> outmap;
	std::vector<int> legends;
	//list of homes
	std::vector<home> homelst;

	//init legends list
	legends.push_back('A');
	legends.push_back('B');
	legends.push_back('C');
	legends.push_back('D');
	legends.push_back('o');
	legends.push_back('.');
	legends.push_back('#');

	//read the map from input
	while (getline(std::cin, strline))
	{
		//skip the blank line
		if (strline.empty()) continue;

		n = strline[0] - '0';
		m = strline[2] - '0';
		//lines of input rows of map to read
		k = m; l = n;

		//inits
		inmap.resize(m);
		outmap.resize(m);

		if (n == 0 && m == 0) break;

		for (int i = 0; i < m; i++)
		{
			//read another line of map
			getline(std::cin, strline);
			//skip the blank line
			if (strline.empty()) { --i; continue; }
			//init inmap matrix row with non-blank row
			for (int j = 0; j < n; j++)
			{
				inmap[i].push_back(strline[j]);
				outmap[i].push_back(strline[j]);
				//locate location of each of homes and store it in each home object
				if ((strline[j] == legends[0]) || (strline[j] == legends[1]) ||
					(strline[j] == legends[2]) || (strline[j] == legends[3])) {
					homelst.push_back(home(pt(j, i), strline[j]));
					homelst[hid].id = strline[j]; homelst[hid].legend = strline[j];
					hid++;
				}
			}
		}
		std::cout << std::endl;

		for (int i = 0; i < m; i++) {
			for (int j = 0; j < n; j++) {
				std::cout << (char)inmap[i][j];
			}
			std::cout << '\n' << std::endl;
		}
	}


	return 0;
}