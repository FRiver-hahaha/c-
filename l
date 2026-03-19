#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#define COLOR_RESET "\033[0m" // 重置 
#define COLOR_WELCOME "\033[1;34m" // 欢迎界面(粗体蓝色)
#define COLOR_DISPLAY "\033[1;33m" // 菜单显示(粗体黄色)
#define COLOR_ERROR "\033[1;31m" // 用户出错(粗体红色)

#define MAX_NAME 30
#define MAX_PASSWORD 20
#define MAX_ONLYNUMBER 6

void firstShow();// 欢迎界面
void menu();// 菜单显示
void signin();// 登录
void signup();// 注册
void quickGet();// 快速取件
void readMeUsing();// 使用说明
void aboutUs();// 关于我们
void exitFirst();// 退出程序
char handleButton1(char button1);// 处理大小写问题

typedef struct user {
    char name[MAX_NAME];// 用户名
    char password[MAX_PASSWORD]// 密码
    char onlyNumber[MAX]
}

int main() {
    firstShow();
    menu();
    
    return 0;
}

void quickGet() {
    printf("快速取件界面\n");
}

void signup() {
    printf("注册界面\n");
}

void signin() {
    printf("登录界面\n");
}

/*
    用来退出应用。
    简单的询问以及switch。
*/

void exitFirst() {
    char button1;
    printf("您确定要退出吗? yes/no (y/n):");
    scanf(" %c", &button1);
    button1 = handleButton1(button1);
    switch (button1)
    {
    case 'y':
        exit(EXIT_SUCCESS);
        break;

    case 'n':
        break;

    default:
        break;
    }
}

/*
    用来展示用户关于作者的信息。
    创建了一个文件指针，和提供展示的char类型缓冲区。
    使用了fopen,fread,fclose,
    用if实现清晰的用户报错显示。
*/

void aboutUs() {
    char buf[2];
    char button1;
    ssize_t readCnt;
    int fd = open("aboutus.md", O_RDWR);
    if(fd == -1) {
        printf("%s无法打开该文件!请确定文件目录里是否有aboutUs.md文件.%s\n", COLOR_ERROR, COLOR_RESET);
        return;
    }
    while((readCnt = read(fd, buf, 1)) > 0) {
        buf[1] = '\0';
        printf("%s", buf);
    }   
    close(fd);
    printf("按任意键回车后退出...\n");
    scanf(" %c", &button1);
    return;
}

/*
    用来展示用户使用当前系统的说明。
    创建了一个文件指针，和提供展示的char类型缓冲区。
    使用了fopen,fread,fclose,
    用if实现清晰的用户报错显示。
*/

void readMeUsing() {
    char buf[2];
    char button1;
    ssize_t readCnt;
    int fd = open("readmeusing.md", O_RDWR);
    if(fd == -1) {
        printf("%s无法打开该文件!请确定文件目录里是否有readMeUsing.md文件.%s\n", COLOR_ERROR, COLOR_RESET);
    }
    while((readCnt = read(fd, buf, 1)) > 0) {
        buf[1] = '\0';
        printf("%s", buf);
    }
    close(fd);
    printf("按任意键回车后退出...\n");
    scanf(" %c", &button1);
    return;
    
}

/*
    用来处理用户输入的大小写。
    如果大写就换成小写，
    小写直接返回。
*/

char handleButton1(char button1) {
    if(button1 >= 65 && button1 <= 65 + 26) {
        button1 += 32;
    }
    return button1;
}

/*
    菜单界面。
    先显示好提供用户可以进行的操作，
    之后在while循环内等待用户输入，
    保证用户输入错误之后，
    依然可以供用户继续输入。
    通过switch确定要进入哪个函数。
*/

void menu() {
    printf("%s\\== 欢迎进入社区快递柜管理系统 ==/%s\n", COLOR_WELCOME, COLOR_RESET);
    char button1;
    while(1) {
        printf("%s\\==%s%sq%s %s登录 ==/%s\n", COLOR_WELCOME, COLOR_RESET, COLOR_DISPLAY, COLOR_RESET, COLOR_WELCOME, COLOR_RESET);
        printf("%s\\==%s%sw%s %s注册 ==/%s\n", COLOR_WELCOME, COLOR_RESET, COLOR_DISPLAY, COLOR_RESET, COLOR_WELCOME, COLOR_RESET);
        printf("%s\\==%s%se%s %s快速取件 ==/%s\n", COLOR_WELCOME, COLOR_RESET, COLOR_DISPLAY, COLOR_RESET, COLOR_WELCOME, COLOR_RESET);
        printf("%s\\==%s%sr%s %s使用说明 ==/%s\n", COLOR_WELCOME, COLOR_RESET, COLOR_DISPLAY, COLOR_RESET, COLOR_WELCOME, COLOR_RESET);
        printf("%s\\==%s%st%s %s关于我们 ==/%s\n", COLOR_WELCOME, COLOR_RESET, COLOR_DISPLAY, COLOR_RESET, COLOR_WELCOME, COLOR_RESET);
        printf("%s\\==%s%sx%s %s退出 ==/%s\n", COLOR_WELCOME, COLOR_RESET, COLOR_DISPLAY, COLOR_RESET, COLOR_WELCOME, COLOR_RESET);
        printf("%s请选择您要进行的操作:%s\n", COLOR_DISPLAY, COLOR_RESET);
        scanf(" %c", &button1);
        button1 = handleButton1(button1);// 处理用户大小写问题
        switch (button1)
        {
        case 'q':
            signin();
            break;

        case 'w':
            signup();
            break;
        
        case 'e':
            quickGet();
            break;

        case 'r':
            readMeUsing();
            break;

        case 't':
            aboutUs();
            break;

        case 'x':
            exitFirst();
            break;

        default:
            printf("%s输入错误!请重试...%s\n", COLOR_ERROR, COLOR_RESET);
            break;
        }
    }
}

/*
    启动程序最开始的欢迎界面。
    用了两个for循环嵌套，
    第一个循环展示行，
    第二个循环确定行内内容。
*/

void firstShow() {
    int time;
    char flag = '#';
    char* wl = "Welcome";
    char* wl2 = "社区快递柜管理系统";
    for(time = 1; time <= 10; time++) {
        for(int i = 0; i < 30; i++) {
            printf("%c",flag);
            if((i == 29)) {
                printf("\n");
            }
        }
        if(time == 10 / 2) {
            for(int i = 0; i < 10; i++) {
                printf("%c",flag);
            }
            printf("%s%s%s", COLOR_WELCOME, wl, COLOR_RESET);
            for(int i = 0; i < 13; i++) {
                printf("%c",flag);
                if(i == 12) {
                    printf("\n");
                }
            }
            for(int i = 0; i < 2 + 4; i++) {
                printf("%c",flag);
            }
            printf("%s%s%s", COLOR_WELCOME, wl2, COLOR_RESET);
            for(int i = 0; i < 2 + 4; i++) {
                printf("%c",flag);
                if(i == 5) {
                    printf("\n");
                }
            }
        }
        usleep(50000);
    };
}