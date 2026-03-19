// 2026.3.19

// 明天彻底完成v1.0版本(无数据库版本)

/*
    当前问题：在用户注册的时候应该先检查文件内是否有用户，
    所以需要在最开始的时候先把文件读出来，以链表的形式存储，
    之后在注册的时候，进行比较，如果注册用户没有，则正式注册，并且把用户数据按行写入文件内，
    把当前用户添加到链表末尾，整个文件的排版是按照用户的注册顺序来的，
    登录也是一样，需要先检查链表内是否有他，没有报错，有则登录。
    主要问题函数：signup(), addUser(), 
*/

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#define COLOR_RESET "\033[0m" // 重置 
#define COLOR_WELCOME "\033[1;34m" // 欢迎界面(粗体蓝色)
#define COLOR_DISPLAY "\033[1;33m" // 菜单显示(粗体黄色)
#define COLOR_ERROR "\033[1;31m" // 用户出错(粗体红色)

#define MAX_NAME 15
#define MAX_PASSWORD 30
#define MAX_ONLYNUMBER 10

void firstShow();// 欢迎界面
void menu(LIST_DELIVERY* pListDelivery, LIST_USER* pListUser);// 菜单显示
void signin();// 登录
void signup(LIST_USER* pList);// 注册
void quickGet();// 快速取件
void readMeUsing();// 使用说明
void aboutUs();// 关于我们
void exitFirst();// 退出程序
char handleButton1(char button1);// 处理大小写问题

// 有关用户注册的函数
void initUser(LIST_USER* pList);// 初始化链表
void addUser(LIST_USER* pList, USER* Node);// 增加用户节点
void deleteUser(LIST_USER* pList);// 删除用户节点

// 有关快递的函数
void initDelivery(LIST_DELIVERY* pList);// 初始化链表
void addDelivery(LIST_DELIVERY* pList);// 增加快递节点
void deleteDelivery(LIST_DELIVERY* pList);// 删除快递节点

typedef struct _user {// 用户
    char name[MAX_NAME];// 用户名
    char password[MAX_PASSWORD];// 密码
    char onlyNumber[MAX_ONLYNUMBER];// 唯一6位校验码
    int NumberDelivery;// 代取快递数量
    int issignup;// 是否为注册用户
    int isvip;// 是否为vip用户
    struct _user* prev;
    struct _user* next;
}USER;

typedef struct _admin {// 管理员
    char name[MAX_NAME];// 用户名
    char password[MAX_PASSWORD];// 密码
    struct _admin* prev;
    struct _admin* next;
}ADMIN;

typedef struct _delivery {// 快递
    char Did[MAX_ONLYNUMBER];// 唯一5位取件码
    char userON[MAX_ONLYNUMBER];// 确定被哪位用户取走
    int status;// 是否被取走
    struct _delivery* prev;
    struct _delivery* next;
}DELIVERY;

typedef struct {// 用户链表
    USER* head;// 头节点
}LIST_USER;

typedef struct {// 用户链表
    ADMIN* head;// 头节点
}LIST_ADMIN;

typedef struct {// 用户链表
    DELIVERY* head;// 头节点
}LIST_DELIVERY;

int main() {
    LIST_DELIVERY listDelivery;
    LIST_USER listUser;
    initDelivery(&listDelivery);
    initUser(&listUser);
    firstShow();
    menu(&listDelivery, &listUser);
    
    return 0;
}

void addUser(LIST_USER* pList, USER* Node) {// 
    int fd = open("user.txt", O_RDWR);
    Node->prev = NULL;
    Node->next = NULL;
    Node->isvip = 0;
    Node->issignup = 1;
    Node->NumberDelivery = 0;
    if(pList->head->next == NULL) {// 检查头节点
        pList->head->next = Node;
        Node->prev = pList->head;
        printf("%s用户注册成功!%s\n", COLOR_DISPLAY, COLOR_RESET);
        


        return;
    }

    while(pList->head) {
        
    }
}

/*
    用来初始化用户列表链表。
    链表为双链表，检索更快。
    创建一个头节点。
*/

void initUser(LIST_USER* pList) {
    pList->head = (USER*)malloc(sizeof(USER));
    pList->head->prev = NULL;
    pList->head->prev = NULL;
}

/*
    用来初始化快递列表链表。
    链表为双链表，检索更快。
    创建一个头节点。
*/

void initDelivery(LIST_DELIVERY* pList) {
    pList->head = (DELIVERY*)malloc(sizeof(DELIVERY));
    pList->head->prev = NULL;
    pList->head->next = NULL;
}

/*
    用来快速取件。
    主要思路是要用户输入快递取件码，
    和已注册用户的唯一校验码。
    先把快递取件码放到一个暂存的数组内，
    供之后在链表遍历的时候进行比较。
    对这个暂存的数组tempDid做好了用户的报错提示。
*/

