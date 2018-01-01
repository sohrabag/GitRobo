//this file contains all the source code to call features
//created in classes or global functions or methods of classes

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

	std::vector<pt> FindPath(pt psrc, pt pdest,
		std::vector<std::vector<int>> map,
		int width, int height,
		std::vector<int> legend)
	{
		//(legend[4] = o,  legend[5] = ., legend[6] = #) !!!!
		std::vector<pt> path;
		std::stack<pt> stk_ok;
		std::stack<pt> stk_fill1;
		std::stack<pt> stk_fill2;
		std::deque<pt> dq_rem;
		bool down_dir(false), up_dir(false), forward_dir(false), backward_dir(false);
		int row1(0), col1(0), row2(0), col2(0);
		pt pnav(0, 0);
		bool bad(false);

		//check downward or upward move, store
		if (psrc.y < pdest.y) { row1 = psrc.y; col1 = psrc.x; down_dir = true; up_dir = false; }
		else { row1 = pdest.y; col1 = pdest.x; up_dir = true; down_dir = false; }
		//check forward or backward move
		//		if (psrc.y < pdest.y) { row1 = psrc.x; col1 = psrc.y; forward_dir = true; }
		//		else { row1 = pdest.x; col1 = pdest.y; backward_dir = true; }

		for (int i = 0; i < width; i++) {
			for (int j = 0; j < height; j++) {

				if (row1 == j && col1 == i) {

					do {
						//check downward or upward move, store or need to change the move direction!?
						if (psrc.y < pdest.y) { row1 = psrc.y; col1 = psrc.x; down_dir = true; }
						else { row1 = pdest.y; col1 = pdest.x; up_dir = true; }

						if (stk_fill1.size() == 0) {
							//push path of source home into the stack point by point
							for (int t = 0; t < width; t++) {
								stk_fill1.push(pt(t, j));
								stk_fill2.push(pt(t, j));
							}
						}
						//this path can be taken just need to be plowed
						//get top of the stack
						pnav = stk_fill1.top();
						stk_fill1.pop();
						row2 = pnav.y; col2 = pnav.x;

						if (map[row2][col2] == legend[6]) {// [#] we are on a obstacle we cannot go anywhere
							stk_fill1.pop(); //get rid of it go to the next element in the stack!
							continue;
						}

						if (pdest.y == row2) {
							//check all the grid squares on this row for a obstacle
							//on that row move forward toward the target home until you either hit a block
							//or reach to the home and that is your best path.
							//to do the above pop the rest of the stk_fill1 and queue it inside a deque and 
							//use the deque to move forward on that row!!!
							dq_rem.push_back(pnav);

							do {
								pnav = stk_fill1.top();
								stk_fill1.pop();
								dq_rem.push_back(pnav);
							} while (!stk_fill1.empty());
							//now we move on this row and check for a good path
							//row2 = pnav.y; col2 = pnav.x;

							do {
								pnav = dq_rem.front();
								//put row back in stk_fill1 stack if there is obstacle we erase the stk_fill1
								//and increment j by 1 to move down the map and then empty the stk_fill1
								stk_fill1.push(pnav);
								if (map[pnav.y][pnav.x] == legend[6]) {
									//stop and move down to the next row
									j++;//at this point stk_fill1 is empty
									pnav = stk_fill1.top();
									//you find another square on the map put it inside the good path
									stk_ok.push(pnav);
									//bad = true;
								}
							} while (!dq_rem.empty());
							continue;//go back to next loop for the next row to analyze
						}


						//look

						if (down_dir) // it is not [#]
						{
							row2 = pnav.y; col2 = pnav.x; row2++;
						}
						else { row2 = pnav.y; col2 = pnav.x; row2--; }
						//go down ward in map
						if (map[row2][col2] == legend[4]) //plenty of snow [o]
						{
							//good path we plow later
							stk_ok.push(pt(col2, row2));
							do {//empty the stack
								stk_fill1.pop();
							} while (!stk_fill1.empty());
							j++;//go downward in map
						}
						else if (map[row2][col2] == legend[5]) //no snow here [.]
						{
							//good take this path
							stk_ok.push(pt(col2, row2));
							do {//empty the stack
								stk_fill1.pop();
							} while (!stk_fill1.empty());
							; // we nyst break and continue on nex row of input---> down_dir move!
							j++;//go downward in map
						}
						else if (map[row2][col2] == legend[6]) //you hit the obstacle [#]
						{
							//dont go there choose another point
							continue;
						}
						else if (map[row2][col2] == legend[0]) //you can pass house [A]
						{
							//am i at destination
							if (pdest.x == col2 && pdest.y == row2) {
								//good take this path
								stk_ok.push(pnav);
								break;
							}
							else if (psrc.x == col2 && psrc.y == row2) {
								stk_ok.empty(); //the navigator looped around itself ?!
								continue;
							}
							continue;
						}
						else if (map[row2][col2] == legend[1]) //you can pass house [B]
						{
							//am i at destination
							if (pdest.x == col2 && pdest.y == row2) {
								//good take this path
								stk_ok.push(pnav);
								break;
							}
							else if (psrc.x == col2 && psrc.y == row2) {
								stk_ok.empty(); //the navigator looped around itself ?!
								continue;
							}
							continue;
						}
						else if (map[row2][col2] == legend[2]) //you can pass house [C]
						{
							//am i at destination
							if (pdest.x == col2 && pdest.y == row2) {
								//good take this path
								stk_ok.push(pnav);
								break;
							}
							else if (psrc.x == col2 && psrc.y == row2) {
								stk_ok.empty(); //the navigator looped around itself ?!
								continue;
							}
							continue;
						}
						else if (map[row2][col2] == legend[3]) //you can pass house [D]
						{
							//am i at destination
							if (pdest.x == col2 && pdest.y == row2) {
								//good take this path
								stk_ok.push(pnav);
								break;
							}
							else if (psrc.x == col2 && psrc.y == row2) {
								stk_ok.empty(); //the navigator looped around itself ?!
								continue;
							}
							continue;
						}
					} while (true);
				}
			}
		}
		return path;
	}
	//plow function
	pt plow(pt startpt);
}person;

