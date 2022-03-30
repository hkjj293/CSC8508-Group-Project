#include "DebugMode.h"

bool DebugMode::isDebug = false;
float DebugMode::memUsed = 0;
int DebugMode::ManifoldsInfo = 0;
std::queue<std::string> DebugMode::runtimeQueue;
std::vector<MemoryInformations> DebugMode::memoryInformations;
std::queue<MemoryInformations> DebugMode::memQueue;
std::queue<std::string> DebugMode::msgQueue;
Tasks DebugMode::tasks;
Timepoint DebugMode::start;
Timepoint DebugMode::end;


DebugMode::DebugMode(int num) {
    InitTasks(num);
}

void DebugMode::UpdateDebug(float dt) {
    if (!isDebug) {
        return;
    }
    GetMemoryInfo();
	GetPhysicsInfo();
	GetFPS(dt);
    //GetRunTime();
	std::cout << std::endl;
}