void quickGet() {// 没写完
    printf("快速取件界面\n");
    char tempDid[MAX_ONLYNUMBER];
    int tempDidLen = 0;
    printf("%s请输入5位取件码:%s", COLOR_DISPLAY, COLOR_RESET);
    scanf("%s", tempDid);
    tempDid[MAX_ONLYNUMBER - 1] = '\0';
    tempDidLen = strlen(tempDid);
    if(tempDidLen != 5) {
        printf("%s取件码位数有误!请重试...%s\n", COLOR_ERROR, COLOR_RESET);
    }else {

    }

}

/*
    用来进行用户的注册。
    先malloc一个新用户节点。
    在三个while循环内分别对用户名，密码，校验码进行输入。
    之后进入添加节点的函数内。
*/

void signup(LIST_USER* pList) {
    USER* newUser = (USER*)malloc(sizeof(USER));
    char tempUserName[MAX_NAME];// 用户名数组
    char tempPassword[MAX_PASSWORD];// 密码数组
    char istempPassword[MAX_PASSWORD];// 确认密码数组
    char tempOnlyNumber[MAX_ONLYNUMBER];// 校验码数组
    char istempOnlyNumber[MAX_ONLYNUMBER];// 确认校验码数组
    int ispwRight = 0, isonRight = 0;// 确认密码，校验码是否成功
    int nameLen = 0, passwordLen = 0, onlyNumberLen = 0;// 用户名长度，密码长度, 校验码长度
    printf("%s\\== 欢迎来到注册界面 ==/%s\n", COLOR_WELCOME, COLOR_RESET);
    while(1) {// 用户名输入循环
        printf("%s请输入您的新用户名:(最大支持10位)%s", COLOR_DISPLAY, COLOR_RESET);
        scanf("%s",tempUserName);
        tempUserName[MAX_NAME - 1] = '\0';
        nameLen = strlen(tempUserName);
        if(nameLen > 10) {
            printf("%s您输入的用户名过长!请重试...%s\n", COLOR_ERROR, COLOR_RESET);
        }else {
            strcpy(newUser->name, tempUserName);
            break;
        }
    }

    while(1) {// 密码输入循环
        printf("%s请输入您的新密码:(最大支持25位)%s", COLOR_DISPLAY, COLOR_RESET);
        scanf("%s",tempPassword);
        tempPassword[MAX_PASSWORD - 1] = '\0';
        passwordLen = strlen(tempPassword);
        if(passwordLen > 25) {
            printf("%s您输入的密码过长!请重试...%s\n", COLOR_ERROR, COLOR_RESET);
        }else {
            while(1) {
                printf("%s请确认您的新密码:%s", COLOR_DISPLAY, COLOR_RESET);
                scanf("%s", istempPassword);
                istempPassword[MAX_PASSWORD - 1] = '\0';
                if(strcmp(tempPassword, istempPassword) == 0) {
                    printf("%s密码确认成功!%s", COLOR_DISPLAY, COLOR_RESET);
                    ispwRight = 1;
                    break;
                }else {
                    printf("%s您重新输入的密码有误!请重试...%s\n", COLOR_ERROR, COLOR_RESET);
                }
            }
            if(ispwRight) {
                strcpy(newUser->password, tempPassword);
                break;
            }
        }
    }

    while(1) {// 校验码输入循环
        printf("%s请输入您的唯一校验码:(最大支持6位)%s", COLOR_DISPLAY, COLOR_RESET);
        scanf("%s",tempOnlyNumber);
        tempOnlyNumber[MAX_ONLYNUMBER - 1] = '\0';
        onlyNumberLen = strlen(tempOnlyNumber);
        if(onlyNumberLen > 6) {
            printf("%s您输入的校验码过长!请重试...%s\n", COLOR_ERROR, COLOR_RESET);
        }else {
            while(1) {
                printf("%s请确认您的新校验码:%s", COLOR_DISPLAY, COLOR_RESET);
                scanf("%s", istempOnlyNumber);
                istempOnlyNumber[MAX_ONLYNUMBER - 1] = '\0';
                if(strcmp(istempOnlyNumber, istempOnlyNumber) == 0) {
                    printf("%s校验码确认成功!%s\n", COLOR_DISPLAY, COLOR_RESET);
                    isonRight = 1;
                    break;
                }else {
                    printf("%s您重新输入的校验码有误!请重试...%s\n", COLOR_ERROR, COLOR_RESET);
                }
            }
            if(isonRight) {
                strcpy(newUser->onlyNumber, tempOnlyNumber);
                break;
            }
        }
    }

    addUser(pList, newUser);
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

void menu(LIST_DELIVERY* pListDelivery, LIST_USER* pListUser) {
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
            signup(pListUser);
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