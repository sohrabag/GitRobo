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

	//flags to determine direction of movement horizontal or vertical
	bool downward_dir;
	bool forward_dir;
	bool vert;

	//x and y coordinates of the square on the grid map of sweden
	int  x; 
	int  y;
	
	//stack to push the path of snow to plow
	std::stack<pt> stk_ok;

	//methods
	persontag(char leg) : legend(leg), forward_dir(false), downward_dir(false), vert(false), x(0), y(0) {}

	//movement of the person methods
	
	//move forward
	inline void MoveForward() { x++; }
	//move backward
	inline void MoveBackward() { x--; }
	//move upward
	inline void MoveUp() { y--; }
	//move downward
	inline void MoveDown() { y++; }

	//check if we arrived from source home to destination home.
inline bool Arrived(pt psrc, pt pdest) {
		return(psrc.x == pdest.x && psrc.y == pdest.y);
	}

//SetCourse will tell person object which direction to choose so that it is shortest, fastest, 
//and very efficient
inline void SetCourse(pt psrc, pt pdest) {

		try {
			//calculate distance between two homes and place it in pt_dist
			pt pt_dist(0, 0);
			pt_dist.x = psrc.x - pdest.x;
			pt_dist.y = psrc.y - pdest.y;

			//determine course of movement on the sweden map
			if (pt_dist.x > 0 && pt_dist.y > 0) {
				forward_dir = false; downward_dir = false;
			}
			else if (pt_dist.x > 0 && pt_dist.y > 0) {
				forward_dir = false; downward_dir = true;
			}
			else if (pt_dist.x < 0 && pt_dist.y > 0) {
				forward_dir = true; downward_dir = false;
			}
			else if (pt_dist.x < 0 && pt_dist.y < 0) {
				forward_dir = true; downward_dir = true;
			}
			else throw;

		}
		catch (const std::exception&)
		{
			std::cout << "an exception happened" << std::endl;
		}
	}

//FindPath finds the path to reach from source to destination home
	std::vector<pt> FindPath( pt psrc, pt pdest,
								std::vector<std::vector<int>> map,
									int width, int height,
											std::vector<int> legend)
	{
		//(legend[4] = o,  legend[5] = ., legend[6] = #) !!!!

		//set the course of the move from source home to destination home
		SetCourse(psrc, pdest);

		//set the start point to source home
		x = psrc.x; y = psrc.y;

		//
		do {
			//check if we arrived to destination home
			if (Arrived(psrc, pdest)) break;

			//check which direction to move? (vertical or horizontal)
			if (!vert) 
			{
				//move horizontally in predefined direction
				if (map[forward_dir ? (x + 1) : (x - 1)][y] == legend[6]) { // we hit the tree square (#)
					
					//move horizontally in reverse direction
					MoveBackward();
					forward_dir ^= forward_dir; continue;
					//				if (map[--x][downward_dir ? ++y : --y] == legend[6])
				}
				else if (map[forward_dir ? (x + 1) : (x - 1)][y] == legend[4]) { //hit the snowy square (o)
																				 //add this to the path we want to plow snow
#ifdef _DEBUG
					std::cout << "x is: " << x << " y is: " << y << std::endl;
#endif
					stk_ok.push(pt(x, y)); continue;
					//now we have to move vertically change vert to true
					vert ^= vert;
				}
				else if (map[forward_dir ? (x + 1) : (x - 1)][y] == legend[5]) {
					//Add this to the path we want to plow snow (.)
					stk_ok.push(pt(x, y)); continue;
					//now we have to move vertically change vert to true
					vert ^= vert;

#ifdef _DEBUG
					std::cout << "x is: " << x << " y is: " << y << std::endl;
#endif
				}
			}
			else {//move vertically in predefined direction

			}


			//
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