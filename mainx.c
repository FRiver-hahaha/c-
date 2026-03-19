#include "express_cabinet.h"

// ==================== 链表工具函数实现 ====================
// 创建用户节点
UserNode* create_user_node(const char* username, const char* password, const char* phone, int role) {
    UserNode *node = (UserNode*)malloc(sizeof(UserNode));
    if (node == NULL) return NULL;
    strcpy(node->username, username);
    strcpy(node->password, password);
    strcpy(node->phone, phone);
    node->role = role;
    strcpy(node->nickname, username); // 默认昵称=用户名
    strcpy(node->avatar, ""); // 默认无头像
    node->next = NULL;
    return node;
}

// 创建快递节点
ExpressNode* create_express_node(const char* express_num, const char* phone, int cabinet_num, const char* pick_code, const char* time, int status) {
    ExpressNode *node = (ExpressNode*)malloc(sizeof(ExpressNode));
    if (node == NULL) return NULL;
    strcpy(node->express_num, express_num);
    strcpy(node->recipient_phone, phone);
    node->cabinet_num = cabinet_num;
    strcpy(node->pick_code, pick_code);
    strcpy(node->store_time, time);
    node->status = status;
    node->next = NULL;
    return node;
}

// 创建柜子节点
CabinetNode* create_cabinet_node(int num, int type, int status) {
    CabinetNode *node = (CabinetNode*)malloc(sizeof(CabinetNode));
    if (node == NULL) return NULL;
    node->num = num;
    node->type = type;
    node->status = status;
    node->next = NULL;
    return node;
}

// 释放用户链表内存
void free_user_list(UserNode *head) {
    UserNode *tmp;
    while (head != NULL) {
        tmp = head;
        head = head->next;
        free(tmp);
    }
}

// 释放快递链表内存
void free_express_list(ExpressNode *head) {
    ExpressNode *tmp;
    while (head != NULL) {
        tmp = head;
        head = head->next;
        free(tmp);
    }
}

// 释放柜子链表内存
void free_cabinet_list(CabinetNode *head) {
    CabinetNode *tmp;
    while (head != NULL) {
        tmp = head;
        head = head->next;
        free(tmp);
    }
}

// ==================== 系统初始化 ====================

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
            for(int i = 0; i < 2; i++) {
                printf("%c",flag);
            }
            printf("%s%s%s", COLOR_WELCOME, wl2, COLOR_RESET);
            for(int i = 0; i < 2; i++) {
                printf("%c",flag);
                if(i == 1) {
                    printf("\n");
                }
            }
        }
        usleep(50000);
    };
}

void system_init() {
    // 检查用户文件，不存在则创建默认管理员
    FILE *fp = fopen("user.txt", "r");
    if (fp == NULL) {
        UserNode *admin = create_user_node("admin", "123456", "00000000000", 1);
        write_users_to_file(admin);
        free_user_list(admin);
        fclose(fp);
    } else {
        fclose(fp);
    }

    // 检查柜子文件，不存在则初始化50个柜子
    fp = fopen("cabinet.txt", "r");
    if (fp == NULL) {
        cabinet_init(50);
        fclose(fp);
    } else {
        fclose(fp);
    }

    // 检查快递文件，不存在则创建空文件
    fp = fopen("express.txt", "w");
    fclose(fp);

    printf("系统初始化完成！\n");
}

// ==================== 用户操作实现 ====================
// 从文件读取用户并构建链表
UserNode* read_users_from_file() {
    UserNode *head = NULL, *tail = NULL;
    FILE *fp = fopen("user.txt", "r");
    if (fp == NULL) return NULL;

    char line[100];
    while (fgets(line, sizeof(line), fp) != NULL) {
        // 分割行：用户名|密码|手机号|角色|昵称|头像
        char* token = strtok(line, "|");
        char username[20], password[20], phone[12];
        int role;
        strcpy(username, token);

        token = strtok(NULL, "|");
        strcpy(password, token);

        token = strtok(NULL, "|");
        strcpy(phone, token);

        token = strtok(NULL, "|");
        role = atoi(token);

        // 创建节点
        UserNode *node = create_user_node(username, password, phone, role);
        if (node == NULL) continue;

        // 插入链表尾部
        if (head == NULL) {
            head = node;
            tail = node;
        } else {
            tail->next = node;
            tail = node;
        }
    }
    fclose(fp);
    return head;
}

