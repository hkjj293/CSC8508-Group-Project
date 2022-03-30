#pragma once
#include <vector>
#include <string>
using namespace std;

class LevelGen {
public:

    LevelGen();
    ~LevelGen();

    void Generate(int l, int w);
    vector<string> GetLevelStrings();

private:

    void LevelToString(int numLevel);
    void AddStairs();
    bool neighbourStairCheck();
    void StairPositions();
    void MazeGenCheck();
    void PathToStairs();
    void MazeGen();

    //void LevelTextFile(string fileName);

    vector<vector<string>> level;

    bool up = false;
    bool down = false;
    bool left = false;
    bool right = false;

    int posX;
    int posY;

    int tempX;
    int tempY;

    int length;
    int height;
    int floors;

    bool end;

    int randInt1;
    bool check;
    int optionsCount;

    int numberOfStairs;
    vector<vector<int>> stairPos;

    vector<string> levelString;

};