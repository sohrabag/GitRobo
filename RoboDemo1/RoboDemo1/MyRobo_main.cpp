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
	std::vector<pt> path;

	//methods
	persontag(char leg) : legend(leg), forward_dir(false), downward_dir(false), vert(false), x(0), y(0) {}

	//movement of the person methods
	
	//move forward
	inline void MoveForward() { y++; }
	//move backward
	inline void MoveBackward() { y--; }
	//move upward
	inline void MoveUp() { x--; }
	//move downward
	inline void MoveDown() { x++; }

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
				downward_dir = false; forward_dir = false;
			}
			else if (pt_dist.x > 0 && pt_dist.y < 0) {
				downward_dir = false; forward_dir = true;
			}
			else if (pt_dist.x < 0 && pt_dist.y > 0) {
				downward_dir = true; forward_dir = false;
			}
			else if (pt_dist.x < 0 && pt_dist.y < 0) {
				downward_dir = true; forward_dir = true;
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
		//
		//set the course of the move from source home to destination home
		SetCourse(psrc, pdest);

		//set the start point to source home
		x = psrc.x; y = psrc.y;

		//
		do {
			//check if x an y are in boundries of the map of sweden
			if (x < 0 && x >= height) forward_dir ? forward_dir = false : forward_dir = true;
			if (y < 0 && y >= width) downward_dir ? downward_dir = false : downward_dir = true;

			//check if we arrived to destination home
			if (Arrived(psrc, pdest)) break;

			//check which direction to move? (vertical or horizontal)
			if (!vert) 
			{
				//move horizontally in predefined direction

				std::cout << "Moving horizontally..." << std::endl;
				if (map[x][forward_dir ? (y + 1) : (y - 1)] == legend[0] ) {
					std::cout << "hit a home A..." << std::endl;
				}
				else if (map[x][forward_dir ? (y + 1) : (y - 1)] == legend[1]) {
					std::cout << "hit a home B..." << std::endl;
				}
				else if (map[x][forward_dir ? (y + 1) : (y - 1)] == legend[2]) {
					std::cout << "hit a home C..." << std::endl;
				}
				else if (map[x][forward_dir ? (y + 1) : (y - 1)] == legend[3]) {
					std::cout << "hit a home D..." << std::endl;
				}
				else if (map[x][forward_dir ? (y + 1) : (y - 1)] == legend[6]) { // we hit the tree square (#)
					
					//move horizontally in reverse direction
					forward_dir ? MoveForward() : MoveBackward(); //useless take cpu time
					//it is time to change to course in vertical motion
					vert ? vert = false : vert = true;
#ifdef _DEBUG
					std::cout << "#---tree obstacle " << '\n' << "x is: " << x << " y is: " << y << std::endl;
#endif			
					//move horizontally in reverse direction
					forward_dir ? MoveBackward() : MoveForward(); //useless take cpu time
					continue;

					//				if (map[--x][downward_dir ? ++y : --y] == legend[6])
				}
				else if (map[x][forward_dir ? (y + 1) : (y - 1)] == legend[4]) { //hit the snowy square (o)
																				 //add this to the path we want to plow snow
					//Move horizontally
					forward_dir ? MoveForward() : MoveBackward();
					//Add this to the path we want to plow snow (.)
					stk_ok.push(pt(x, y));
					//now we have to move vertically change vert to true
					vert ? vert = false : vert = true; 
#ifdef _DEBUG
					std::cout << "o--- snowy road " << '\n' << "x is: " << x << " y is: " << y << std::endl;
#endif
					continue;
				}
				else if (map[x][forward_dir ? (y + 1) : (y - 1)] == legend[5]) {//hit cleaned square
					//Move horizontally
					forward_dir ? MoveForward() : MoveBackward();
					//Add this to the path we want to plow snow (.)
					stk_ok.push(pt(x, y));
					//now we have to move vertically change vert to true
					vert ? vert = false : vert = true;

#ifdef _DEBUG
					std::cout << ". cleaned up road " << '\n' << "x is: " << x << " y is: " << y << std::endl;
#endif
					continue;
				}
			}
			else {//move vertically in predefined direction
				std::cout << "Moving vertically..." << std::endl;
				std::cout << "Moving horizontally..." << std::endl;
				if (map[x][forward_dir ? (y + 1) : (y - 1)] == legend[0]) {
					std::cout << "hit a home A..." << std::endl;
				}
				else if (map[x][forward_dir ? (y + 1) : (y - 1)] == legend[1]) {
					std::cout << "hit a home B..." << std::endl;
				}
				else if (map[x][forward_dir ? (y + 1) : (y - 1)] == legend[2]) {
					std::cout << "hit a home C..." << std::endl;
				}
				else if (map[x][forward_dir ? (y + 1) : (y - 1)] == legend[3]) {
					std::cout << "hit a home D..." << std::endl;
				}
				else if (map[downward_dir ? (x + 1) : (x - 1)][y] == legend[6]) {//we hit tree obstacle square
					//move vertically in reverse direction
					downward_dir ? MoveDown() : MoveUp();
					//dont need to change the course of movement vertically stay on this road until
					//you could go downward direction
					vert ? vert = false : vert = true;
#ifdef _DEBUG
					std::cout << "# obstacle" << '\n' << "x is: " << x << 
										" y is: " << y << std::endl;
#endif				
					//you hit an obstacle move in reverse direction
					downward_dir ? MoveUp() : MoveDown();
					continue;
				}
				else if (map[downward_dir ? (x + 1) : (x - 1)][y] == legend[4]) {//we hit snow coverd square
					downward_dir ? MoveDown() : MoveUp();
#ifdef _DEBUG
					std::cout << "o--- snow covered road" << '\n' << "x is: " << x << " y is: " << y << std::endl;
#endif
					stk_ok.push(pt(x, y));

					//time to disable vertical motion
					vert ? vert = false : vert = true;
					continue;
				}
				else if (map[downward_dir ? (x + 1) : (x - 1)][y] == legend[5]) {//cleaned up of snow square
					downward_dir ? MoveDown() : MoveUp();
#ifdef _DEBUG
					std::cout << ". cleaned up road " << '\n' << "x is: " << x << " y is: " << y << std::endl;
#endif
					stk_ok.push(pt(x, y));

					//time to disable vertical motion
					vert ? vert = false : vert = true;
					continue;

				}
			}

			//
		} while (true);

		return path;
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
					homelst.push_back(home(pt(i, j), strline[j]));
					homelst[hid].id = strline[j]; homelst[hid].legend = strline[j];
					hid++;
				}
			}
		}
		std::cout << std::endl;

		//init homes A, B, C, D
		home HA(pt(0, 0), 'A');
		home HB(pt(0, 0), 'B');
		home HC(pt(0, 0), 'C');
		home HD(pt(0, 0), 'D');

		//init location of homes(A, B, C, D) for HA, HB, HC, HD
		for (unsigned i = 0; i < homelst.size(); i++) {
			if (homelst[i].legend == 'A') {
				HA.p = homelst[i].p;
				HA.id = homelst[i].id;//N/A
			}
			else if (homelst[i].legend == 'B') {
				HB.p = homelst[i].p;
				HB.id = homelst[i].id;//N/A
			}
			else if (homelst[i].legend == 'C') {
				HC.p = homelst[i].p;
				HC.id = homelst[i].id;//N/A
			}
			else if (homelst[i].legend == 'C') {
				HD.p = homelst[i].p;
				HD.id = homelst[i].id;//N/A
			}
			else {}
		}

		//init persons A, B, C, D
		person B('B');
		person C('C');
		person D('D');
		person A('A');
		//-------------------------
		//analyze the map
		//		std::vector<pt> path1 = A.FindPath(HA.p, HB.p, inmap, n,m,legends);
		std::vector<pt> path2 = B.FindPath(HB.p, HC.p, inmap, n, m, legends);
		//		std::vector<pt> path3 = C.FindPath(HC.p, HD.p, inmap, n, m, legends);
		//		std::vector<pt> path4 = D.FindPath(HD.p, HA.p, inmap, n, m, legends);

		for (int i = 0; i < m; i++) {
			for (int j = 0; j < n; j++) {
				std::cout << (char)inmap[i][j];
			}
			std::cout << '\n' << std::endl;
		}
	}

	return 0;
}