// 将用户链表写入文件
int write_users_to_file(UserNode *head) {
    FILE *fp = fopen("user.txt", "w");
    if (fp == NULL) return -1;

    UserNode *p = head;
    while (p != NULL) {
        fprintf(fp, "%s|%s|%s|%d|%s|%s\n",
                p->username, p->password, p->phone,
                p->role, p->nickname, p->avatar);
        p = p->next;
    }
    fclose(fp);
    return 0;
}

// 用户注册
int user_register(const char* username, const char* password, const char* phone) {
    // 读取现有用户链表
    UserNode *head = read_users_from_file();
    if (head == NULL) {
        // 无现有用户，直接创建新链表
        UserNode *new_node = create_user_node(username, password, phone, 0);
        write_users_to_file(new_node);
        free_user_list(new_node);
        return 0;
    }

    // 检查用户名/手机号是否重复
    UserNode *p = head;
    while (p != NULL) {
        if (strcmp(p->username, username) == 0 || strcmp(p->phone, phone) == 0) {
            free_user_list(head);
            return -1; // 重复
        }
        p = p->next;
    }

    // 插入新节点到链表尾部
    p = head;
    while (p->next != NULL) p = p->next;
    p->next = create_user_node(username, password, phone, 0);

    // 写入文件并释放内存
    int ret = write_users_to_file(head);
    free_user_list(head);
    return ret;
}

// 用户登录
int user_login(const char* username, const char* password, UserNode **login_node) {
    UserNode *head = read_users_from_file();
    if (head == NULL) return -1;

    UserNode *p = head;
    while (p != NULL) {
        if (strcmp(p->username, username) == 0 && strcmp(p->password, password) == 0) {
            // 复制登录节点（避免释放原链表后指针失效）
            *login_node = create_user_node(p->username, p->password, p->phone, p->role);
            free_user_list(head);
            return 0; // 登录成功
        }
        p = p->next;
    }

    free_user_list(head);
    return -1; // 登录失败
}

// ==================== 快递操作实现 ====================
// 生成6位取件码
void generate_pick_code(char* code) {
    srand((unsigned int)time(NULL));
    sprintf(code, "%06d", rand() % 999999);
}

// 获取当前时间
void get_current_time(char* time_str) {
    time_t now = time(NULL);
    struct tm* t = localtime(&now);
    sprintf(time_str, "%04d-%02d-%02d %02d:%02d:%02d",
            t->tm_year + 1900, t->tm_mon + 1, t->tm_mday,
            t->tm_hour, t->tm_min, t->tm_sec);
}

// 从文件读取快递并构建链表
ExpressNode* read_express_from_file() {
    ExpressNode *head = NULL, *tail = NULL;
    FILE *fp = fopen("express.txt", "r");
    if (fp == NULL) return NULL;

    char line[200];
    while (fgets(line, sizeof(line), fp) != NULL) {
        char* token = strtok(line, "|");
        char express_num[20], phone[12], pick_code[6], time[20];
        int cabinet_num, status;

        strcpy(express_num, token);
        token = strtok(NULL, "|");
        strcpy(phone, token);
        token = strtok(NULL, "|");
        cabinet_num = atoi(token);
        token = strtok(NULL, "|");
        strcpy(pick_code, token);
        token = strtok(NULL, "|");
        strcpy(time, token);
        token = strtok(NULL, "|");
        token[strcspn(token, "\n")] = '\0';
        status = atoi(token);

        ExpressNode *node = create_express_node(express_num, phone, cabinet_num, pick_code, time, status);
        if (node == NULL) continue;

        if (head == NULL) {
            head = node;
            tail = node;
        } else {
            tail->next = node;
            tail = node;
        }
    }
    fclose(fp);
    return head;
}

// 将快递链表写入文件
int write_express_to_file(ExpressNode *head) {
    FILE *fp = fopen("express.txt", "w");
    if (fp == NULL) return -1;

    ExpressNode *p = head;
    while (p != NULL) {
        fprintf(fp, "%s|%s|%d|%s|%s|%d\n",
                p->express_num, p->recipient_phone, p->cabinet_num,
                p->pick_code, p->store_time, p->status);
        p = p->next;
    }
    fclose(fp);
    return 0;
}

