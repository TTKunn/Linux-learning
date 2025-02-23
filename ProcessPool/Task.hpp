#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <ctime>

typedef void (*task_t)();

void PrintLog(){
    std:: cout<<"打印日志"<<std::endl;
}

void Download()
{
    std::cout << "下载任务" << std::endl;
}

void Upload()
{
    std::cout << "上传任务" << std::endl;
}

// ------------------------------------------------

class TaskManager {
public:
    TaskManager() 
    {
        srand(time(nullptr));
    }
    ~TaskManager() {}

    void RegisterTask(task_t task) 
    {
        _tasks.push_back(task);
    }
    int Code(){
        return rand()%_tasks.size();
    }
    void Execute(int code){
        if(code >=0 && code<_tasks.size())
            _tasks[code]();
    }

private:
    std::vector<task_t> _tasks;
};