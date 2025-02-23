#ifndef __PROCESS_POOL_HPP__
#define __PROCESS_POOL_HPP__

#include <iostream>
#include <cstdlib>
#include <vector>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <string>
#include "Task.hpp"

const int gdef_channel_num = 5; // 默认管道数量

//
class Channel
{
public:
    Channel(int wfd, pid_t subid)
        : _wfd(wfd), _subid(subid)
    {
        _name = "channel_" + std::to_string(wfd) + "_" + std::to_string(subid);
        // to_string()函数将数字转化为字符串
    }
    ~Channel() {}

    void Send(int code) // 向管道发送消息
    {
        int n = write(_wfd, &code, sizeof(int));
        (void)n; // 因为n定义了但是没有使用，所以多一步来避免编译警告
    }

    std::string Name()
    {
        return _name;
    }

    void Close()
    {
        close(_wfd);
    }

    void Wait()
    {
        pid_t rid = waitpid(_subid, nullptr, 0);
        (void)rid;
    }

private:
    int _wfd;          // 写入管道
    pid_t _subid;      // 子进程id
    std::string _name; // 管道名称
};

class ChannelManager
{
public:
    ChannelManager()
        : _next(0)
    {
    }
    ~ChannelManager() {}

    void BuildChannel(int wfd, pid_t subid) // 构建通信管道,wfd是子进程对应的写端，subid是子进程对应的pid
    {
        _channels.emplace_back(wfd, subid);
        // C++11新标准中，emplace_back()函数可以直接在容器末尾添加元素，不需要先构造对象再push_back()
        //  Channel c(wfd,subid);
        //  _channels.push_back(c);
    }

    Channel &Select()
    {
        auto &c = _channels[_next];
        _next++;
        _next %= _channels.size();
        return c;
    }

    void PrintChannel() // 打印通信管道信息
    {
        for (auto &channel : _channels)
        {
            std::cout << channel.Name() << std::endl;
        }
    }

    void StopSubProcess()// 停止子进程
    {
        for(auto &channel: _channels)
        {
            channel.Close();
            std::cout<<"关闭管道"<<channel.Name()<<std::endl;
        }
    }

    void WaitSubProcess()// 等待子进程
    {
        for(auto &channel: _channels)
        {
            channel.Wait();
            std::cout<<"回收子进程"<<channel.Name()<<std::endl;
        }
    }

private:
    std::vector<Channel> _channels;
    int _next;
};

class ProcessPool
{
public:
    ProcessPool(int num)
        : _process_num(num)
    {
        _tm.RegisterTask(PrintLog);
        _tm.RegisterTask(Download);
        _tm.RegisterTask(Upload);
    }
    ~ProcessPool() {}

    

    void ChileRead(int rfd) // 子进程读
    {
        while (1)
        {
            int code = 0;
            ssize_t n = read(rfd, &code, sizeof(int));
            if (n > 0)
            {
                if (n != sizeof(code))
                {
                    continue;
                }
                std::cout << "子进程【" << getpid() << "】收到消息：" << code << std::endl;
                _tm.Execute(code);
            }

            else if (n == 0) // 说明父进程关了写端
            {
                std::cout << "子进程退出" << std::endl;
                break;
            }

            else // 读失败
            {
                std::cout << "读取错误" << std::endl;
                break;
            }
            std::cout << "child read, rfd=" << rfd << std::endl;
            sleep(1);
        }
    }

    bool Create() // 创建进程池
    {
        for (int i = 0; i < _process_num; i++)
        {

            // 1. 创建管道
            int pipefd[2] = {0};
            int n = pipe(pipefd);
            if (n < 0)
                return false;
            // 2.创建子进程
            pid_t subid = fork();
            if (subid < 0)
                return false;

            else if (subid == 0)
            {
                // 子进程读
                // 3. 关闭子进程的写
                close(pipefd[1]);
                ChileRead(pipefd[0]);
                // 4. 关闭子进程的读
                close(pipefd[0]);
                exit(0);
            }

            else
            {
                // 父进程写
                // 3.关闭父进程的读
                close(pipefd[0]);                   // 写端是pipefd[1]
                _cm.BuildChannel(pipefd[1], subid); // 构建通信管道,pipefd[1]是子进程对应的写端，subid是子进程对应的pid
            }
        }
        return true;
    }

    void Debug()
    {
        _cm.PrintChannel();
    }

    void RunTask() // 向进程池推送任务
    {
        // 选择一个任务
        int taskcode = _tm.Code();
        // 选择一个管道
        auto &c = _cm.Select();
        std::cout << "选择了一个子进程：" << c.Name() << std::endl;
        // 向管道发送任务
        c.Send(taskcode);
        std::cout << "向子进程发送任务码：" << taskcode << std::endl;
    }

    void Stop() // 停止进程池
    {
        //只需要关闭父进程，这样子进程就会读到0，然后需要回收子进程
        _cm.StopSubProcess();
        //回收子进程        
        _cm.WaitSubProcess();
    }
private:
    ChannelManager _cm;
    int _process_num; // 进程数量
    TaskManager _tm;
};

#endif