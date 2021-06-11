#include "ofApp.h"
#include "ofPolyline.h"

using namespace std;

// ofPolyline 사용
ofPolyline solution_route;
ofPolyline every_route;

//--------------------------------------------------------------
void ofApp::setup()
{
    ofSetWindowTitle("Maze Example");
    ofSetLineWidth(10);
    ofSetBackgroundColor(255, 255, 255);
    this->draw_flag = 0;
}
//--------------------------------------------------------------
void ofApp::update()
{
    
}
//--------------------------------------------------------------
void ofApp::draw()
{
    if (this->draw_flag == 1)
    {
        if (this->dfs_flag == 1)
        {
            //dfs를 통한 모든 탐색 경로는 파란색으로 표시
            ofSetColor(ofColor::blue);
            int search_num = stack_every_route.size();
            for (int i = 0; i < search_num; i++)
            {
                every_route.addVertex(20 * stack_every_route.top().x + 10, 20 * stack_every_route.top().y + 10);
                stack_every_route.pop();
            }
            every_route.draw();
            
            // dfs를 통한 가장 짧은 탐색 경로는 빨간색으로 표시
            ofSetColor(ofColor::red);
            int search_block_num = stack_solution_route.size();
            for (int i = 0; i < search_block_num; i++)
            {
                solution_route.addVertex(20 * stack_solution_route.top().x + 10, 20 * stack_solution_route.top().y + 10);
                stack_solution_route.pop();
            }
            solution_route.draw();
        }
        for (int i = 0; i < this->maze_lines.size(); i++) MazeLine(this->maze_lines[i]);
    }
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key)
{
    if (key == 'r')
    {
        cout << "Read key Pressed" << endl;
        readFile();
        this->draw_flag = 1;
    }
    if (key == 'v')
    {
        // HACK: only needed on windows, when using ofSetAutoBackground(false)
        glReadBuffer(GL_FRONT);
        ofSaveScreen("savedScreenshot_" + ofGetTimestampString() + ".png");
    }
    // dfs key
    if (key == 'd')
    {
        if (this->draw_flag == 1)
        {
            this->dfs_flag = 1;
            cout << "DFS key Pressed" << endl;
            int search_num = stack_solution_route.size();
            if (DFS())
                solution_route.draw();
        }
    }
    if (key == 'q')
    {
        cout << "Quit" << endl;
        freeMemory();
        cout << "Dynamically allocated memory has been freed." << endl;
        OF_EXIT_APP(0);
    }
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key)
{
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y)
{
}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button)
{
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button)
{
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button)
{
}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y)
{
}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y)
{
}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h)
{
}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg)
{
}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo)
{
}

//--------------------------------------------------------------
// 파일 읽기 함수
// 파일로부터 입력 자료를 읽어 들여 자료구조에 내용을 저장하는 함수
bool ofApp::readFile()
{
    freeMemory();
    maze_height = 0;
    maze_width = 0;
    ofFileDialogResult openFileResult = ofSystemLoadDialog("Open .maz file");
    // 이전 실습에서 생성한 3x5 사이즈 미로를 10 x 10  사이즈로 재생성하여 사용함
    if (openFileResult.bSuccess)
    {
        string line;
        //out.maz 파일 열기
        string file_location = openFileResult.getPath();
        ifstream input_file, input_file2;
        input_file.open(file_location);
        input_file2.open(file_location);

        if (!input_file)
            cout << ".maz file does not exists." << endl;
        else
            cout << "We found the .maz file." << endl;

        if (input_file.is_open())
        {
            while (getline(input_file, line))
            {
                maze_width = line.length();
                width = (line.length() - 1) / 2;
                maze_height++;
            }
            height = (maze_height - 1) / 2;
            //불러온 미로의 크기는 10*10
            maze.resize(maze_height, vector<char>(maze_width));

            string tmp_arr;
            for (int i = 0; i < maze_height; i++)
            {
                getline(input_file2, tmp_arr);
                for (int j = 0; j < maze_width; j++)
                    maze[i][j] = tmp_arr[j];
            }
            mazeLine tmp_lines;

            for (int i = 0; i < maze_height; i++)
            {
                for (int j = 0; j < maze_width; j++)
                {
                    if (maze[i][j] == '-')
                    {
                        tmp_lines.startX = j - 0.5;
                        tmp_lines.startY = i + 0.5;
                        tmp_lines.toX = j + 1.5;
                        tmp_lines.toY = i + 0.5;
                        this->maze_lines.push_back(tmp_lines);
                    }
                    else if (maze[i][j] == '|')
                    {
                        tmp_lines.startX = j + 0.5;
                        tmp_lines.startY = i - 0.5;
                        tmp_lines.toX = j + 0.5;
                        tmp_lines.toY = i + 1.5;
                        this->maze_lines.push_back(tmp_lines);
                    }
                }
            }
            // 파일 읽기가 성공하면 true 반환, 실패하면 false 반환
            return true;
        }
        else
        {
            cout << "Failed to open .maz file" << endl;
            this->draw_flag = 0;
            return false;
        }
    }
}

