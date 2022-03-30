#include "LevelGen.h"
using namespace std;
#include <stdlib.h>
#include <string>

#include <fstream> // TEST

LevelGen::LevelGen() {

}

LevelGen::~LevelGen() {

}

void LevelGen::Generate(int l, int w) {

    length = w;
    height = l;

    level.resize(l);
    for (int i = 0; i < l; i++) {
        level[i].resize(w);
    }

    numberOfStairs = 0;
    MazeGenCheck();
    AddStairs();
    //LevelTextFile("map1"); //TEST
    LevelToString(0);

    MazeGenCheck();
    numberOfStairs = 0;
    AddStairs();
    PathToStairs();
    //LevelTextFile("map2"); //TEST
    LevelToString(1);

    MazeGenCheck();
    PathToStairs();
    //LevelTextFile("map3"); //TEST
    LevelToString(2);

}

vector<string> LevelGen::GetLevelStrings() {

    return levelString;

}

void LevelGen::LevelToString(int numLevel) {
    string tempString = "";

    for (int i = 0; i < height; i++)
    {
        for (int z = 0; z < length; z++)
        {
            tempString = tempString + level[z][i];
        }
    }
    levelString.push_back(tempString);
}

/*
void LevelGen::LevelTextFile(string fileName) { //TEST
    string mapline = "";
    ofstream File(fileName + ".txt");

    for (int i = 0; i < height; i++)
    {
        for (int z = 0; z < length; z++)
        {
            mapline = mapline + level[z][i];
        }
        File << (mapline + "\n");
        mapline = "";
    }
}
*/

void LevelGen::AddStairs() {
    tempX = 0;
    tempY = 0;

    while (true)
    {
        up = false;
        down = false;
        left = false;
        right = false;
        optionsCount = 0;

        int r = rand() % 4;

        if (r == 1)
        {

            if (level[tempX][tempY] == "P" && neighbourStairCheck())
            {
                if (tempY != 0) { if (level[tempX][tempY - 1] == "P") { up = true; optionsCount++; } }
                if (tempY != (height - 1)) { if (level[tempX][tempY + 1] == "P") { down = true; optionsCount++; } }
                if (tempX != 0) { if (level[tempX - 1][tempY] == "P") { left = true; optionsCount++; } }
                if (tempX != (length - 1)) { if (level[tempX + 1][tempY] == "P") { right = true; optionsCount++; } }

                if (optionsCount == 1)
                {
                    if (up == true) { level[tempX][tempY] = "A"; numberOfStairs++; }
                    if (down == true) { level[tempX][tempY] = "V"; numberOfStairs++; }
                    if (left == true) { level[tempX][tempY] = "<"; numberOfStairs++; }
                    if (right == true) { level[tempX][tempY] = ">"; numberOfStairs++; }
                }

                else if (optionsCount == 2)
                {
                    if ((up == true && down == true) || (left == true && right == true)) {}
                    else
                    {
                        if (up == true && left == true && level[tempX - 1][tempY - 1] == "P") { level[tempX][tempY] = "A"; numberOfStairs++; }
                        if (up == true && right == true && level[tempX + 1][tempY - 1] == "P") { level[tempX][tempY] = ">"; numberOfStairs++; }
                        if (down == true && left == true && level[tempX - 1][tempY + 1] == "P") { level[tempX][tempY] = "<"; numberOfStairs++; }
                        if (down == true && right == true && level[tempX + 1][tempY + 1] == "P") { level[tempX][tempY] = "V"; numberOfStairs++; }

                    }
                }
            }
        }

        tempX++;
        if (tempX == length)
        {
            tempX = 0;
            tempY++;
        }
        if (tempY == height) { AddStairs(); }

        if (numberOfStairs >= ((length * height) / 50)) { StairPositions(); break; }
    }
}

bool LevelGen::neighbourStairCheck() {
    bool check = true;
    if (tempY != 0) { if (level[tempX][tempY - 1] == "A" || level[tempX][tempY - 1] == "V" || level[tempX][tempY - 1] == "<" || level[tempX][tempY - 1] == ">") { check = false; } }
    else if (tempY != (height - 1)) { if (level[tempX][tempY + 1] == "A" || level[tempX][tempY + 1] == "V" || level[tempX][tempY + 1] == "<" || level[tempX][tempY + 1] == ">") { check = false; } }
    else if (tempX != 0) { if (level[tempX - 1][tempY] == "<" || level[tempX - 1][tempY] == ">" || level[tempX - 1][tempY] == "A" || level[tempX - 1][tempY] == "V") { check = false; } }
    else if (tempX != (length - 1)) { if (level[tempX + 1][tempY] == "<" || level[tempX + 1][tempY] == ">" || level[tempX + 1][tempY] == "A" || level[tempX + 1][tempY] == "V") { check = false; } }
    return check;
}

