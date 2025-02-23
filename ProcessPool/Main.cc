#include "ProcessPool.hpp"

int main() {    
    ProcessPool pool(gdef_channel_num);
    //创建进程池对象

    pool.Create();
    //启动进程池
    
    int task_code = 1;
    int cnt = 10;
    while(cnt--){
        pool.RunTask(); // 向进程池推送任务
        sleep(1); // 模拟任务处理时间
    }
    pool.Stop(); // 停止进程池
    return 0;
}