#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>
#include <string.h>
#include <time.h>
#include <pwd.h>

void printbit(int x)  //转换为二进制输出
{
	int i = 16;
	while (i--)
	{
		if (x & (1 << i))
			printf("%d", 1);
		else
			printf("%d", 0);
	}
    printf("  ");
}

int main(int argc,char *argv[])
{
    //定义变量
    DIR *dp;    //目录流指针
    struct dirent *dir;  //目录
    struct stat filestat;   //stat函数变量
    int message,total=0;
    struct tm *t;  //localtime函数变量
    struct passwd *my_info;  //getpwuid函数变量
    
    //获取目录流指针
    dp = opendir("."); //打开当前目录
    if(dp == NULL){
        printf("opendir is error\n");  //打开错误报错
        return -2;
    }
    //else printf("opendir is right\n");
    
//当只输入一个参数时，打印当前目录的文件名（不含隐藏文件），功能与shell中'ls'相同
    if(argc == 1){   //如果只输入了执行文件作为参数
        while(1){
            dir = readdir(dp);
            if(dir != NULL){
                message = stat(dir->d_name,&filestat);
                if(message<0){
                    printf("stat failed  \n");
                    return -1;
                }
                else{ 
                    if(strncmp(dir->d_name, ".", 1) == 0) continue;//跳出以.开头的文件名
                    printf( "%s    ",dir->d_name);
                }
            }
            else{
                printf("\n");
                break;
            }
        }
    }

//当输入两个参数且第二个参数为'-a'时，打印当前目录的文件名（含隐藏文件），功能与shell中'ls -a'相同
    if(argc == 2){//如果只有两个参数
        if(strcmp(argv[1], "-l" ) == 0){ //如果第二个参数为'-l'
            while(1){
                dir = readdir(dp);
                if(dir != NULL){
                    message = stat(dir->d_name,&filestat);
                    t = localtime(&filestat.st_mtime);
                    if(strncmp(dir->d_name, ".", 1) == 0) continue;
                    if(message<0){
                        printf("stat failed\n");
                        return -1;
                    }
                    else{ //输出类似于ls -l
                        printbit(filestat.st_mode);//文件信息权限,二进制输出
                        printf( "%3lu",filestat.st_nlink ); //链接数
                        my_info = getpwuid( filestat.st_uid ); //用户
                        printf( "   %s", my_info->pw_name );
                        my_info = getpwuid( filestat.st_gid );//用户组
                        printf( "   %s", my_info->pw_gecos );
                        printf( " %6ld  ",filestat.st_size ); //大小 
                        //printf( "%12ld  ",filestat.st_mtime );//最后修改时间(数字纯返回值)
                        printf("%4d年%02d月%02d日 %02d:%02d:%02d",t->tm_year+1900,t->tm_mon+1,t->tm_mday,t->tm_hour,t->tm_min,t->tm_sec);//最后修改时间
                        printf("   %s\n",dir->d_name);//文件名
                        if(filestat.st_size <= 4096) total+=1;    
                        else total+=(long int)(filestat.st_size / 4096)+1;
                    }
                }
                else break;
            }
            printf("总用量 %dK\n",total*4);
        }
        else if(strcmp(argv[1], "-a" ) == 0) {//如果第二个参数'-a'输入正确
            while(1){
                dir = readdir(dp);
                if(dir != NULL){
                    message = stat(dir->d_name,&filestat);
                    if(message<0){
                        printf("stat failed \n");
                        return -1;
                    }
                    else printf( "%s    ",dir->d_name);//打印所有文件名
                }
                else{
                    printf("\n");
                    break;
                }
            }
        }
        else printf("please enter the correct format in the second parameter.\n");  //如果第二个参数输入有误
    }
    
//当输入三个参数，且第二个参数为'ls -a'，第三个参数为'-l'时，打印当前目录的文件信息，功能与shell中'ls -l'相同
    if( argc == 3){
        if(strcmp(argv[1], "-a" ) != 0) printf("please enter the correct format in the second parameter.\n");//如果第二个参数'-a'输入有误
        else if((strcmp(argv[1], "-a" ) == 0) && (strcmp(argv[2],"-l") != 0 )){    //如果第三个参数'-l'输入有误
            printf("please enter the correct format in the third parameter.\n");
        }
        else{//如果第二个参数输入正确，且第三个参数'-l'输入正确
            while(1){
                dir = readdir(dp);
                if(dir != NULL){
                    message = stat(dir->d_name,&filestat);
                    t = localtime(&filestat.st_mtime);
                    if(message<0){
                        printf("stat failed,because stat error\n");
                        return -1;
                    }
                    else{ //输出ls -l
                        printbit(filestat.st_mode);//文件信息权限,二进制输出
                        printf( "%3lu",filestat.st_nlink ); //链接数
                        my_info = getpwuid( filestat.st_uid );
                        printf( "   %s", my_info->pw_name );
                        my_info = getpwuid( filestat.st_gid );
                        printf( "   %s", my_info->pw_gecos );
                        printf( " %6ld  ",filestat.st_size ); //大小 
                        //printf( "%12ld  ",filestat.st_mtime );//最后修改时间(数字纯返回值)
                        printf("%4d年%02d月%02d日 %02d:%02d:%02d",t->tm_year+1900,t->tm_mon+1,t->tm_mday,t->tm_hour,t->tm_min,t->tm_sec);//最后修改时间
                        printf("   %s\n",dir->d_name);//文件名
                        if(filestat.st_size <= 4096) total+=1;                     
                        else total+=(long int)(filestat.st_size / 4096)+1; 
                    }
                }
                else break;
            }  
            printf("总用量 %dK\n",total*4);
        }   
    }
    
    if( argc > 3) printf("Please enter correct parameters. \n"); //如果多于三个参数
        
    closedir(dp); //关闭目录
    return 0;
}


