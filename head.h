#ifndef HEAD_H
#define HEAD_H

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include <time.h>

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
    time_t regTime;// 注册时间
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
    time_t addTime;// 添加时间
    struct _delivery* prev;
    struct _delivery* next;
}DELIVERY;

typedef struct {// 用户链表
    USER* head;// 头节点
}LIST_USER;

typedef struct {// 管理员链表
    ADMIN* head;// 头节点
}LIST_ADMIN;

typedef struct {// 快递链表
    DELIVERY* head;// 头节点
}LIST_DELIVERY;

// 统计相关结构体
typedef struct {// 快递统计结构体
    int totalDeliveries;        // 总快递数
    int pickedDeliveries;       // 已取件数
    int waitingDeliveries;      // 待取件数
    int totalUsers;             // 总用户数
    int registeredUsers;        // 注册用户数
    int adminCount;             // 管理员数量
    double pickRate;            // 取件率
    time_t oldestDelivery;      // 最早未取快递时间
    time_t newestDelivery;      // 最新快递时间
    char mostActiveUser[MAX_NAME];  // 快递最多的用户
    int mostActiveUserCount;    // 该用户的快递数
}STATISTICS;

typedef struct {// 用户快递统计结构体
    char userName[MAX_NAME];
    char userON[MAX_ONLYNUMBER];
    int deliveryCount;
    int pickedCount;
    int waitingCount;
    time_t lastDeliveryTime;
}USER_DELIVERY_STAT;

typedef struct {// 时间统计结构体
    int lastHour;       // 最近1小时
    int lastDay;        // 最近24小时
    int lastWeek;       // 最近7天
    int lastMonth;      // 最近30天
    int older;          // 更早的
}TIME_STAT;

void firstShow();// 欢迎界面(1)
void firstMenu(LIST_DELIVERY* pListDelivery, LIST_USER* pListUser, LIST_ADMIN* pListAdmin);// 欢迎菜单显示(1)
void userMenu(LIST_DELIVERY* pListDelivery, LIST_USER* pListUser, LIST_ADMIN* pListAdmin, USER* currentUser);// 用户菜单显示(1)
void adminMenu(LIST_DELIVERY* pListDelivery, LIST_USER* pListUser, LIST_ADMIN* pListAdmin);// 管理员菜单显示(1)
int signin(LIST_USER* pList, USER** currentUser);// 登录(1)
void signup(LIST_USER* pList);// 注册(1)
void quickGet(LIST_DELIVERY* pListDelivery, LIST_USER* pListUser);// 快速取件(1)
void readMeUsing();// 使用说明(1)
void aboutUs();// 关于我们(1)
void exitFirst(LIST_USER* pListUser, LIST_DELIVERY* pListDelivery, LIST_ADMIN* pListAdmin);// 退出程序(1)
int aboutAdmin(LIST_ADMIN* pList);// 管理员相关(1)
char handleButton1(char button1);// 处理大小写问题(1)

// 有关用户注册的函数(启动前)
void initUser(LIST_USER* pList);// 初始化链表(1)
USER* createUserNode(char* userName, char* passWord, char* onlyNumber, int isSignup);// 创建用户节点(1)
void addUser(LIST_USER* pList, USER* Node);// 增加用户节点(1)
void deleteUser(LIST_USER* pList);// 删除用户节点(1)
void saveAllUsers(LIST_USER* pList);// 保存所有用户到文件(1)

// 有关快递的函数(管理员操作)
void initDelivery(LIST_DELIVERY* pList);// 初始化链表(1)
DELIVERY* createDeliveryNode(char* Did, char* userON, char* userName, int status);// 创建快递节点(1)
void addDelivery(LIST_DELIVERY* pList, DELIVERY* Node);// 增加快递节点(1)
void deleteDelivery(LIST_DELIVERY* pList);// 删除快递节点(1)
void removeDelivery(LIST_DELIVERY* pList, char* tempDid, char* userON);// 取走快递节点(1)
void saveAllDeliveries(LIST_DELIVERY* pList);// 保存所有快递到文件(1)
int getDeliveryCountByUser(LIST_DELIVERY* pList, char* userON);// 获取用户可取快递数量(1)
void sortDeliveriesByTime(LIST_DELIVERY* pList);// 按添加时间排序(1)
void sortDeliveriesByCode(LIST_DELIVERY* pList);// 按取件码排序(1)
void showUserDeliveries(LIST_DELIVERY* pList, char* userON);// 显示用户快递(1)

// 有关管理员(快递员)的函数
int signinAdmin(LIST_ADMIN* pList);// 管理员登录(1)
int signupAdmin(LIST_ADMIN* pList);// 管理员注册(1)
void initAdmin(LIST_ADMIN* pList);// 初始化链表(1)
ADMIN* createAdminNode(char* userName, char* passWord);// 创建管理员节点(1)
void addAdmin(LIST_ADMIN* pList, ADMIN* Node);// 增加管理员节点(1)
void deleteAdmin(LIST_ADMIN* pList);// 删除管理员节点(1)

// 统计相关函数
void collectStatistics(LIST_DELIVERY* pListDelivery, LIST_USER* pListUser, 
                       LIST_ADMIN* pListAdmin, STATISTICS* stats, TIME_STAT* timeStat);// 收集统计数据(1)
void simpleStatistics(LIST_DELIVERY* pListDelivery, LIST_USER* pListUser, 
                     LIST_ADMIN* pListAdmin);// 简单统计(1)
void comprehensiveStatistics(LIST_DELIVERY* pListDelivery, LIST_USER* pListUser, 
                            LIST_ADMIN* pListAdmin);// 综合统计(1)
void exportStatisticsToFile(LIST_DELIVERY* pListDelivery, LIST_USER* pListUser, 
                           LIST_ADMIN* pListAdmin);// 导出统计报表(1)
void printStatisticsReport(STATISTICS* stats, TIME_STAT* timeStat, 
                          USER_DELIVERY_STAT* topUsers, int topUserCount);// 打印统计报表(1)

#endif