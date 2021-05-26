#pragma once
#include "ofMain.h"
#include <vector>

struct Line
{
    int startX;
    int startY;
    int toX;
    int toY;
    float slope;
};
struct Dot
{
    bool water_falling;
    int X;
    int Y;
};
struct WaterDrop
{
    float current_x;
    float current_y;
};
class ofApp : public ofBaseApp
{
private:
    int draw_flag;
    int flow_flag;
    int water_dot_index = 0;
    vector<Line> water_lines;
    vector<Dot> water_dots;
    vector<WaterDrop> water_drops;

public:
    void setup();
    void update();
    void draw();
    void keyPressed(int key);
    void keyReleased(int key);
    void mouseMoved(int x, int y);
    void mouseDragged(int x, int y, int button);
    void mousePressed(int x, int y, int button);
    void mouseReleased(int x, int y, int button);
    void mouseEntered(int x, int y);
    void mouseExited(int x, int y);
    void windowResized(int w, int h);
    void dragEvent(ofDragInfo dragInfo);
    void gotMessage(ofMessage msg);
    void processOpenFileSelection(ofFileDialogResult openFileResult);
    void makeLine(Line water_line);
    void makeCircle(Dot water_dot);
    void drawDrop(WaterDrop water_drop);
    void calculatePath(Dot water_falling, vector<Line> lines);
};