void LevelGen::StairPositions() {

    stairPos.clear();

    for (int i = 0; i < height; i++)
    {
        for (int z = 0; z < length; z++)
        {
            if (level[z][i] == "A" || level[z][i] == "V" || level[z][i] == "<" || level[z][i] == ">")
            {
                vector<int> tempIntVec;
                tempIntVec.push_back(z);
                tempIntVec.push_back(i);

                stairPos.push_back(tempIntVec);

            }
        }
    }
}

void LevelGen::MazeGenCheck() {
    int pathCounter;
    while (true)
    {
        MazeGen();

        bool stairCheck = true;
        pathCounter = 0;
        for (int i = 0; i < height; i++)
        {
            for (int z = 0; z < length; z++)
            {
                if (level[z][i] == "P") { pathCounter++; }
            }
        }
        if (pathCounter >= ((length * height) / 2) && stairCheck) { break; }
    }
}

void LevelGen::PathToStairs() {
    bool stairCheck = false;

    for (int i = 0; i < height; i++)
    {
        for (int z = 0; z < length; z++)
        {
            if (level[z][i] == "S")
            {
                stairCheck = false;
                if (i != 0) { if (level[z][i - 1] == "P") { stairCheck = true; } }
                if (i != (height - 1)) { if (level[z][i + 1] == "P") { stairCheck = true; } }
                if (z != 0) { if (level[z - 1][i] == "P") { stairCheck = true; } }
                if (z != (length - 1)) { if (level[z + 1][i] == "P") { stairCheck = true; } }


                if (stairCheck == false)
                {
                    int x = z;
                    int y = i;
                    while (stairCheck == false)
                    {
                        up = false;
                        down = false;
                        left = false;
                        right = false;

                        if (y != 0) { if (level[x][y - 1] == "#") { up = true; } }
                        if (y != (height - 1)) { if (level[x][y + 1] == "#") { down = true; } }
                        if (x != 0) { if (level[x - 1][y] == "#") { left = true; optionsCount++; } }
                        if (x != (length - 1)) { if (level[x + 1][y] == "#") { right = true; } }


                        int nextDirection = rand() % 5;

                        if (nextDirection == 0)
                        {
                            if (up == true) { level[x][y - 1] = "T"; y--; }
                            else { nextDirection++; }
                        }
                        if (nextDirection == 1)
                        {
                            if (down == true) { level[x][y + 1] = "T"; y++; }
                            else { nextDirection++; }
                        }
                        if (nextDirection == 2)
                        {
                            if (left == true) { level[x - 1][y] = "T"; x--; }
                            else { nextDirection++; }
                        }
                        if (nextDirection == 3)
                        {
                            if (right == true) { level[x + 1][y] = "T"; x++; }
                            else { nextDirection++; }
                        }

                        if (y != 0) { if (level[x][y - 1] == "P") { stairCheck = true; break; } }
                        if (y != (height - 1)) { if (level[x][y + 1] == "P") { stairCheck = true; break; } }
                        if (x != 0) { if (level[x - 1][y] == "P") { stairCheck = true; break; } }
                        if (x != (length - 1)) { if (level[x + 1][y] == "P") { stairCheck = true; break; } }
                    }
                }
            }
        }
    }

    for (int i = 0; i < height; i++)
    {
        for (int z = 0; z < length; z++)
        {
            if (level[z][i] == "T") { level[z][i] = "P"; }
        }
    }
}