// 录入快递
int express_add(const char* express_num, const char* phone, int cabinet_num, const char* pick_code, const char* time, int status) {
    ExpressNode *head = read_express_from_file();

    // 检查单号是否重复
    ExpressNode *p = head;
    while (p != NULL) {
        if (strcmp(p->express_num, express_num) == 0) {
            free_express_list(head);
            return -1; // 重复
        }
        p = p->next;
    }

    // 插入新节点
    ExpressNode *new_node = create_express_node(express_num, phone, cabinet_num, pick_code, time, status);
    if (head == NULL) {
        head = new_node;
    } else {
        p = head;
        while (p->next != NULL) p = p->next;
        p->next = new_node;
    }

    // 写入文件并释放内存
    int ret = write_express_to_file(head);
    free_express_list(head);
    return ret;
}

// 取件验证（手机号后4位+取件码）
int express_query_pick(const char* phone_suffix, const char* pick_code, int* cabinet_num) {
    ExpressNode *head = read_express_from_file();
    if (head == NULL) return -1;

    ExpressNode *p = head;
    while (p != NULL) {
        // 取手机号后4位
        char suffix[5];
        strncpy(suffix, p->recipient_phone + 8, 4);
        suffix[4] = '\0';

        if (strcmp(suffix, phone_suffix) == 0 && strcmp(p->pick_code, pick_code) == 0 && p->status == 0) {
            *cabinet_num = p->cabinet_num;
            free_express_list(head);
            return 0; // 验证成功
        }
        p = p->next;
    }

    free_express_list(head);
    return -1; // 验证失败
}

// 查询用户快递（返回新链表）
void express_query_user(const char* phone, ExpressNode **result_head) {
    ExpressNode *head = read_express_from_file();
    if (head == NULL) {
        *result_head = NULL;
        return;
    }

    ExpressNode *p = head, *res_head = NULL, *res_tail = NULL;
    while (p != NULL) {
        if (strcmp(p->recipient_phone, phone) == 0) {
            // 复制节点到结果链表
            ExpressNode *node = create_express_node(p->express_num, p->recipient_phone, p->cabinet_num,
                                                    p->pick_code, p->store_time, p->status);
            if (res_head == NULL) {
                res_head = node;
                res_tail = node;
            } else {
                res_tail->next = node;
                res_tail = node;
            }
        }
        p = p->next;
    }

    free_express_list(head);
    *result_head = res_head;
}

// ==================== 柜子操作实现 ====================
// 初始化柜子（创建50个小柜，空状态）
void cabinet_init(int total) {
    CabinetNode *head = NULL, *tail = NULL;
    for (int i = 1; i <= total; i++) {
        CabinetNode *node = create_cabinet_node(i, 0, 0); // 柜号i，小柜，空
        if (head == NULL) {
            head = node;
            tail = node;
        } else {
            tail->next = node;
            tail = node;
        }
    }
    write_cabinets_to_file(head);
    free_cabinet_list(head);
}

// 从文件读取柜子并构建链表
CabinetNode* read_cabinets_from_file() {
    CabinetNode *head = NULL, *tail = NULL;
    FILE *fp = fopen("cabinet.txt", "r");
    if (fp == NULL) return NULL;

    char line[50];
    while (fgets(line, sizeof(line), fp) != NULL) {
        char* token = strtok(line, "|");
        int num = atoi(token);
        token = strtok(NULL, "|");
        int type = atoi(token);
        token = strtok(NULL, "|");
        token[strcspn(token, "\n")] = '\0';
        int status = atoi(token);

        CabinetNode *node = create_cabinet_node(num, type, status);
        if (node == NULL) continue;

        if (head == NULL) {
            head = node;
            tail = node;
        } else {
            tail->next = node;
            tail = node;
        }
    }
    fclose(fp);
    return head;
}

// 将柜子链表写入文件
int write_cabinets_to_file(CabinetNode *head) {
    FILE *fp = fopen("cabinet.txt", "w");
    if (fp == NULL) return -1;

    CabinetNode *p = head;
    while (p != NULL) {
        fprintf(fp, "%d|%d|%d\n", p->num, p->type, p->status);
        p = p->next;
    }
    fclose(fp);
    return 0;
}

