/*
    #include <sys/types.h>
    #include <dirent.h>
    DIR *opendir(const char *name);
        参数：
            - name 目录路径
        返回值：
            - DIR 目录流信息
            错误返回null
    #include <dirent.h>
    struct dirent *readdir(DIR *dirp);
        参数：
            - dirp 是opendir返回的结果
            - 返回值：
                struct dirent 返回的结构体信息
                读取到了末尾或者失败 返回null
    //关闭目录
    #include <sys/types.h>
    #include <dirent.h>
    int closedir(DIR *dirp);

*/
#include <sys/types.h>
#include <dirent.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
int getFileNum(const char* path);
int main(int argc,char* argv[]){
    if(argc<2){
        printf("%s path\n",argv[0]);
        return -1;
    }
    int num=getFileNum(argv[1]);
    printf("普通文件的个数为：%d\n",num);
    return 0;
}

//用于获取目录下所有普通文件的个数
int getFileNum(const char* path){
    int res=0;
    DIR* dir=opendir(path);
    if(dir==NULL){
        perror("opendir:");
        return -1;
    }
    struct dirent *ptr=NULL;
    while((ptr=readdir(dir))!=NULL){
        char* dname=ptr->d_name;
        if(strcmp(dname,".")==0||strcmp(dname,"..")==0)
            continue;
        //判断是否是普通文件
        if(ptr->d_type==DT_DIR){
            char newpath[256];
            sprintf(newpath,"%s/%s",path,dname);
            res+=getFileNum(newpath);
        }
        if(ptr->d_type==DT_REG){
            printf("%s\n",dname);
            res++;
        }
    }
    closedir(dir);
    return res;
}