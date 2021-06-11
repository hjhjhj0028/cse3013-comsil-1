#pragma once

#include "ofMain.h"
#include <string>
#include <vector>
#include <stack>
#include <queue>

struct mazeLine{
    float startX;
    float startY;
    float toX;
    float toY;
};

struct blockMove{
    bool up;
    bool down;
    bool left;
    bool right;
};

struct block {
    int x;
    int y;
    int count = 0;
};

class ofApp : public ofBaseApp{
    public:
        int draw_flag;
        int dfs_flag;
        int bfs_flag;
    
        stack<block> stack_solution_route;
        stack<block> stack_every_route;
        vector<mazeLine> maze_lines;
        vector<vector<char>> maze;
        vector<vector<bool>> visited_block;
        vector<vector<blockMove>> maze_block;
        
        int x_move[4]={0, 0, 1, -1};
        int y_move[4]={1, -1, 0, 0};
    
        int maze_height=0;
        int maze_width=0;
        int width;
        int height;
    
        void setup();
        void update();
        void draw();
        void keyPressed(int key);
        void keyReleased(int key);
        void mouseMoved(int x, int y );
        void mouseDragged(int x, int y, int button);
        void mousePressed(int x, int y, int button);
        void mouseReleased(int x, int y, int button);
        void mouseEntered(int x, int y);
        void mouseExited(int x, int y);
        void windowResized(int w, int h);
        void dragEvent(ofDragInfo dragInfo);
        void gotMessage(ofMessage msg);
        void processOpenFileSelection(ofFileDialogResult openFileResult);
        bool readFile();
        void MazeLine(mazeLine lines);
        bool DFS();
        void freeMemory();
};
