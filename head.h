#ifndef HEAD
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

typedef struct _user {// 用户
    char name[MAX_NAME];// 用户名
    char password[MAX_PASSWORD];// 密码
    char onlyNumber[MAX_ONLYNUMBER];// 唯一6位校验码
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
    char userON[MAX_ONLYNUMBER];// 确定被哪个唯一校验码取走
    char userName[MAX_NAME];// 确定被哪位用户取走
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

void firstShow();// 欢迎界面(1)
void firstMenu(LIST_DELIVERY* pListDelivery, LIST_USER* pListUser, LIST_ADMIN* pListAdmin);// 欢迎菜单显示(1)
void userMenu(LIST_DELIVERY* pListDelivery, LIST_USER* pListUser, LIST_ADMIN* pListAdmin);// 用户菜单显示(1)
void adminMenu(LIST_DELIVERY* pListDelivery);// 管理员菜单显示(1)
int signin(LIST_USER* pList);// 登录(1)
void signup(LIST_USER* pList);// 注册(1)
void quickGet();// 快速取件(0)
void readMeUsing();// 使用说明(1)
void aboutUs();// 关于我们(1)
void exitFirst(LIST_USER* pListUser, LIST_DELIVERY* pListDelivery, LIST_ADMIN* pListAdmin);// 退出程序(1)
int aboutAdmin(LIST_ADMIN* pList);// 管理员相关(1)
char handleButton1(char button1);// 处理大小写问题(1)

// 有关用户注册的函数(启动前)
void initUser(LIST_USER* pList);// 初始化链表(1)
USER* createUserNode(char* userName, char* passWord, char* onlyNumber, int isVip, int isSignup);// 创建用户节点(1)
void addUser(LIST_USER* pList, USER* Node);// 增加用户节点(1)
void deleteUser(LIST_USER* pList);// 删除用户节点(1)

// 有关快递的函数(管理员操作)
void initDelivery(LIST_DELIVERY* pList);// 初始化链表(1)
DELIVERY* createDeliveryNode(char* Did, char* userON, char* userName, int status);// 创建快递节点(1)
void addDelivery(LIST_DELIVERY* pList, DELIVERY* Node);// 增加快递节点(1)
void deleteDelivery(LIST_DELIVERY* pList);// 删除快递节点(1)
void removeDelivery(LIST_DELIVERY* pList, char* tempDid);// 取走快递节点(0)

// 有关管理员(快递员)的函数
int signinAdmin(LIST_ADMIN* pList);// 管理员登录(1)
int signupAdmin(LIST_ADMIN* pList);// 管理员注册(1)
void initAdmin(LIST_ADMIN* pList);// 初始化链表(1)
ADMIN* createAdminNode(char* userName, char* passWord);// 创建管理员节点(1)
void addAdmin(LIST_ADMIN* pList, ADMIN* Node);// 增加管理员节点(1)
void deleteAdmin(LIST_ADMIN* pList);// 删除管理员节点(1)
#endif