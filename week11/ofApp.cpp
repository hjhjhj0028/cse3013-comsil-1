#include "ofApp.h"
#include <stdio.h>
#include <fstream>
#include <stdlib.h>

int water_drop_count = 0; //물방울 개수

//--------------------------------------------------------------
void ofApp::setup()
{
    // Limit the speed of our program to 15 frames per second
    ofSetFrameRate(15);
    // We still want to draw on a black background, so we need to draw
    // the background before we do anything with the brush
    ofSetLineWidth(5);
    ofSetBackgroundColor(255, 255, 255);
    this->draw_flag = 0;
}

//--------------------------------------------------------------
void ofApp::update()
{
    if (this->flow_flag == 1)
        calculatePath(this->water_dots[this->water_dot_index], this->water_lines);
}

//--------------------------------------------------------------
void ofApp::draw()
{
    ofSetColor(127, 23, 31); // Set the drawing color to brown
    // Draw shapes for ceiling and floor
    ofDrawRectangle(0, 0, 1024, 40);   // Top left corner at (50, 50), 100 wide x 100 high
    ofDrawRectangle(0, 728, 1024, 40); // Top left corner at (50, 50), 100 wide x 100 high
    ofSetLineWidth(5);

    /* COMSIL1-TODO 3 : Draw the line segment and dot in which water starts to flow in the screen. Note that after drawing line segment and dot, you have to make selected water start dot colored in red.
    */

    if (this->draw_flag == 1)
    {
        ofSetColor(143, 0, 25);
        for (int i = 0; i < this->water_lines.size(); i++)
            makeLine(this->water_lines[i]);

        ofSetColor(0, 0, 0);
        for (int i = 0; i < this->water_dots.size(); i++)
            makeCircle(this->water_dots[i]);
    }
    if (this->flow_flag == 1 && this->draw_flag == 1)
    {
        for (int i = 0; i < water_drop_count; i++)
            drawDrop(this->water_drops[i]);
        water_drop_count++;
    }
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key)
{
    // input.txt data를 입력 받는다
    if (key == 'l')
    {
        cout << "L key Pressed" << endl;
        //input 파일 선택을 위한 다일로그 창 생성
        ofFileDialogResult fileResult = ofSystemLoadDialog("Load input file");
        processOpenFileSelection(fileResult);
    }
    if (key == 'v')
    {
        // HACK: only needed on windows, when using ofSetAutoBackground(false)
        glReadBuffer(GL_FRONT);
        ofSaveScreen("savedScreenshot_" + ofGetTimestampString() + ".png");
    }
    // 선분과 점 그리기
    if (key == 'd')
    {
        this->draw_flag = 1;
        cout << "Draw key Pressed" << endl;
    }

    if (key == 's')
    {
        if (this->draw_flag == 1)
            this->flow_flag = 1;
        cout << "Start key Pressed" << endl;
    }

    if (key == 'e')
    {
        this->flow_flag = 0;
        cout << "End key Pressed" << endl;
    }
    // 왼쪽 or 오른쪽 키 눌러서 물이 흘러나올 점 선택
    if (key == OF_KEY_LEFT)
    {
        cout << "Left key Pressed" << endl;

        if (this->draw_flag == 1)
        {
            //최소까지 이동
            if (water_dot_index > 0)
            {
                this->water_dots[water_dot_index--].water_falling = false;
                this->water_dots[water_dot_index].water_falling = true;
            }
            else
            {
                this->water_dots[water_dot_index].water_falling = false;
                this->water_dots[this->water_dots.size() - 1].water_falling = true;
                water_dot_index = this->water_dots.size() - 1;
            }
        }
    }

    if (key == OF_KEY_RIGHT)
    {
        cout << "Right key Pressed" << endl;

        if (this->draw_flag == 1)
        {
            //최대까지 이동
            if (water_dot_index < this->water_dots.size() - 1)
            {
                this->water_dots[water_dot_index++].water_falling = false;
                this->water_dots[water_dot_index].water_falling = true;
            }
            else
            {
                this->water_dots[water_dot_index].water_falling = false;
                this->water_dots[0].water_falling = true;
                water_dot_index = 0;
            }
        }
    }

    if (key == 'q')
    {
        cout << "Quit key Presssed" << endl;
        // 각각의 size를 0으로 clear
        this->water_lines.clear();
        this->water_dots.clear();
        this->water_drops.clear();
        // swap() 통해 메모리 할당 해제
        vector<Dot>().swap(this->water_dots);
        vector<Line>().swap(this->water_lines);
        vector<WaterDrop>().swap(this->water_drops);

        cout << "Memory for water line segment has been freed." << endl;
        cout << "Memory for water dots has been freed." << endl;
        cout << "Memory for water drops has been freed." << endl;
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

void ofApp::processOpenFileSelection(ofFileDialogResult openFileResult)
{
    if (openFileResult.bSuccess)
    {
        //파일 열기
        string file_location = openFileResult.getPath();
        ifstream input_file;
        input_file.open(file_location);

        if (!input_file)
            cout << "Target file does not exists." << endl;
        else
            cout << "We found the target file." << endl;
        
        string line;
        int lines;
        int startX, startY, toX, toY, pointX, pointY;
        float slope;
        
        // line segment와 dot에 대한 좌표 (coordinate) 입력 받기
        if (input_file.is_open())
        {
            //선분
            input_file >> lines;
            for (int i = 0; i < lines; i++)
            {
                input_file >> startX;
                input_file >> startY;
                input_file >> toX;
                input_file >> toY;

                Line line;
                line.startX = startX;
                line.startY = startY;
                line.toX = toX;
                line.toY = toY;

                //기울기 계산하고 넣기
                //기울기 공식 = (y2-y1)/(x2-x1)
                slope = (double)(line.toY - line.startY) / (double)(line.toX - line.startX);
                line.slope = slope;

                this->water_lines.push_back(line);
            }

            //점
            input_file >> lines;
            for (int i = 0; i < lines; i++)
            {
                input_file >> pointX;
                input_file >> pointY;

                Dot water_dot_tmp;
                water_dot_tmp.X = pointX;
                water_dot_tmp.Y = pointY;
                //
                if (i == 0)
                    water_dot_tmp.water_falling = true;
                else
                    water_dot_tmp.water_falling = false;

                this->water_dots.push_back(water_dot_tmp);
            }
        }
        else
            cout << "Failed to open file" << endl;
    }
}

void ofApp::makeLine(Line water_line)
{
    ofDrawLine(water_line.startX, water_line.startY, water_line.toX, water_line.toY);
}

// 원의 반지름은 10, 물이 나오는 곳이면 빨간색으로 아니면 검정색으로 표시
void ofApp::makeCircle(Dot water_dot)
{
    if (water_dot.water_falling)
    {
        ofSetColor(255, 0, 0);
        ofDrawCircle(water_dot.X, water_dot.Y, 10);
    }
    else
    {
        ofSetColor(0, 0, 0);
        ofDrawCircle(water_dot.X, water_dot.Y, 10);
    }
}

void ofApp::drawDrop(WaterDrop water_drop)
{
    ofSetColor(0, 0, 255);
    ofDrawCircle(water_drop.current_x, water_drop.current_y, 5);
}

void ofApp::calculatePath(Dot water_falling, vector<Line> water_lines)
{
    //임시 좌표
    float x_tmp, y_tmp;
    x_tmp = water_falling.X;
    y_tmp = water_falling.Y;

    WaterDrop water_drop_tmp;

    for (int i = 0; i < 1000; i++)
    {
        y_tmp++;
        
        // 선반의 개수-1 까지 반복하여 비교
        for (int j = 0; j < water_lines.size(); j++)
        {
            if ((int)y_tmp == (int)(water_lines[j].startY + (x_tmp - water_lines[j].startX) * water_lines[j].slope))
            {
                if (water_lines[j].startX <= x_tmp && x_tmp <= water_lines[j].toX)
                {
                    x_tmp += (1 / water_lines[j].slope);
                    y_tmp--;
                    y_tmp += water_lines[j].slope;
                }
            }
        }
        water_drop_tmp.current_x = x_tmp;
        water_drop_tmp.current_y = y_tmp;
        this->water_drops.push_back(water_drop_tmp);
    }
}