void LevelGen::MazeGen() {

    for (int i = 0; i < height; i++) // populate
    {
        for (int z = 0; z < length; z++)
        {
            level[z][i] = "S";
        }
    }

    posX = rand() % length; // pick random start
    posY = rand() % height;

    level[posX][posY] = "P";

    end = false;
    while (end == false)
    {

        up = false;
        down = false;
        left = false;
        right = false;
        optionsCount = 0;


        if (posY != 0) { if (level[posX][posY - 1] == "S") { up = true; optionsCount++; } } // check if empty
        if (posY != (height - 1)) { if (level[posX][posY + 1] == "S") { down = true; optionsCount++; } }
        if (posX != 0) { if (level[posX - 1][posY] == "S") { left = true; optionsCount++; } }
        if (posX != (length - 1)) { if (level[posX + 1][posY] == "S") { right = true; optionsCount++; } }


        if (up == false && down == false && left == false && right == false)
        {
            tempX = 0;
            tempY = 0;
            while (true) // find new start
            {
                if (level[tempX][tempY] == "P")
                {
                    if (tempY != 0) { if (level[tempX][tempY - 1] == "S") { posX = tempX; posY = tempY; break; } }
                    if (tempY != (height - 1)) { if (level[tempX][tempY + 1] == "S") { posX = tempX; posY = tempY; break; } }
                    if (tempX != 0) { if (level[tempX - 1][tempY] == "S") { posX = tempX; posY = tempY; break; } }
                    if (tempX != (length - 1)) { if (level[tempX + 1][tempY] == "S") { posX = tempX; posY = tempY; break; } }

                }

                tempX++;
                if (tempX == length)
                {
                    tempX = 0;
                    tempY++;
                }
                if (tempY == height) { end = true; break; }
            }

        }

        else
        {
            randInt1 = 4;
            if (optionsCount > 1)
            {
                check = true;
                while (check)
                {
                    randInt1 = rand() % 5;

                    switch (randInt1)
                    {
                    case 0:
                        if (up == true)
                        {
                            check = false;
                            level[posX][posY - 1] = "#";
                        }
                        break;
                    case 1:
                        if (down == true)
                        {
                            check = false;
                            level[posX][posY + 1] = "#";
                        }
                        break;
                    case 2:
                        if (left == true)
                        {
                            check = false;
                            level[posX - 1][posY] = "#";
                        }
                        break;
                    case 3:
                        if (right == true)
                        {
                            check = false;
                            level[posX + 1][posY] = "#";
                        }
                        break;
                    }

                }
            }
            check = true;
            while (check) // pick next path
            {
                int randInt2;

                if (randInt1 != 4)
                {
                    while (true) // check to not replace wall with path
                    {
                        randInt2 = rand() % 5;
                        if (randInt2 != randInt1) { break; }
                    }
                }
                else
                {
                    randInt2 = rand() % 5;
                }
                switch (randInt2)
                {
                case 0:
                    if (up == true)
                    {
                        check = false;
                        level[posX][posY - 1] = "P";
                        posY = posY - 1;
                    }
                    break;
                case 1:
                    if (down == true)
                    {
                        check = false;
                        level[posX][posY + 1] = "P";
                        posY = posY + 1;
                    }
                    break;
                case 2:
                    if (left == true)
                    {
                        check = false;
                        level[posX - 1][posY] = "P";
                        posX = posX - 1;
                    }
                    break;
                case 3:
                    if (right == true)
                    {
                        check = false;
                        level[posX + 1][posY] = "P";
                        posX = posX + 1;
                    }
                    break;
                }

            }
        }
    }


    for (int i = 0; i < height; i++) // fill gaps
    {
        for (int z = 0; z < length; z++)
        {
            if (level[z][i] == "S") { level[z][i] = "#"; }
        }
    }


    int paths;
    for (int i = 0; i < height; i++) // add connecting paths
    {
        for (int z = 0; z < length; z++)
        {
            if (level[z][i] == "#")
            {
                paths = 0;
                if (i != 0) { if (level[z][i - 1] == "P") { paths++; } }
                if (i != (height - 1)) { if (level[z][i + 1] == "P") { paths++; } }
                if (z != 0) { if (level[z - 1][i] == "P") { paths++; } }
                if (z != (length - 1)) { if (level[z + 1][i] == "P") { paths++; } }

                if (paths > 1) {
                    int r = rand() % 4;
                    if (r == 0)
                    {
                        level[z][i] = "P";
                    }
                }
            }
        }
    }

    if (numberOfStairs > 0) {
        for (int i = 0; i < height; i++) // create space for stairs
        {
            for (int z = 0; z < length; z++)
            {
                for (int x = 0; x < numberOfStairs; x++)
                {
                    if (z == stairPos[x][0] && i == stairPos[x][1]) { level[z][i] = "S"; }
                }
            }
        }
    }

}