// 获取空柜（指定柜型）
int cabinet_get_empty(int type, int* cabinet_num) {
    CabinetNode *head = read_cabinets_from_file();
    if (head == NULL) return -1;

    CabinetNode *p = head;
    while (p != NULL) {
        if (p->type == type && p->status == 0) {
            *cabinet_num = p->num;
            free_cabinet_list(head);
            return 0; // 找到空柜
        }
        p = p->next;
    }

    free_cabinet_list(head);
    return -1; // 无空柜
}

// 更新柜子状态
int cabinet_update_status(int cabinet_num, int status) {
    CabinetNode *head = read_cabinets_from_file();
    if (head == NULL) return -1;

    CabinetNode *p = head;
    while (p != NULL) {
        if (p->num == cabinet_num) {
            p->status = status;
            int ret = write_cabinets_to_file(head);
            free_cabinet_list(head);
            return ret;
        }
        p = p->next;
    }

    free_cabinet_list(head);
    return -1; // 柜号不存在
}

// 查看所有柜子状态
void cabinet_show_all() {
    CabinetNode *head = read_cabinets_from_file();
    if (head == NULL) {
        printf("暂无柜子数据！\n");
        return;
    }

    printf("柜号\t柜型\t状态\n");
    printf("------------------\n");
    CabinetNode *p = head;
    while (p != NULL) {
        char type[5], status[10];
        strcpy(type, p->type == 0 ? "小" : (p->type == 1 ? "中" : "大"));
        strcpy(status, p->status == 0 ? "空" : (p->status == 1 ? "占用" : "故障"));
        printf("%d\t%s\t%s\n", p->num, type, status);
        p = p->next;
    }

    free_cabinet_list(head);
}

// ==================== 菜单驱动 ====================
// 普通用户菜单
void user_menu(UserNode *login_node) {
    int choice;
    char phone_suffix[5], pick_code[6];
    int cabinet_num;
    ExpressNode *user_exps = NULL;

    while (1) {
        CLEAR_SCREEN();
        printf("===== 普通用户菜单（%s）=====\n", login_node->nickname);
        printf("1. 取件\n");
        printf("2. 查看我的快递\n");
        printf("0. 退出登录\n");
        printf("请选择操作：");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                printf("请输入手机号后4位：");
                scanf("%s", phone_suffix);
                printf("请输入6位取件码：");
                scanf("%s", pick_code);

                if (express_query_pick(phone_suffix, pick_code, &cabinet_num) == 0) {
                    printf("✅ 取件成功！柜号：%d\n", cabinet_num);

                    // 更新快递状态为已取
                    ExpressNode *head = read_express_from_file();
                    ExpressNode *p = head;
                    while (p != NULL) {
                        if (p->cabinet_num == cabinet_num && p->status == 0) {
                            p->status = 1;
                            break;
                        }
                        p = p->next;
                    }
                    write_express_to_file(head);
                    free_express_list(head);

                    // 更新柜子状态为空
                    cabinet_update_status(cabinet_num, 0);
                } else {
                    printf("❌ 取件失败！\n");
                }
                PAUSE();
                break;

            case 2:
                printf("===== 我的快递记录 =====\n");
                express_query_user(login_node->phone, &user_exps);
                if (user_exps == NULL) {
                    printf("暂无快递记录\n");
                } else {
                    printf("单号\t\t手机号\t\t柜号\t取件码\t状态\n");
                    printf("--------------------------------------------\n");
                    ExpressNode *p = user_exps;
                    while (p != NULL) {
                        char status[10];
                        strcpy(status, p->status == 0 ? "未取" : (p->status == 1 ? "已取" : "异常"));
                        printf("%s\t%s\t%d\t%s\t%s\n",
                               p->express_num, p->recipient_phone,
                               p->cabinet_num, p->pick_code, status);
                        p = p->next;
                    }
                }
                free_express_list(user_exps); // 释放查询结果链表
                PAUSE();
                break;

            case 0:
                printf("退出登录成功！\n");
                return;

            default:
                printf("输入错误，请重新选择！\n");
                PAUSE();
                break;
        }
    }
}

