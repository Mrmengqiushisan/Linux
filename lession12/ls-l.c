#include<stdio.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<unistd.h>
#include<pwd.h>
#include<grp.h>
#include<time.h>
#include<string.h>
//-rw-rw-r-- 1 wangtao wangtao 0  9月  8 16:18 a.txt
//模拟实现ls-l 命令
int main(int argc,char* argv[]){
    if(argc<2){
        printf("%s filename\n",argv[0]);
        return -1;
    }
    struct stat st;
    int ret=stat(argv[1],&st);
    if(ret==-1){
        perror("stat failed:");
        return -1;
    }
    //获取文件类型和文件权限
    char perms[11]={0};
    switch (st.st_mode&__S_IFMT)
    {
    case __S_IFLNK:
        perms[0]='l';
        break;
    case __S_IFDIR:
        perms[0]='d';
        break;
    case __S_IFREG:
        perms[0]='-';
        break;
    case __S_IFBLK:
        perms[0]='b';
        break;
    case __S_IFCHR:
        perms[0]='c';
        break;
    case __S_IFSOCK:
        perms[0]='s';
        break;
    case __S_IFIFO:
        perms[0]='t';
        break;
    default:
        perms[0]='?';
        break;
    }
    perms[1]=(st.st_mode&S_IRUSR)?'r':'-';
    perms[2]=(st.st_mode&S_IWUSR)?'w':'-';
    perms[3]=(st.st_mode&S_IXUSR)?'x':'-';

    perms[4]=(st.st_mode&S_IRGRP)?'r':'-';
    perms[5]=(st.st_mode&S_IWGRP)?'w':'-';
    perms[6]=(st.st_mode&S_IXGRP)?'x':'-';

    perms[7]=(st.st_mode&S_IROTH)?'r':'-';
    perms[8]=(st.st_mode&S_IWOTH)?'w':'-';
    perms[9]=(st.st_mode&S_IXOTH)?'x':'-';

    int linknum=st.st_nlink;

    char* fileUser=getpwuid(st.st_uid)->pw_name;

    char* filegroup=getgrgid(st.st_gid)->gr_name;

    long int fileSize=st.st_size;

    char* time= ctime(&st.st_mtime);
    char mtime[512]={0};
    strncpy(mtime,time,strlen(time)-1);
    char buf[1024]={0};
    sprintf(buf,"%s %d %s %s %ld %s %s",perms,linknum,fileUser,filegroup,fileSize,mtime,argv[1]);
    printf("%s\n",buf);
    return 0;
}