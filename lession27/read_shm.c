#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
#include <string.h>
int main(){
    //获取一个共享内存
    int shmid=shmget(100,0,IPC_CREAT);
    printf("shmid: %d\n",shmid);
    //和当前进程进行关联
    void* ptr=shmat(shmid,NULL,0);
    //读数据
    printf("%s\n",(char*)ptr);
    printf("按任意键继续\n");
    getchar();
    //解除关联
    shmdt(ptr);
    //删除共享内存
    shmctl(shmid,IPC_RMID,NULL);
    return 0;
}