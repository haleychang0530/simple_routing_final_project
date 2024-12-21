#include <vector>
#include <queue>
#include <iostream>
#include <string>
#include <iomanip>
#include<fstream>
#include <sstream>
#include <algorithm>
using namespace std;
//Directions(up, down, left, right)
const int rowDir[] = {-1, 1, 0, 0};
const int colDir[] = {0, 0, -1, 1};
void drawSVG(ofstream& svgFile,vector<vector<int>> grid);
//check if a cell is valid
bool isValid(int x, int y, int rows, int cols, const vector<vector<int>>& grid) {
    return (x >= 0 && x < rows && y >= 0 && y < cols && grid[x][y] == 0);
}

// Lee's Algorithm, ripple expansion, backtrack
bool leeAlgorithm(
    ofstream& output,
    vector<vector<int>>& grid,
    pair<int, int> src,
    pair<int, int> sink
) {
    int rows = grid.size();
    int cols = grid[0].size();
    int dist;
    //Queue for BFS
    queue<pair<int, int>> q;
   
    q.push(src);
    grid[src.first][src.second] = 0; 
    bool toSink=0;
    
    //Ripple expansion
    while (!q.empty()) {
        pair<int, int> current = q.front();
        q.pop();//take the first one and delete it
        
        int x = current.first;
        int y = current.second;
        
        // ripple
        for (int i = 0; i < 4; ++i) {
            int newX = x + rowDir[i];
            int newY = y + colDir[i];//加上方向 
            if (isValid(newX, newY, rows, cols, grid)) {
                grid[newX][newY] = grid[x][y] + 1; // distance
                q.push({newX, newY});//put into queue之後處理 
            }else if(newX==sink.first&&newY==sink.second){
            	toSink=1;
            	dist=grid[x][y]+1;//stop expansion
			}
        }
        
		 //到了stop expansion
        if(toSink){
        	break;
		}
    }

    // 到不了 
    if(!toSink){
        output << "No path exists!" << endl;
        return false;
		}
    
    //走回去 
    int x = sink.first;
    int y = sink.second;
    int shortcut=dist;
    int dire[2]={0};
    int newDire[2]={0};
    vector<vector<int>> shortestPath=grid; //紀錄最短路徑的網格 (方便之後做SVG)
	 
    int lastDire=0;
    int turnCount = 0;
    vector<int>bend_x;
    vector<int>bend_y;
    
    while (!(x == src.first && y == src.second)) {
    bool foundNext = false;
    // Check  four directions
    for (int i = 0; i < 4; ++i) {
        int newX = x + rowDir[(lastDire+i)%4];//優先選擇與上一次移動同方向，就可以減少轉折處 
        int newY = y + colDir[(lastDire+i)%4];
        newDire[0]=rowDir[(lastDire+i)%4];
        newDire[1]=colDir[(lastDire+i)%4];
        
        if (newX >= 0 && newX < rows && newY >= 0 && newY < cols ) {
        	int next=shortestPath[newX][newY];
        	if((newX==src.first&&newY==src.second ) ||next==dist-1){
        	dist=shortestPath[newX][newY];
            
            if((lastDire+i)%4!=lastDire){
            	
                if(x!=sink.first||y!=sink.second){
				shortestPath[x][y] = -5;
                bend_x.push_back(x);
                bend_y.push_back(y);
                turnCount++;
				}
                
			}else{
				shortestPath[x][y] = -4; 
			}
            lastDire=(lastDire+i)%4;
            x = newX;
            y = newY;
            foundNext = true;
            break;
			}  
            
        }
    }

    //no valid, exit 
    if (!foundNext) {
        break;
    }
}
shortestPath[src.first][src.second] = -2;
shortestPath[sink.first][sink.second] = -3;

//印出地圖比較方便debug~~
/*grid[src.first][src.second] = -2;
output<<"Before\n";
for (int i = 0; i < rows; ++i) {
    for (int j = 0; j < cols; ++j) {
    output<<setw(4)<<grid[i][j]<<" ";
    }   
    output<<endl;
}
output<<"After\n";
for (int i = 0; i < rows; ++i) {
    for (int j = 0; j < cols; ++j) {
    output<<setw(4)<<shortestPath[i][j]<<" ";
    }   
    output<<endl;
}
*/
    output<< "TotalLength "<<shortcut<<endl;
    output << "Source " << src.second <<" "<<-src.first-1+grid.size()<< endl;
    output << "Bend " << turnCount << endl;
        for(int i=0;i<turnCount;i++){
        	output<<bend_y[i]<<" "<<-bend_x[i]-1+grid.size()<<endl;
		}
    output << "Sink " << sink.second <<" "<<-sink.first-1+grid.size()<< endl;
    grid=shortestPath;
    return true;
}

int main() {
	ifstream input("input.txt");
    ofstream output("output.txt");
    ofstream graph("graph.svg");
    
    if (!input.is_open()) {
        cerr << "Failed to open input.txt" << endl;
        return -1;
    }
    if (!output.is_open()) {
        cerr << "Failed to open output.txt" << endl;
        return -1;
    }
    
    string command, action;
    pair<int, int> src, sink;
    int rows = 0, cols = 0, blockNum = 0;

    // Read input
    for (int i = 0; i < 4; ++i) {
        getline(input, command);
        stringstream ss(command);
        ss >> action;

        if (action == "Grid") {
            ss >> cols >> rows;  // grid x, y
        } else if (action == "Source") {
            int src_x, src_y;
            ss >> src_x >> src_y;
            src = {rows - 1 - src_y, src_x};  // y轉換 
        } else if (action == "Sink") {
            int sink_x, sink_y;
            ss >> sink_x >> sink_y;
            sink = {rows - 1 - sink_y, sink_x};  //y轉換 
        } else if (action == "Block") {
            ss >> blockNum;
        }
    }
   
    vector<vector<int>> grid(rows, vector<int>(cols, 0));
    vector<vector<int>> blockInfo(blockNum, vector<int>(4, 0));

    input.ignore();  //清掉buffer 
    for (int i = 0; i < blockNum; ++i) {
        string block;
        getline(input, block);
        stringstream blockStream(block);
        string blockName;
        int left_x, left_y, width, height;
        blockStream >> blockName >> left_x >> left_y >> width >> height;

        //Mark blocks 
        for (int j = 0; j < width; ++j) {
            for (int k = 0; k < height; ++k) {
                grid[rows - 1 - left_y - k][left_x + j] = -1;  
            }
        }
    }

    grid[src.first][src.second] = -2;  // Source
    grid[sink.first][sink.second] = -3;  // Sink
    
    
   leeAlgorithm(output,grid, src, sink); 
   drawSVG(graph,grid);  
   
   graph.close(); 
   output.close();
   input.close();
}