pt persontag::plow(pt startpt) {
	pt endpt(0, 0);

	return endpt;
}

//-------------------------

int main(void)
{
	int ncases(0), nlines(0);
	int n(0);
	int m(0);
	int mloop(0);
	int k(0), l(0);
	int hid(0);
	//legend values
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

		hid = 0;
		//init 
		home HA(pt(0, 0), 'A');
		home HB(pt(0, 0), 'B');
		home HC(pt(0, 0), 'C');
		home HD(pt(0, 0), 'D');

		for (unsigned i = 0; i < homelst.size(); i++) {
			if (homelst[i].legend == 'A') {
				HA.p = homelst[i].p;
				HA.id = homelst[i].id;
			}
			else if (homelst[i].legend == 'B') {
				HB.p = homelst[i].p;
				HB.id = homelst[i].id;
			}
			else if (homelst[i].legend == 'C') {
				HC.p = homelst[i].p;
				HC.id = homelst[i].id;
			}
			else if (homelst[i].legend == 'C') {
				HD.p = homelst[i].p;
				HD.id = homelst[i].id;
			}
			else {}
		}

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

		do {


		} while (false);

		std::cout << '\n';
		//print n m
		std::cout << n << " " << m << std::endl << '\n';

		for (int i = 0; i < k; i++) {
			for (int j = 0; j < l; j++) {
				std::cout << (char)(inmap[i][j]);
			}
			std::cout << std::endl;
			std::cout << "\n";
		}
		//clean up and reset everythig
		m = 0; n = 0; inmap.clear(); outmap.clear(); strline.clear();

		std::cout << std::endl;
	}


	return 0;
}