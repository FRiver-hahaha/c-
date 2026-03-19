#ifndef EXPRESS_CABINET_H
#define EXPRESS_CABINET_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>
#include <unistd.h>
#define COLOR_RESET "\033[0m" // 重置 
#define COLOR_WELCOME "\033[1;34m" // 欢迎界面(粗体蓝色)

// 跨平台宏定义
#ifdef _WIN32
#define CLEAR_SCREEN() system("cls")
#define PAUSE() system("pause")
#else
#define CLEAR_SCREEN() system("clear")
#define PAUSE() getchar()
#endif

// ==================== 链表节点结构体 ====================
// 用户节点
typedef struct UserNode {
    char username[20];    // 用户名
    char password[20];    // 密码
    char phone[12];       // 手机号
    int role;             // 0=普通用户，1=管理员
    char nickname[20];    // 昵称
    char avatar[50];      // 头像路径
    struct UserNode *next; // 下一个节点指针
} UserNode;

// 快递节点
typedef struct ExpressNode {
    char express_num[20]; // 快递单号
    char recipient_phone[12]; // 收件人手机号
    int cabinet_num;      // 柜号
    char pick_code[6];    // 取件码
    char store_time[20];  // 存放时间
    int status;           // 0=未取，1=已取，2=异常
    struct ExpressNode *next; // 下一个节点指针
} ExpressNode;

// 柜子节点
typedef struct CabinetNode {
    int num;              // 柜号
    int type;             // 0=小，1=中，2=大
    int status;           // 0=空，1=占用，2=故障
    struct CabinetNode *next; // 下一个节点指针
} CabinetNode;

// ==================== 函数声明 ====================
// 系统初始化
void system_init();

// 链表通用操作（工具函数）
UserNode* create_user_node(const char* username, const char* password, const char* phone, int role);
ExpressNode* create_express_node(const char* express_num, const char* phone, int cabinet_num, const char* pick_code, const char* time, int status);
CabinetNode* create_cabinet_node(int num, int type, int status);
void free_user_list(UserNode *head); // 释放用户链表内存
void free_express_list(ExpressNode *head); // 释放快递链表内存
void free_cabinet_list(CabinetNode *head); // 释放柜子链表内存

// 用户操作
int user_register(const char* username, const char* password, const char* phone); // 注册
int user_login(const char* username, const char* password, UserNode **login_node); // 登录（返回节点指针）
UserNode* read_users_from_file(); // 从文件读取用户并构建链表
int write_users_to_file(UserNode *head); // 将用户链表写入文件

// 快递操作
int express_add(const char* express_num, const char* phone, int cabinet_num, const char* pick_code, const char* time, int status);
int express_query_pick(const char* phone_suffix, const char* pick_code, int* cabinet_num); // 取件验证
ExpressNode* read_express_from_file(); // 从文件读取快递并构建链表
int write_express_to_file(ExpressNode *head); // 将快递链表写入文件
void express_query_user(const char* phone, ExpressNode **result_head); // 查询用户快递（返回新链表）

// 柜子操作
void cabinet_init(int total); // 初始化柜子（创建链表并写入文件）
int cabinet_get_empty(int type, int* cabinet_num); // 获取空柜
int cabinet_update_status(int cabinet_num, int status); // 更新柜子状态
CabinetNode* read_cabinets_from_file(); // 从文件读取柜子并构建链表
int write_cabinets_to_file(CabinetNode *head); // 将柜子链表写入文件
void cabinet_show_all(); // 查看所有柜子

// 工具函数
void generate_pick_code(char* code); // 生成6位取件码
void get_current_time(char* time_str); // 获取当前时间

#endif