// 管理员菜单
void admin_menu(UserNode *login_node) {
    int choice, cabinet_num, type;
    char express_num[20], phone[12], pick_code[6], time_str[20];

    while (1) {
        CLEAR_SCREEN();
        printf("===== 管理员菜单（%s）=====\n", login_node->nickname);
        printf("1. 录入快递\n");
        printf("2. 柜子状态管理\n");
        printf("3. 查看所有快递\n");
        printf("0. 退出登录\n");
        printf("请选择操作：");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                printf("请输入快递单号：");
                scanf("%s", express_num);
                printf("请输入收件人手机号：");
                scanf("%s", phone);
                printf("选择柜型（0=小 1=中 2=大）：");
                scanf("%d", &type);

                if (cabinet_get_empty(type, &cabinet_num) == 0) {
                    generate_pick_code(pick_code);
                    get_current_time(time_str);

                    if (express_add(express_num, phone, cabinet_num, pick_code, time_str, 0) == 0) {
                        printf("✅ 快递录入成功！\n");
                        printf("柜号：%d | 取件码：%s | 时间：%s\n", cabinet_num, pick_code, time_str);
                        cabinet_update_status(cabinet_num, 1); // 标记柜子为占用
                    } else {
                        printf("❌ 快递录入失败（单号重复）！\n");
                    }
                } else {
                    printf("❌ 暂无空柜！\n");
                }
                PAUSE();
                break;

            case 2:
                printf("===== 柜子状态管理 =====\n");
                printf("1. 查看所有柜子\n");
                printf("2. 修改柜子状态\n");
                printf("请选择：");
                scanf("%d", &type);

                if (type == 1) {
                    cabinet_show_all();
                } else if (type == 2) {
                    printf("请输入柜号：");
                    scanf("%d", &cabinet_num);
                    printf("选择状态（0=空 1=占用 2=故障）：");
                    scanf("%d", &type);

                    if (cabinet_update_status(cabinet_num, type) == 0) {
                        printf("✅ 柜子状态更新成功！\n");
                    } else {
                        printf("❌ 柜号不存在！\n");
                    }
                }
                PAUSE();
                break;

            case 3:
                printf("===== 所有快递记录 =====\n");
                ExpressNode *head = read_express_from_file();
                if (head == NULL) {
                    printf("暂无快递记录\n");
                } else {
                    printf("单号\t\t手机号\t\t柜号\t取件码\t时间\t\t状态\n");
                    printf("------------------------------------------------------------\n");
                    ExpressNode *p = head;
                    while (p != NULL) {
                        char status[10];
                        strcpy(status, p->status == 0 ? "未取" : (p->status == 1 ? "已取" : "异常"));
                        printf("%s\t%s\t%d\t%s\t%s\t%s\n",
                               p->express_num, p->recipient_phone, p->cabinet_num,
                               p->pick_code, p->store_time, status);
                        p = p->next;
                    }
                }
                free_express_list(head);
                PAUSE();
                break;

            case 0:
                printf("退出登录成功！\n");
                return;

            default:
                printf("输入错误，请重新选择！\n");
                PAUSE();
                break;
        }
    }
}

// 主函数
int main() {
    system_init();

    int choice;
    UserNode *login_node = NULL;
    char username[20], password[20], phone[12];

    while (1) {
        CLEAR_SCREEN();
        firstShow();
        printf("===== 社区快递柜管理系统 V1.0（链表版）=====\n");
        printf("1. 登录\n");
        printf("2. 注册\n");
        printf("0. 退出系统\n");
        printf("请选择操作：");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                printf("请输入用户名：");
                scanf("%s", username);
                printf("请输入密码：");
                scanf("%s", password);

                if (user_login(username, password, &login_node) == 0) {
                    printf("✅ 登录成功！\n");
                    PAUSE();
                    if (login_node->role == 0) {
                        user_menu(login_node);
                    } else {
                        admin_menu(login_node);
                    }
                    free_user_list(login_node); // 释放登录节点内存
                    login_node = NULL;
                } else {
                    printf("❌ 用户名或密码错误！\n");
                    PAUSE();
                }
                break;

            case 2:
                printf("请输入用户名：");
                scanf("%s", username);
                printf("请输入密码：");
                scanf("%s", password);
                printf("请输入手机号：");
                scanf("%s", phone);

                if (user_register(username, password, phone) == 0) {
                    printf("✅ 注册成功！请登录\n");
                } else {
                    printf("❌ 注册失败（用户名/手机号重复）！\n");
                }
                PAUSE();
                break;

            case 0:
                printf("退出系统成功！\n");
                return 0;

            default:
                printf("输入错误，请重新选择！\n");
                PAUSE();
                break;
        }
    }

    return 0;
}