//--------------------------------------------------------------
void ofApp::MazeLine(mazeLine lines)
{
    ofSetColor(ofColor::black);
    ofSetLineWidth(5);
    ofDrawLine(20 * lines.startX, 20 * lines.startY, 20 * lines.toX, 20 * lines.toY);
}

//--------------------------------------------------------------
//dfs(depth first search) 한 vertex에서 자식 node를 우선으로
//탐색하는 graph의 node를 탐색하는 기법 중 하나
bool ofApp::DFS()
{
    this->draw_flag = 1;
    this->dfs_flag = 1;
    visited_block.resize(maze_height, vector<bool>(maze_width));
    maze_block.resize(maze_height, vector<blockMove>(maze_width));

    for (int i = 0; i < maze_height; i++)
    {
        for (int j = 0; j < maze_width; j++)
        {
            if (maze[i][j] != ' ')
                visited_block[i][j] = true;
            else
                visited_block[i][j] = false;
        }
    }
    block block1;
    block1.x = 1;
    block1.y = 1;
    visited_block[1][1] = true;
    bool moving_or_not; //이동 가능한지 아닌지 여부

    stack_solution_route.push(block1);
    stack_every_route.push(block1);

    while (!stack_solution_route.empty())
    {
        stack_every_route.push(stack_solution_route.top());
        if (stack_solution_route.top().x == maze_width - 2 && stack_solution_route.top().y == maze_height - 2)
            return true;
        moving_or_not = false;
        block block2;
        
        for (int i = 0; i < 4; i++)
        {
            block2.y = stack_solution_route.top().y + y_move[i];
            block2.x = stack_solution_route.top().x + x_move[i];
            if (!visited_block[block2.y][block2.x])
            {
                stack_every_route.push(block2);
                visited_block[stack_solution_route.top().y + y_move[i]][stack_solution_route.top().x + x_move[i]] = true;
                stack_solution_route.push(block2);
                moving_or_not = true;
                break;
            }
        }
        if (!moving_or_not)
        {
            stack_solution_route.pop();
        }
    }
}

//--------------------------------------------------------------
// readfile이나 그 후 다른 함수에서 할당한 메모리 해제하기
void ofApp::freeMemory()
{
    this->maze.clear();
    this->maze_lines.clear();
    this->visited_block.clear();
    this->maze_block.clear();

    vector<vector<char>>().swap(this->maze);
    vector<mazeLine>().swap(this->maze_lines);
    vector<vector<bool>>().swap(this->visited_block);
    vector<vector<blockMove>>().swap(this->maze_block);

    while (!stack_solution_route.empty()) stack_solution_route.pop();
    while (!stack_every_route.empty()) stack_every_route.pop();

    this->draw_flag = 0;
    this->dfs_flag = 0;

    solution_route.clear();
    every_route.clear();
}
