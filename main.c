#include "head.h"

int main() {
    LIST_DELIVERY listDelivery;
    LIST_USER listUser;
    LIST_ADMIN listAdmin;
    initDelivery(&listDelivery);
    initAdmin(&listAdmin);
    initUser(&listUser);
    firstShow();
    firstMenu(&listDelivery, &listUser, &listAdmin);
    
    return 0;
}

/*
    管理员登录函数。
    通过遍历链表，匹配用户名和密码。
    如果匹配成功，返回1，否则返回0。
*/

int signinAdmin(LIST_ADMIN* pList) {
    char tempAdminName[MAX_NAME];
    char tempPassword[MAX_PASSWORD];
    int canSignin = 0;
    
    printf("%s\\== 管理员登录界面 ==/%s\n", COLOR_WELCOME, COLOR_RESET);
    printf("%s请输入管理员用户名:%s", COLOR_DISPLAY, COLOR_RESET);
    scanf("%s", tempAdminName);
    printf("%s请输入管理员密码:%s", COLOR_DISPLAY, COLOR_RESET);
    scanf("%s", tempPassword);
    tempPassword[MAX_PASSWORD - 1] = '\0';
    for(ADMIN* tempNode = pList->head->next; tempNode; tempNode = tempNode->next) {
        if(strcmp(tempNode->name, tempAdminName) == 0 && 
           strcmp(tempNode->password, tempPassword) == 0) {
            canSignin = 1;
            break;
        }
    }
    
    if(canSignin) {
        printf("%s管理员登录成功!%s\n", COLOR_WELCOME, COLOR_RESET);
        return 1;
    } else {
        printf("%s用户名或密码错误!%s\n", COLOR_ERROR, COLOR_RESET);
        return 0;
    }
}

/*
    用来删除管理员链表。
*/

void deleteAdmin(LIST_ADMIN* pList) {
    ADMIN* tempNode;
    while(pList->head) {
        tempNode = pList->head;
        pList->head = pList->head->next;
        free(tempNode);
    }
}

/*
    用来将管理员信息写入文件内。
    不仅仅把管理员信息写入文件，
    还需要把当前管理员信息，
    和程序所存在的链表连起来。
*/

void addAdmin(LIST_ADMIN* pList, ADMIN* Node) {
    FILE* fd = fopen("admin.txt", "a");
    ADMIN* tempNode = pList->head;
    Node->prev = NULL;
    Node->next = NULL;
    if(pList->head->next == NULL) {// 检查头节点
        pList->head->next = Node;
        Node->prev = pList->head;
    }else {// 头节点下有内容
        for(tempNode = pList->head->next; tempNode; tempNode = tempNode->next) {
            if(tempNode->next == NULL) {
                tempNode->next = Node;
                Node->prev = tempNode;
                break;
            }
        }
    }
    // 写入文件
    fprintf(fd, "%s|%s\n",Node->name, Node->password);
    printf("%s管理员注册成功!%s\n", COLOR_DISPLAY, COLOR_RESET);
    fclose(fd);
    return;
}

/*
    用来创建管理员链表节点。
    创建好的节点，
    返回值即为当前节点。
*/

ADMIN* createAdminNode(char* userName, char* passWord) {
    ADMIN* adminNode = (ADMIN*)malloc(sizeof(ADMIN));
    adminNode->prev = NULL;
    adminNode->next = NULL;
    strcpy(adminNode->name, userName);
    strcpy(adminNode->password, passWord);
    return adminNode;
}

/*
    用来初始化管理员列表链表。
    链表为双链表，检索更快。
    创建一个头节点。
*/

void initAdmin(LIST_ADMIN* pList) {
    FILE* fd = fopen("admin.txt", "r");
    if(fd == NULL) {
        pList->head = (ADMIN*)malloc(sizeof(ADMIN));
        pList->head->prev = NULL;
        pList->head->next = NULL;
        return;
    }
    
    pList->head = (ADMIN*)malloc(sizeof(ADMIN));
    pList->head->prev = NULL;
    pList->head->next = NULL;
    
    char line[100];
    while (fgets(line, sizeof(line), fd) != NULL) {
        // 移除换行符
        line[strcspn(line, "\n")] = 0;
        
        // 分割行：用户名|密码
        char* token = strtok(line, "|");
        char userName[MAX_NAME];
        strcpy(userName, token);
        token = strtok(NULL, "|");
        char passWord[MAX_PASSWORD];
        strcpy(passWord, token);
        
        // 创建节点
        ADMIN* Node = createAdminNode(userName, passWord);
        
        // 插入链表尾部
        ADMIN* tempNode = pList->head;
        while(tempNode->next != NULL) {
            tempNode = tempNode->next;
        }
        tempNode->next = Node;
        Node->prev = tempNode;
    }
    fclose(fd);
}

/*
    用来删除快递链表。
*/

void deleteDelivery(LIST_DELIVERY* pList) {
    DELIVERY* tempNode;
    while(pList->head) {
        tempNode = pList->head;
        pList->head = pList->head->next;
        free(tempNode);
    }
}

/*
    用来删除用户链表。
*/

void deleteUser(LIST_USER* pList) {
    USER* tempNode;
    while(pList->head) {
        tempNode = pList->head;
        pList->head = pList->head->next;
        free(tempNode);
    }
}

/*
    用来将快递信息写入文件内。
    不仅仅把快递信息写入文件，
    还需要把当前快递信息，
    和程序所存在的链表连起来。
*/

void addDelivery(LIST_DELIVERY* pList, DELIVERY* Node) {
    DELIVERY* tempNode = pList->head;
    Node->prev = NULL;
    Node->next = NULL;
    Node->status = 0;// 初始状态为未取走
    Node->addTime = time(NULL);// 记录添加时间
    
    if(pList->head->next == NULL) {// 检查头节点
        pList->head->next = Node;
        Node->prev = pList->head;
    }else {// 头节点下有内容
        for(tempNode = pList->head->next; tempNode; tempNode = tempNode->next) {
            if(tempNode->next == NULL) {
                tempNode->next = Node;
                Node->prev = tempNode;
                break;
            }
        }
    }
    
    // 保存所有快递到文件
    saveAllDeliveries(pList);
    printf("%s快递存件成功!%s\n", COLOR_DISPLAY, COLOR_RESET);
    return;
}

/*
    用来保存所有用户信息到文件。
    遍历整个用户链表，将所有用户信息写入文件。
*/

void saveAllUsers(LIST_USER* pList) {
    FILE* fd = fopen("user.txt", "w");
    for(USER* tempNode = pList->head->next; tempNode; tempNode = tempNode->next) {
        fprintf(fd, "%s|%s|%s|%d\n", tempNode->name, tempNode->password, 
                tempNode->onlyNumber, tempNode->issignup);
    }
    fclose(fd);
}

/*
    用来将用户信息写入文件内。
    不仅仅把用户信息写入文件，
    还需要把当前用户信息，
    和程序所存在的链表连起来。
*/

void addUser(LIST_USER* pList, USER* Node) {
    USER* tempNode = pList->head;
    Node->prev = NULL;
    Node->next = NULL;
    Node->issignup = 1;// 标记为已注册用户
    Node->regTime = time(NULL);// 记录注册时间
    
    if(pList->head->next == NULL) {// 检查头节点
        pList->head->next = Node;
        Node->prev = pList->head;
    }else {// 头节点下有内容
        for(tempNode = pList->head->next; tempNode; tempNode = tempNode->next) {
            if(tempNode->next == NULL) {
                tempNode->next = Node;
                Node->prev = tempNode;
                break;
            }
        }
    }
    
    // 保存所有用户到文件
    saveAllUsers(pList);
    printf("%s用户注册成功!%s\n", COLOR_DISPLAY, COLOR_RESET);
    return;
}

/*
    用来初始化用户列表链表。
    链表为双链表，检索更快。
    创建一个头节点。
*/

void initUser(LIST_USER* pList) {
    FILE* fd = fopen("user.txt", "r");
    pList->head = (USER*)malloc(sizeof(USER));
    pList->head->prev = NULL;
    pList->head->next = NULL;
    
    if(fd == NULL) return;
    
    char line[100];
    while (fgets(line, sizeof(line), fd) != NULL) {
        // 分割行：用户名|密码|唯一校验码|是否为vip用户|是否为已注册用户
        char* token = strtok(line, "|");
        char userName[MAX_NAME], passWord[MAX_PASSWORD], onlyNumber[MAX_ONLYNUMBER];
        int isSignup = 0;
        
        strcpy(userName, token);
        token = strtok(NULL, "|");
        strcpy(passWord, token);
        token = strtok(NULL, "|");
        strcpy(onlyNumber, token);
        token = strtok(NULL, "|");
        if(token != NULL) isSignup = atoi(token);
        
        // 创建节点
        USER* Node = createUserNode(userName, passWord, onlyNumber, isSignup);
        
        // 插入链表尾部
        USER* tail = pList->head;
        while (tail->next != NULL) {
            tail = tail->next;
        }
        tail->next = Node;
        Node->prev = tail;
    }
    fclose(fd);
}

/*
    用来创建快递链表节点。
    创建好的节点，
    返回值即为当前节点。
*/

DELIVERY* createDeliveryNode(char* Did, char* userON, char* userName, int status) {
    DELIVERY* deliveryNode = (DELIVERY*)malloc(sizeof(DELIVERY));
    deliveryNode->prev = NULL;
    deliveryNode->next = NULL;
    strcpy(deliveryNode->Did, Did);
    strcpy(deliveryNode->userON, userON);
    strcpy(deliveryNode->userName, userName);
    deliveryNode->status = status;
    deliveryNode->addTime = time(NULL);
    return deliveryNode;
}

/*
    用来创建用户链表节点。
    创建好的节点，
    返回值即为当前节点。
*/

USER* createUserNode(char* userName, char* passWord, char* onlyNumber, int isSignup) {
    USER* userNode = (USER*)malloc(sizeof(USER));
    userNode->prev = NULL;
    userNode->next = NULL;
    strcpy(userNode->name, userName);
    strcpy(userNode->password, passWord);
    strcpy(userNode->onlyNumber, onlyNumber);
    userNode->issignup = isSignup;
    userNode->regTime = time(NULL);
    return userNode;
}

/*
    用来初始化快递列表链表。
    链表为双链表，检索更快。
    创建一个头节点。
*/

void initDelivery(LIST_DELIVERY* pList) {
    FILE* fd = fopen("delivery.txt", "r");
    pList->head = (DELIVERY*)malloc(sizeof(DELIVERY));
    pList->head->prev = NULL;
    pList->head->next = NULL;
    
    if(fd == NULL) return;
    
    char line[200];
    while (fgets(line, sizeof(line), fd) != NULL) {
        // 分割行：唯一5位取件码|唯一校验码|用户名|是否被取走
        char* token = strtok(line, "|");
        char Did[MAX_ONLYNUMBER], userON[MAX_ONLYNUMBER], userName[MAX_NAME];
        int status;
        strcpy(Did, token);
        token = strtok(NULL, "|");
        strcpy(userON, token);
        token = strtok(NULL, "|");
        strcpy(userName, token);
        token = strtok(NULL, "|");
        status = atoi(token);
        
        // 创建节点
        DELIVERY* Node = createDeliveryNode(Did, userON, userName, status);
        
        // 插入链表尾部
        DELIVERY* tail = pList->head;
        while (tail->next != NULL) {
            tail = tail->next;
        }
        tail->next = Node;
        Node->prev = tail;
    }
    fclose(fd);
}

/*
    用来保存所有快递信息到文件。
    遍历整个快递链表，将未被取走的快递信息写入文件。
*/

void saveAllDeliveries(LIST_DELIVERY* pList) {
    FILE* fd = fopen("delivery.txt", "w");
    for(DELIVERY* tempNode = pList->head->next; tempNode; tempNode = tempNode->next) {
        if(tempNode->status == 0) {// 只保存未被取走的快递
            fprintf(fd, "%s|%s|%s|%d\n", tempNode->Did, tempNode->userON, 
                    tempNode->userName, tempNode->status);
        }
    }
    fclose(fd);
}

/*
    用来快速取件。
    主要思路是要用户输入快递取件码，
    和已注册用户的唯一校验码。
    先把快递取件码放到一个暂存的数组内，
    供之后在链表遍历的时候进行比较。
    对这个暂存的数组tempDid做好了用户的报错提示。
*/

void quickGet(LIST_DELIVERY* pListDelivery, LIST_USER* pListUser) {
    char tempDid[MAX_ONLYNUMBER];
    char tempUserON[MAX_ONLYNUMBER];
    int found = 0;
    
    printf("%s\\== 快速取件界面 ==/%s\n", COLOR_WELCOME, COLOR_RESET);
    printf("%s请输入5位取件码:%s", COLOR_DISPLAY, COLOR_RESET);
    scanf("%s", tempDid);
    tempDid[MAX_ONLYNUMBER - 1] = '\0';
    
    if(strlen(tempDid) != 5) {
        printf("%s取件码位数有误!请重试...%s\n", COLOR_ERROR, COLOR_RESET);
        return;
    }
    
    printf("%s请输入您的6位唯一校验码:%s", COLOR_DISPLAY, COLOR_RESET);
    scanf("%s", tempUserON);
    tempUserON[MAX_ONLYNUMBER - 1] = '\0';
    
    if(strlen(tempUserON) != 6) {
        printf("%s校验码位数有误!请重试...%s\n", COLOR_ERROR, COLOR_RESET);
        return;
    }
    
    // 查找用户是否存在
    USER* currentUser = NULL;
    for(USER* tempUser = pListUser->head->next; tempUser; tempUser = tempUser->next) {
        if(strcmp(tempUser->onlyNumber, tempUserON) == 0) {
            currentUser = tempUser;
            break;
        }
    }
    
    if(!currentUser) {
        printf("%s未找到该用户!请检查校验码...%s\n", COLOR_ERROR, COLOR_RESET);
        return;
    }
    
    // 查找快递并取件
    for(DELIVERY* tempDelivery = pListDelivery->head->next; tempDelivery; tempDelivery = tempDelivery->next) {
        if(strcmp(tempDelivery->Did, tempDid) == 0 && 
           strcmp(tempDelivery->userON, tempUserON) == 0 && 
           tempDelivery->status == 0) {
            printf("%s取件成功!请取走包裹.%s\n", COLOR_DISPLAY, COLOR_RESET);
            removeDelivery(pListDelivery, tempDid, tempUserON);
            found = 1;
            break;
        }
    }
    
    if(!found) {
        printf("%s暂无该包裹或取件码与校验码不匹配!请检查后重试...%s\n", COLOR_ERROR, COLOR_RESET);
    }
}

/*
    用来进行用户的登录。
    创建两个暂存的数组，
    用来将来与库内文件进行比较。
    在一个for循环内进行检查，
    如果确实匹配到了，则登陆成功，
    否则要求用户重新输入。
*/

int signin(LIST_USER* pList, USER** currentUser) {
    char tempUserName[MAX_NAME];// 用户名数组
    char tempPassword[MAX_PASSWORD];// 密码数组
    int canSignin = 0;
    
    printf("%s\\== 欢迎来到登录界面 ==/%s\n", COLOR_WELCOME, COLOR_RESET);
    printf("%s请输入您的用户名:(最大支持10位)%s", COLOR_DISPLAY, COLOR_RESET);
    scanf("%s", tempUserName);
    tempUserName[MAX_NAME - 1] = '\0';
    printf("%s请输入您的密码:(最大支持25位)%s", COLOR_DISPLAY, COLOR_RESET);
    scanf("%s", tempPassword);
    tempPassword[MAX_PASSWORD - 1] = '\0';
    
    for(USER* tempNode = pList->head->next; tempNode; tempNode = tempNode->next) {
        if(strcmp(tempNode->name, tempUserName) == 0 && 
           strcmp(tempNode->password, tempPassword) == 0) {
            canSignin = 1;
            *currentUser = tempNode;// 保存当前登录的用户信息
            break;
        }
    }
    
    if(canSignin) {
        printf("%s\\== 登陆成功! ==/%s\n", COLOR_WELCOME, COLOR_RESET);
        return 1;
    } else {
        printf("%s您输入的用户名或密码有误!请重试...%s\n", COLOR_ERROR, COLOR_RESET);
        return 0;
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
        scanf("%s", tempUserName);
        tempUserName[MAX_NAME - 1] = '\0';
        nameLen = strlen(tempUserName);
        if(nameLen > 10) {
            printf("%s您输入的用户名过长!请重试...%s\n", COLOR_ERROR, COLOR_RESET);
        } else {
            int nameExists = 0;
            for(USER* tempNode = pList->head->next; tempNode; tempNode = tempNode->next) {
                if(strcmp(tempNode->name, tempUserName) == 0) {
                    printf("%s您输入的用户名已被注册过!请重试...%s\n", COLOR_ERROR, COLOR_RESET);
                    nameExists = 1;
                    break;
                }
            }
            if(!nameExists) {
                strcpy(newUser->name, tempUserName);
                break;
            }
        }
    }
    
    while(1) {// 密码输入循环
        printf("%s请输入您的新密码:(最大支持25位)%s", COLOR_DISPLAY, COLOR_RESET);
        scanf("%s", tempPassword);
        tempPassword[MAX_PASSWORD - 1] = '\0';
        passwordLen = strlen(tempPassword);
        if(passwordLen > 25) {
            printf("%s您输入的密码过长!请重试...%s\n", COLOR_ERROR, COLOR_RESET);
        } else {
            while(1) {
                printf("%s请确认您的新密码:%s", COLOR_DISPLAY, COLOR_RESET);
                scanf("%s", istempPassword);
                istempPassword[MAX_PASSWORD - 1] = '\0';
                if(strcmp(tempPassword, istempPassword) == 0) {
                    printf("%s密码确认成功!%s\n", COLOR_DISPLAY, COLOR_RESET);
                    ispwRight = 1;
                    break;
                } else {
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
        printf("%s请输入您的唯一校验码:(6位)%s", COLOR_DISPLAY, COLOR_RESET);
        scanf("%s", tempOnlyNumber);
        tempOnlyNumber[MAX_ONLYNUMBER - 1] = '\0';
        onlyNumberLen = strlen(tempOnlyNumber);
        if(onlyNumberLen != 6) {
            printf("%s您输入的校验码不符合规定!请重试...%s\n", COLOR_ERROR, COLOR_RESET);
        } else {
            int codeExists = 0;
            for(USER* tempNode = pList->head->next; tempNode; tempNode = tempNode->next) {
                if(strcmp(tempNode->onlyNumber, tempOnlyNumber) == 0) {
                    printf("%s该校验码已被注册!请重试...%s\n", COLOR_ERROR, COLOR_RESET);
                    codeExists = 1;
                    break;
                }
            }
            if(!codeExists) {
                while(1) {
                    printf("%s请确认您的新校验码:%s", COLOR_DISPLAY, COLOR_RESET);
                    scanf("%s", istempOnlyNumber);
                    istempOnlyNumber[MAX_ONLYNUMBER - 1] = '\0';
                    if(strcmp(tempOnlyNumber, istempOnlyNumber) == 0) {
                        printf("%s校验码确认成功!%s\n", COLOR_DISPLAY, COLOR_RESET);
                        isonRight = 1;
                        break;
                    } else {
                        printf("%s您重新输入的校验码有误!请重试...%s\n", COLOR_ERROR, COLOR_RESET);
                    }
                }
                if(isonRight) {
                    strcpy(newUser->onlyNumber, tempOnlyNumber);
                    break;
                }
            }
        }
    }
    
    addUser(pList, newUser);
}

/*
    用来退出应用。
    简单的询问以及switch。
*/

void exitFirst(LIST_USER* pListUser, LIST_DELIVERY* pListDelivery, LIST_ADMIN* pListAdmin) {
    char button1;
    printf("%s您确定要退出吗?%s yes/no (%sy%s/%sn%s):", COLOR_DISPLAY, COLOR_RESET, COLOR_DISPLAY, COLOR_RESET, COLOR_DISPLAY, COLOR_RESET);
    scanf(" %c", &button1);
    button1 = handleButton1(button1);
    switch (button1) {
        case 'y':
            saveAllUsers(pListUser);// 退出前保存所有用户信息
            saveAllDeliveries(pListDelivery);// 退出前保存所有快递信息
            deleteUser(pListUser);
            deleteDelivery(pListDelivery);
            deleteAdmin(pListAdmin);
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
    printf("\n按任意键回车后退出...\n");
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
        return;
    }
    while((readCnt = read(fd, buf, 1)) > 0) {
        buf[1] = '\0';
        printf("%s", buf);
    }
    close(fd);
    printf("\n按任意键回车后退出...\n");
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
    用来从链表中移除已取走的快递。
    同时将取件记录写入历史文件。
*/

void removeDelivery(LIST_DELIVERY* pList, char* tempDid, char* userON) {
    FILE* fd = fopen("log.txt", "a");
    DELIVERY* toRemove = NULL;
    char userName[MAX_NAME] = "";
    
    // 查找要移除的快递节点
    for(DELIVERY* tempNode = pList->head->next; tempNode; tempNode = tempNode->next) {
        if(strcmp(tempNode->Did, tempDid) == 0 && strcmp(tempNode->userON, userON) == 0) {
            toRemove = tempNode;
            strcpy(userName, tempNode->userName);
            break;
        }
    }
    
    if(toRemove && toRemove->status == 0) {
        toRemove->status = 1;// 标记为已取走
        
        // 写入历史记录，包含详细时间戳
        time_t now = time(NULL);
        struct tm* t = localtime(&now);
        fprintf(fd, "[%04d-%02d-%02d %02d:%02d:%02d] 取件码[%s] 已被用户[%s](校验码:%s)取走.\n",
                t->tm_year + 1900, t->tm_mon + 1, t->tm_mday,
                t->tm_hour, t->tm_min, t->tm_sec,
                toRemove->Did, toRemove->userName, toRemove->userON);
        
        // 从链表中移除节点
        if(toRemove->prev) {
            toRemove->prev->next = toRemove->next;
        }
        if(toRemove->next) {
            toRemove->next->prev = toRemove->prev;
        }
        
        free(toRemove);
        saveAllDeliveries(pList);// 更新文件
    } else {
        printf("%s未找到该快递或快递已被取走!%s\n", COLOR_ERROR, COLOR_RESET);
    }
    fclose(fd);
}

/*
    用来获取指定用户的可取快递数量。
    遍历快递链表，统计属于该用户且未被取走的快递数量。
*/

int getDeliveryCountByUser(LIST_DELIVERY* pList, char* userON) {
    int count = 0;
    for(DELIVERY* tempNode = pList->head->next; tempNode; tempNode = tempNode->next) {
        if(strcmp(tempNode->userON, userON) == 0 && tempNode->status == 0) {
            count++;
        }
    }
    return count;
}

/*
    用来按添加时间对快递进行排序。
    使用冒泡排序算法，交换节点位置而非数据。
    按时间从小到大排序。
*/

void sortDeliveriesByTime(LIST_DELIVERY* pList) {
    if(pList->head->next == NULL || pList->head->next->next == NULL) return;
    
    int swapped;
    DELIVERY* ptr1;
    DELIVERY* lptr = NULL;
    
    do {
        swapped = 0;
        ptr1 = pList->head->next;
        
        while(ptr1->next != lptr) {
            if(ptr1->addTime > ptr1->next->addTime) {
                // 交换相邻的两个节点
                DELIVERY* node1 = ptr1;
                DELIVERY* node2 = ptr1->next;
                DELIVERY* node1_prev = node1->prev;
                DELIVERY* node2_next = node2->next;
                
                // 调整 node1 和 node2 的 prev 和 next
                node1->next = node2_next;
                node1->prev = node2;
                node2->next = node1;
                node2->prev = node1_prev;
                
                // 调整前后节点的指针
                if(node1_prev != NULL) {
                    node1_prev->next = node2;
                }
                if(node2_next != NULL) {
                    node2_next->prev = node1;
                }
                
                // 更新 ptr1 继续遍历
                ptr1 = node2;
                swapped = 1;
            }
            ptr1 = ptr1->next;
        }
        lptr = ptr1;
    } while(swapped);
}

/*
    用来按取件码对快递进行排序。
    使用冒泡排序算法，交换节点位置而非数据。
    按取件码字典序从小到大排序。
*/

void sortDeliveriesByCode(LIST_DELIVERY* pList) {
    if(pList->head->next == NULL || pList->head->next->next == NULL) return;
    
    int swapped;
    DELIVERY* ptr1;
    DELIVERY* lptr = NULL;
    
    do {
        swapped = 0;
        ptr1 = pList->head->next;
        
        while(ptr1->next != lptr) {
            if(strcmp(ptr1->Did, ptr1->next->Did) > 0) {
                // 交换相邻的两个节点
                DELIVERY* node1 = ptr1;
                DELIVERY* node2 = ptr1->next;
                DELIVERY* node1_prev = node1->prev;
                DELIVERY* node2_next = node2->next;
                
                // 调整 node1 和 node2 的 prev 和 next
                node1->next = node2_next;
                node1->prev = node2;
                node2->next = node1;
                node2->prev = node1_prev;
                
                // 调整前后节点的指针
                if(node1_prev != NULL) {
                    node1_prev->next = node2;
                }
                if(node2_next != NULL) {
                    node2_next->prev = node1;
                }
                
                // 更新 ptr1 继续遍历
                ptr1 = node2;
                swapped = 1;
            }
            ptr1 = ptr1->next;
        }
        lptr = ptr1;
    } while(swapped);
}

/*
    用来显示指定用户的所有待取快递。
    遍历快递链表，显示属于该用户且未被取走的快递信息。
*/

void showUserDeliveries(LIST_DELIVERY* pList, char* userON) {
    int count = 0;
    printf("%s\\== 您的快递列表 ==/%s\n", COLOR_WELCOME, COLOR_RESET);
    
    for(DELIVERY* tempNode = pList->head->next; tempNode; tempNode = tempNode->next) {
        if(strcmp(tempNode->userON, userON) == 0 && tempNode->status == 0) {
            count++;
            struct tm* t = localtime(&tempNode->addTime);
            printf("%s取件码: %s | 添加时间: %04d-%02d-%02d %02d:%02d:%02d%s\n",
                   COLOR_DISPLAY, tempNode->Did,
                   t->tm_year + 1900, t->tm_mon + 1, t->tm_mday,
                   t->tm_hour, t->tm_min, t->tm_sec, COLOR_RESET);
        }
    }
    
    if(count == 0) {
        printf("%s暂无待取快递!%s\n", COLOR_DISPLAY, COLOR_RESET);
    } else {
        printf("%s共有 %d 件快递待取%s\n", COLOR_WELCOME, count, COLOR_RESET);
    }
}

/*
    用户菜单界面。
    显示用户可以进行的操作：
    q: 取件
    s: 查看快递
    f: 排序快递
    x: 退出
*/

void userMenu(LIST_DELIVERY* pListDelivery, LIST_USER* pListUser, LIST_ADMIN* pListAdmin, USER* currentUser) {
    printf("%s\\== 欢迎进入用户管理界面 ==/%s\n", COLOR_WELCOME, COLOR_RESET);
    char button1;
    char tempDid[MAX_ONLYNUMBER];
    int sortChoice;
    
    while(1) {
        // 显示当前待取快递数量
        int deliveryCount = getDeliveryCountByUser(pListDelivery, currentUser->onlyNumber);
        printf("%s当前有 %d 件快递待取%s\n", COLOR_WELCOME, deliveryCount, COLOR_RESET);
        
        printf("%s\\==%s%sq%s %s取件 ==/%s\n", COLOR_WELCOME, COLOR_RESET, COLOR_DISPLAY, COLOR_RESET, COLOR_WELCOME, COLOR_RESET);
        printf("%s\\==%s%ss%s %s查看快递 ==/%s\n", COLOR_WELCOME, COLOR_RESET, COLOR_DISPLAY, COLOR_RESET, COLOR_WELCOME, COLOR_RESET);
        printf("%s\\==%s%sf%s %s排序快递 ==/%s\n", COLOR_WELCOME, COLOR_RESET, COLOR_DISPLAY, COLOR_RESET, COLOR_WELCOME, COLOR_RESET);
        printf("%s\\==%s%sx%s %s退出 ==/%s\n", COLOR_WELCOME, COLOR_RESET, COLOR_DISPLAY, COLOR_RESET, COLOR_WELCOME, COLOR_RESET);
        printf("%s请选择您要进行的操作:%s", COLOR_DISPLAY, COLOR_RESET);
        
        scanf(" %c", &button1);
        button1 = handleButton1(button1);
        
        switch (button1) {
            case 'q':// 取件
                printf("%s请输入取件码:%s", COLOR_DISPLAY, COLOR_RESET);
                scanf("%s", tempDid);
                tempDid[MAX_ONLYNUMBER - 1] = '\0';
                if(strlen(tempDid) != 5) {
                    printf("%s取件码位数错误!请重试...%s\n", COLOR_ERROR, COLOR_RESET);
                    break;
                } else {
                    int foundDelivery = 0;
                    for(DELIVERY* tempDelivery = pListDelivery->head->next; 
                                tempDelivery; tempDelivery = tempDelivery->next) {
                        if(strcmp(tempDelivery->Did, tempDid) == 0 && 
                           strcmp(tempDelivery->userON, currentUser->onlyNumber) == 0 &&
                           tempDelivery->status == 0) {
                            printf("%s取件成功!请取走包裹.%s\n", COLOR_DISPLAY, COLOR_RESET);
                            removeDelivery(pListDelivery, tempDid, currentUser->onlyNumber);
                            foundDelivery = 1;
                            break;
                        }
                    }
                    if(!foundDelivery) {
                        printf("%s暂无该包裹或取件码错误!请检查后重试...%s\n", COLOR_ERROR, COLOR_RESET);
                    }
                }
                break;
                
            case 's':// 查看快递
                showUserDeliveries(pListDelivery, currentUser->onlyNumber);
                break;
                
            case 'f':// 排序快递
                printf("%s请选择排序方式: 1.按添加时间排序 2.按取件码排序%s\n", COLOR_DISPLAY, COLOR_RESET);
                printf("%s请输入选择(1/2):%s", COLOR_DISPLAY, COLOR_RESET);
                scanf("%d", &sortChoice);
                if(sortChoice == 1) {
                    sortDeliveriesByTime(pListDelivery);
                    printf("%s已按添加时间排序!%s\n", COLOR_DISPLAY, COLOR_RESET);
                } else if(sortChoice == 2) {
                    sortDeliveriesByCode(pListDelivery);
                    printf("%s已按取件码排序!%s\n", COLOR_DISPLAY, COLOR_RESET);
                } else {
                    printf("%s输入错误!%s\n", COLOR_ERROR, COLOR_RESET);
                }
                showUserDeliveries(pListDelivery, currentUser->onlyNumber);
                break;
                
            case 'x':// 退出
                exitFirst(pListUser, pListDelivery, pListAdmin);
                return;
                
            default:
                printf("%s输入错误!请重试...%s\n", COLOR_ERROR, COLOR_RESET);
                break;
        }
    }
}

/*
    管理员注册函数。
    创建新的管理员节点，输入用户名和密码，
    并添加到链表和文件中。
*/

int signupAdmin(LIST_ADMIN* pList) {
    ADMIN* newAdmin = (ADMIN*)malloc(sizeof(ADMIN));
    char tempAdminName[MAX_NAME];// 用户名数组
    char tempPassword[MAX_PASSWORD];// 密码数组
    char istempPassword[MAX_PASSWORD];// 确认密码数组
    int ispwRight = 0;// 确认密码是否成功
    int nameLen = 0, passwordLen = 0;// 用户名长度，密码长度
    
    printf("%s\\== 欢迎来到管理员注册界面 ==/%s\n", COLOR_WELCOME, COLOR_RESET);
    
    while(1) {// 用户名输入循环
        printf("%s请输入您的新用户名:(最大支持10位)%s", COLOR_DISPLAY, COLOR_RESET);
        scanf("%s", tempAdminName);
        tempAdminName[MAX_NAME - 1] = '\0';
        nameLen = strlen(tempAdminName);
        if(nameLen > 10) {
            printf("%s您输入的用户名过长!请重试...%s\n", COLOR_ERROR, COLOR_RESET);
        } else {
            int nameExists = 0;
            for(ADMIN* tempNode = pList->head->next; tempNode; tempNode = tempNode->next) {
                if(strcmp(tempNode->name, tempAdminName) == 0) {
                    printf("%s您输入的用户名已被注册过!请重试...%s\n", COLOR_ERROR, COLOR_RESET);
                    nameExists = 1;
                    break;
                }
            }
            if(!nameExists) {
                strcpy(newAdmin->name, tempAdminName);
                break;
            }
        }
    }
    
    while(1) {// 密码输入循环
        printf("%s请输入您的新密码:(最大支持25位)%s", COLOR_DISPLAY, COLOR_RESET);
        scanf("%s", tempPassword);
        tempPassword[MAX_PASSWORD - 1] = '\0';
        passwordLen = strlen(tempPassword);
        if(passwordLen > 25) {
            printf("%s您输入的密码过长!请重试...%s\n", COLOR_ERROR, COLOR_RESET);
        } else {
            while(1) {
                printf("%s请确认您的新密码:%s", COLOR_DISPLAY, COLOR_RESET);
                scanf("%s", istempPassword);
                istempPassword[MAX_PASSWORD - 1] = '\0';
                if(strcmp(tempPassword, istempPassword) == 0) {
                    printf("%s密码确认成功!%s\n", COLOR_DISPLAY, COLOR_RESET);
                    ispwRight = 1;
                    break;
                } else {
                    printf("%s您重新输入的密码有误!请重试...%s\n", COLOR_ERROR, COLOR_RESET);
                }
            }
            if(ispwRight) {
                strcpy(newAdmin->password, tempPassword);
                break;
            }
        }
    }
    
    addAdmin(pList, newAdmin);
    return 1;
}

/*
    管理员相关界面。
    提供管理员登录、注册和退出选项。
*/

int aboutAdmin(LIST_ADMIN* pList) {
    printf("%s\\== 欢迎进入管理员系统 ==/%s\n", COLOR_WELCOME, COLOR_RESET);
    char button1;
    while(1) {
        printf("%s\\==%s%sq%s %s登录 ==/%s\n", COLOR_WELCOME, COLOR_RESET, COLOR_DISPLAY, COLOR_RESET, COLOR_WELCOME, COLOR_RESET);
        printf("%s\\==%s%sw%s %s注册 ==/%s\n", COLOR_WELCOME, COLOR_RESET, COLOR_DISPLAY, COLOR_RESET, COLOR_WELCOME, COLOR_RESET);
        printf("%s\\==%s%sx%s %s退出 ==/%s\n", COLOR_WELCOME, COLOR_RESET, COLOR_DISPLAY, COLOR_RESET, COLOR_WELCOME, COLOR_RESET);
        printf("%s请选择您要进行的操作:%s", COLOR_DISPLAY, COLOR_RESET);
        scanf(" %c", &button1);
        button1 = handleButton1(button1);
        
        switch (button1) {
            case 'q':// 登录
                if(signinAdmin(pList)) {
                    return 1;
                }
                break;
            case 'w':// 注册
                if(signupAdmin(pList)) {
                    return 2;
                }
                break;
            case 'x':// 退出
                return 0;
                break;
            default:
                printf("%s输入错误!请重试...%s\n", COLOR_ERROR, COLOR_RESET);
                break;
        }
    }
}

/*
    收集统计数据函数。
    遍历所有链表，收集各种统计信息。
*/

void collectStatistics(LIST_DELIVERY* pListDelivery, LIST_USER* pListUser, 
                       LIST_ADMIN* pListAdmin, STATISTICS* stats, TIME_STAT* timeStat) {
    // 初始化统计结构体
    memset(stats, 0, sizeof(STATISTICS));
    memset(timeStat, 0, sizeof(TIME_STAT));
    stats->oldestDelivery = time(NULL);
    
    // 创建用户快递统计数组
    USER_DELIVERY_STAT* userStats = NULL;
    int userStatCount = 0;
    
    // 统计用户
    for(USER* tempUser = pListUser->head->next; tempUser; tempUser = tempUser->next) {
        stats->totalUsers++;
        if(tempUser->issignup == 1) {
            stats->registeredUsers++;
        }
        
        // 扩展用户统计数组
        userStatCount++;
        userStats = (USER_DELIVERY_STAT*)realloc(userStats, userStatCount * sizeof(USER_DELIVERY_STAT));
        strcpy(userStats[userStatCount-1].userName, tempUser->name);
        strcpy(userStats[userStatCount-1].userON, tempUser->onlyNumber);
        userStats[userStatCount-1].deliveryCount = 0;
        userStats[userStatCount-1].pickedCount = 0;
        userStats[userStatCount-1].waitingCount = 0;
        userStats[userStatCount-1].lastDeliveryTime = 0;
    }
    
    // 统计管理员
    for(ADMIN* tempAdmin = pListAdmin->head->next; tempAdmin; tempAdmin = tempAdmin->next) {
        stats->adminCount++;
    }
    
    // 获取当前时间用于时间统计
    time_t now = time(NULL);
    time_t oneHourAgo = now - 3600;
    time_t oneDayAgo = now - 86400;
    time_t oneWeekAgo = now - 604800;
    time_t oneMonthAgo = now - 2592000;
    
    // 统计快递
    for(DELIVERY* tempDelivery = pListDelivery->head->next; tempDelivery; 
        tempDelivery = tempDelivery->next) {
        stats->totalDeliveries++;
        
        if(tempDelivery->status == 1) {
            stats->pickedDeliveries++;
        } else {
            stats->waitingDeliveries++;
            
            // 更新最早未取快递时间
            if(tempDelivery->addTime < stats->oldestDelivery) {
                stats->oldestDelivery = tempDelivery->addTime;
            }
            
            // 时间分布统计
            if(tempDelivery->addTime >= oneHourAgo) {
                timeStat->lastHour++;
            } else if(tempDelivery->addTime >= oneDayAgo) {
                timeStat->lastDay++;
            } else if(tempDelivery->addTime >= oneWeekAgo) {
                timeStat->lastWeek++;
            } else if(tempDelivery->addTime >= oneMonthAgo) {
                timeStat->lastMonth++;
            } else {
                timeStat->older++;
            }
        }
        
        // 更新最新快递时间
        if(tempDelivery->addTime > stats->newestDelivery) {
            stats->newestDelivery = tempDelivery->addTime;
        }
        
        // 统计每个用户的快递
        for(int i = 0; i < userStatCount; i++) {
            if(strcmp(userStats[i].userON, tempDelivery->userON) == 0) {
                userStats[i].deliveryCount++;
                if(tempDelivery->status == 1) {
                    userStats[i].pickedCount++;
                } else {
                    userStats[i].waitingCount++;
                }
                if(tempDelivery->addTime > userStats[i].lastDeliveryTime) {
                    userStats[i].lastDeliveryTime = tempDelivery->addTime;
                }
                break;
            }
        }
    }
    
    // 计算取件率
    if(stats->totalDeliveries > 0) {
        stats->pickRate = (double)stats->pickedDeliveries / stats->totalDeliveries * 100;
    }
    
    // 查找快递最多的用户
    stats->mostActiveUserCount = 0;
    for(int i = 0; i < userStatCount; i++) {
        if(userStats[i].deliveryCount > stats->mostActiveUserCount) {
            stats->mostActiveUserCount = userStats[i].deliveryCount;
            strcpy(stats->mostActiveUser, userStats[i].userName);
        }
    }
    
    free(userStats);
}

/*
    打印统计报表函数。
    生成格式化的统计报表。
*/

void printStatisticsReport(STATISTICS* stats, TIME_STAT* timeStat, 
                          USER_DELIVERY_STAT* topUsers, int topUserCount) {
    printf("\n%s╔══════════════════════════════════════════════════════════════╗%s\n", 
           COLOR_WELCOME, COLOR_RESET);
    printf("%s║                   社区快递柜管理系统统计报表                    ║%s\n", 
           COLOR_WELCOME, COLOR_RESET);
    printf("%s╠══════════════════════════════════════════════════════════════╣%s\n", 
           COLOR_WELCOME, COLOR_RESET);
    
    // 快递总体情况
    printf("%s║ 快递统计                                    ║%s\n", 
           COLOR_DISPLAY, COLOR_RESET);
    printf("%s║   总快递数: %-36d ║%s\n", 
           COLOR_DISPLAY, stats->totalDeliveries, COLOR_RESET);
    printf("%s║   已取件数: %-36d ║%s\n", 
           COLOR_DISPLAY, stats->pickedDeliveries, COLOR_RESET);
    printf("%s║   待取件数: %-36d ║%s\n", 
           COLOR_DISPLAY, stats->waitingDeliveries, COLOR_RESET);
    printf("%s║   取件率: %-36.2f%% ║%s\n", 
           COLOR_DISPLAY, stats->pickRate, COLOR_RESET);
    
    // 时间分布统计
    printf("%s╠══════════════════════════════════════════════════════════════╣%s\n", 
           COLOR_DISPLAY, COLOR_RESET);
    printf("%s║ 快递时间分布                                ║%s\n", 
           COLOR_DISPLAY, COLOR_RESET);
    printf("%s║   最近1小时: %-34d ║%s\n", 
           COLOR_DISPLAY, timeStat->lastHour, COLOR_RESET);
    printf("%s║   最近24小时: %-33d ║%s\n", 
           COLOR_DISPLAY, timeStat->lastDay, COLOR_RESET);
    printf("%s║   最近7天: %-36d ║%s\n", 
           COLOR_DISPLAY, timeStat->lastWeek, COLOR_RESET);
    printf("%s║   最近30天: %-35d ║%s\n", 
           COLOR_DISPLAY, timeStat->lastMonth, COLOR_RESET);
    printf("%s║   30天以上: %-35d ║%s\n", 
           COLOR_DISPLAY, timeStat->older, COLOR_RESET);
    
    // 用户统计
    printf("%s╠══════════════════════════════════════════════════════════════╣%s\n", 
           COLOR_DISPLAY, COLOR_RESET);
    printf("%s║ 用户统计                                    ║%s\n", 
           COLOR_DISPLAY, COLOR_RESET);
    printf("%s║   总用户数: %-36d ║%s\n", 
           COLOR_DISPLAY, stats->totalUsers, COLOR_RESET);
    printf("%s║   注册用户数: %-34d ║%s\n", 
           COLOR_DISPLAY, stats->registeredUsers, COLOR_RESET);
    printf("%s║   管理员数: %-36d ║%s\n", 
           COLOR_DISPLAY, stats->adminCount, COLOR_RESET);
    
    // 最活跃用户
    if(topUserCount > 0) {
        printf("%s╠══════════════════════════════════════════════════════════════╣%s\n", 
               COLOR_DISPLAY, COLOR_RESET);
        printf("%s║ 快递最多的用户(TOP %d)                         ║%s\n", 
               COLOR_DISPLAY, topUserCount > 3 ? 3 : topUserCount, COLOR_RESET);
        for(int i = 0; i < (topUserCount > 3 ? 3 : topUserCount); i++) {
            printf("%s║   %d. %-10s (校验码:%-6s) 快递数:%-4d ║%s\n", 
                   COLOR_DISPLAY, i+1, topUsers[i].userName, 
                   topUsers[i].userON, topUsers[i].deliveryCount, COLOR_RESET);
        }
    }
    
    // 最早未取快递提示
    if(stats->oldestDelivery > 0 && stats->oldestDelivery < time(NULL)) {
        struct tm* t = localtime(&stats->oldestDelivery);
        printf("%s╠══════════════════════════════════════════════════════════════╣%s\n", 
               COLOR_DISPLAY, COLOR_RESET);
        printf("%s║ 最早未取快递时间: %04d-%02d-%02d %02d:%02d:%02d        ║%s\n", 
               COLOR_DISPLAY, t->tm_year + 1900, t->tm_mon + 1, t->tm_mday,
               t->tm_hour, t->tm_min, t->tm_sec, COLOR_RESET);
    }
    
    printf("%s╚══════════════════════════════════════════════════════════════╝%s\n", 
           COLOR_WELCOME, COLOR_RESET);
}

/*
    简单统计功能函数。
    快速显示核心统计数据。
*/

void simpleStatistics(LIST_DELIVERY* pListDelivery, LIST_USER* pListUser, 
                     LIST_ADMIN* pListAdmin) {
    STATISTICS stats;
    TIME_STAT timeStat;
    
    collectStatistics(pListDelivery, pListUser, pListAdmin, &stats, &timeStat);
    
    printf("\n%s╔══════════════════════════════════════════════════════════════╗%s\n", 
           COLOR_WELCOME, COLOR_RESET);
    printf("%s║                      简单统计信息                              ║%s\n", 
           COLOR_WELCOME, COLOR_RESET);
    printf("%s╠══════════════════════════════════════════════════════════════╣%s\n", 
           COLOR_WELCOME, COLOR_RESET);
    printf("%s║   总快递数: %-6d | 待取件数: %-6d | 取件率: %-6.2f%% ║%s\n", 
           COLOR_DISPLAY, stats.totalDeliveries, stats.waitingDeliveries, 
           stats.pickRate, COLOR_RESET);
    printf("%s║   总用户数: %-6d | 注册用户: %-6d | 管理员数: %-6d ║%s\n", 
           COLOR_DISPLAY, stats.totalUsers, stats.registeredUsers, 
           stats.adminCount, COLOR_RESET);
    printf("%s║   最活跃用户: %-10s | 快递数: %-6d                ║%s\n", 
           COLOR_DISPLAY, stats.mostActiveUser, stats.mostActiveUserCount, 
           COLOR_RESET);
    printf("%s╚══════════════════════════════════════════════════════════════╝%s\n", 
           COLOR_WELCOME, COLOR_RESET);
}

/*
    综合统计功能函数。
    显示详细统计信息，包括时间分布和用户排名。
*/

void comprehensiveStatistics(LIST_DELIVERY* pListDelivery, LIST_USER* pListUser, 
                            LIST_ADMIN* pListAdmin) {
    STATISTICS stats;
    TIME_STAT timeStat;
    USER_DELIVERY_STAT* userStats = NULL;
    int userStatCount = 0;
    
    // 收集统计数据
    collectStatistics(pListDelivery, pListUser, pListAdmin, &stats, &timeStat);
    
    // 重新收集用户快递统计用于排名
    userStatCount = 0;
    for(USER* tempUser = pListUser->head->next; tempUser; tempUser = tempUser->next) {
        userStatCount++;
        userStats = (USER_DELIVERY_STAT*)realloc(userStats, userStatCount * sizeof(USER_DELIVERY_STAT));
        strcpy(userStats[userStatCount-1].userName, tempUser->name);
        strcpy(userStats[userStatCount-1].userON, tempUser->onlyNumber);
        userStats[userStatCount-1].deliveryCount = 0;
        userStats[userStatCount-1].pickedCount = 0;
        userStats[userStatCount-1].waitingCount = 0;
        userStats[userStatCount-1].lastDeliveryTime = 0;
    }
    
    // 统计快递
    for(DELIVERY* tempDelivery = pListDelivery->head->next; tempDelivery; 
        tempDelivery = tempDelivery->next) {
        for(int i = 0; i < userStatCount; i++) {
            if(strcmp(userStats[i].userON, tempDelivery->userON) == 0) {
                userStats[i].deliveryCount++;
                if(tempDelivery->status == 1) {
                    userStats[i].pickedCount++;
                } else {
                    userStats[i].waitingCount++;
                }
                if(tempDelivery->addTime > userStats[i].lastDeliveryTime) {
                    userStats[i].lastDeliveryTime = tempDelivery->addTime;
                }
                break;
            }
        }
    }
    
    // 对用户按快递数量排序（冒泡排序）
    for(int i = 0; i < userStatCount - 1; i++) {
        for(int j = 0; j < userStatCount - i - 1; j++) {
            if(userStats[j].deliveryCount < userStats[j+1].deliveryCount) {
                USER_DELIVERY_STAT temp = userStats[j];
                userStats[j] = userStats[j+1];
                userStats[j+1] = temp;
            }
        }
    }
    
    // 生成完整报表
    printStatisticsReport(&stats, &timeStat, userStats, userStatCount);
    
    free(userStats);
}

/*
    导出统计报表到文件函数。
    将统计信息保存到statistics.txt文件中。
*/

void exportStatisticsToFile(LIST_DELIVERY* pListDelivery, LIST_USER* pListUser, 
                           LIST_ADMIN* pListAdmin) {
    FILE* fd = fopen("statistics.txt", "w");
    if(fd == NULL) {
        printf("%s无法创建统计文件!%s\n", COLOR_ERROR, COLOR_RESET);
        return;
    }
    
    STATISTICS stats;
    TIME_STAT timeStat;
    USER_DELIVERY_STAT* userStats = NULL;
    int userStatCount = 0;
    time_t now = time(NULL);
    
    // 收集统计数据
    collectStatistics(pListDelivery, pListUser, pListAdmin, &stats, &timeStat);
    
    // 重新收集用户统计用于排名
    userStatCount = 0;
    for(USER* tempUser = pListUser->head->next; tempUser; tempUser = tempUser->next) {
        userStatCount++;
        userStats = (USER_DELIVERY_STAT*)realloc(userStats, userStatCount * sizeof(USER_DELIVERY_STAT));
        strcpy(userStats[userStatCount-1].userName, tempUser->name);
        strcpy(userStats[userStatCount-1].userON, tempUser->onlyNumber);
        userStats[userStatCount-1].deliveryCount = 0;
        userStats[userStatCount-1].pickedCount = 0;
        userStats[userStatCount-1].waitingCount = 0;
        userStats[userStatCount-1].lastDeliveryTime = 0;
    }
    
    // 统计快递
    for(DELIVERY* tempDelivery = pListDelivery->head->next; tempDelivery; 
        tempDelivery = tempDelivery->next) {
        for(int i = 0; i < userStatCount; i++) {
            if(strcmp(userStats[i].userON, tempDelivery->userON) == 0) {
                userStats[i].deliveryCount++;
                if(tempDelivery->status == 1) {
                    userStats[i].pickedCount++;
                } else {
                    userStats[i].waitingCount++;
                }
                if(tempDelivery->addTime > userStats[i].lastDeliveryTime) {
                    userStats[i].lastDeliveryTime = tempDelivery->addTime;
                }
                break;
            }
        }
    }
    
    // 排序用户
    for(int i = 0; i < userStatCount - 1; i++) {
        for(int j = 0; j < userStatCount - i - 1; j++) {
            if(userStats[j].deliveryCount < userStats[j+1].deliveryCount) {
                USER_DELIVERY_STAT temp = userStats[j];
                userStats[j] = userStats[j+1];
                userStats[j+1] = temp;
            }
        }
    }
    
    // 写入文件
    fprintf(fd, "社区快递柜管理系统统计报表\n");
    fprintf(fd, "生成时间: %s", ctime(&now));
    fprintf(fd, "========================================\n\n");
    
    fprintf(fd, "一、快递统计\n");
    fprintf(fd, "   总快递数: %d\n", stats.totalDeliveries);
    fprintf(fd, "   已取件数: %d\n", stats.pickedDeliveries);
    fprintf(fd, "   待取件数: %d\n", stats.waitingDeliveries);
    fprintf(fd, "   取件率: %.2f%%\n\n", stats.pickRate);
    
    fprintf(fd, "二、快递时间分布\n");
    fprintf(fd, "   最近1小时: %d\n", timeStat.lastHour);
    fprintf(fd, "   最近24小时: %d\n", timeStat.lastDay);
    fprintf(fd, "   最近7天: %d\n", timeStat.lastWeek);
    fprintf(fd, "   最近30天: %d\n", timeStat.lastMonth);
    fprintf(fd, "   30天以上: %d\n\n", timeStat.older);
    
    fprintf(fd, "三、用户统计\n");
    fprintf(fd, "   总用户数: %d\n", stats.totalUsers);
    fprintf(fd, "   注册用户数: %d\n", stats.registeredUsers);
    fprintf(fd, "   管理员数: %d\n\n", stats.adminCount);
    
    fprintf(fd, "四、用户快递排行榜\n");
    for(int i = 0; i < (userStatCount > 10 ? 10 : userStatCount); i++) {
        fprintf(fd, "   %d. %-10s (校验码:%-6s) 总快递:%-3d 待取:%-3d\n", 
                i+1, userStats[i].userName, userStats[i].userON,
                userStats[i].deliveryCount, userStats[i].waitingCount);
    }
    
    if(stats.oldestDelivery > 0 && stats.oldestDelivery < now) {
        struct tm* t = localtime(&stats.oldestDelivery);
        fprintf(fd, "\n五、其他信息\n");
        fprintf(fd, "   最早未取快递时间: %04d-%02d-%02d %02d:%02d:%02d\n",
                t->tm_year + 1900, t->tm_mon + 1, t->tm_mday,
                t->tm_hour, t->tm_min, t->tm_sec);
        fprintf(fd, "   最活跃用户: %s (快递数: %d)\n", 
                stats.mostActiveUser, stats.mostActiveUserCount);
    }
    
    fclose(fd);
    free(userStats);
    
    printf("%s统计报表已导出到 statistics.txt 文件!%s\n", COLOR_DISPLAY, COLOR_RESET);
}

/*
    管理员菜单界面。
    提供存件、查看所有快递、统计和退出功能。
    存件时需要输入取件码和收件人用户名。
*/

void adminMenu(LIST_DELIVERY* pListDelivery, LIST_USER* pListUser, LIST_ADMIN* pListAdmin) {
    printf("%s\\== 欢迎进入管理员系统 ==/%s\n", COLOR_WELCOME, COLOR_RESET);
    DELIVERY* Node;
    char tempDid[MAX_ONLYNUMBER];
    char tempUserName[MAX_NAME];
    int isRight = 0;
    int foundUser = 0;
    char button1;
    
    while(1) {
        printf("%s\\==%s%sq%s %s存件 ==/%s\n", COLOR_WELCOME, COLOR_RESET, COLOR_DISPLAY, COLOR_RESET, COLOR_WELCOME, COLOR_RESET);
        printf("%s\\==%s%sv%s %s查看所有快递 ==/%s\n", COLOR_WELCOME, COLOR_RESET, COLOR_DISPLAY, COLOR_RESET, COLOR_WELCOME, COLOR_RESET);
        printf("%s\\==%s%ss%s %s简单统计 ==/%s\n", COLOR_WELCOME, COLOR_RESET, COLOR_DISPLAY, COLOR_RESET, COLOR_WELCOME, COLOR_RESET);
        printf("%s\\==%s%sc%s %s综合统计 ==/%s\n", COLOR_WELCOME, COLOR_RESET, COLOR_DISPLAY, COLOR_RESET, COLOR_WELCOME, COLOR_RESET);
        printf("%s\\==%s%se%s %s导出报表 ==/%s\n", COLOR_WELCOME, COLOR_RESET, COLOR_DISPLAY, COLOR_RESET, COLOR_WELCOME, COLOR_RESET);
        printf("%s\\==%s%sx%s %s退出 ==/%s\n", COLOR_WELCOME, COLOR_RESET, COLOR_DISPLAY, COLOR_RESET, COLOR_WELCOME, COLOR_RESET);
        printf("%s请选择您要进行的操作:%s", COLOR_DISPLAY, COLOR_RESET);
        scanf(" %c", &button1);
        button1 = handleButton1(button1);
        
        switch (button1) {
            case 'q':// 存件
                Node = (DELIVERY*)malloc(sizeof(DELIVERY));
                Node->prev = NULL;
                Node->next = NULL;
                isRight = 0;
                
                // 输入取件码
                while(1) {
                    printf("%s请输入5位取件码:%s", COLOR_DISPLAY, COLOR_RESET);
                    scanf("%s", tempDid);
                    tempDid[MAX_ONLYNUMBER - 1] = '\0';
                    if(strlen(tempDid) != 5) {
                        printf("%s位数错误!请重试...%s\n", COLOR_ERROR, COLOR_RESET);
                    } else {
                        isRight = 1;
                        break;
                    }
                }
                
                // 输入收件人用户名并查找其校验码
                foundUser = 0;
                while(1) {
                    printf("%s请输入取件人用户名:%s", COLOR_DISPLAY, COLOR_RESET);
                    scanf("%s", tempUserName);
                    tempUserName[MAX_NAME - 1] = '\0';
                    
                    for(USER* tempUser = pListUser->head->next; tempUser; tempUser = tempUser->next) {
                        if(strcmp(tempUser->name, tempUserName) == 0) {
                            strcpy(Node->userName, tempUserName);
                            strcpy(Node->userON, tempUser->onlyNumber);
                            foundUser = 1;
                            break;
                        }
                    }
                    
                    if(foundUser) {
                        break;
                    } else {
                        printf("%s未找到该用户!请重新输入...%s\n", COLOR_ERROR, COLOR_RESET);
                    }
                }
                
                if(isRight && foundUser) {
                    strcpy(Node->Did, tempDid);
                    addDelivery(pListDelivery, Node);
                } else {
                    free(Node);
                }
                break;
                
            case 'v':// 查看所有快递
                printf("%s\\== 所有快递列表 ==/%s\n", COLOR_WELCOME, COLOR_RESET);
                int count = 0;
                for(DELIVERY* tempNode = pListDelivery->head->next; tempNode; tempNode = tempNode->next) {
                    if(tempNode->status == 0) {
                        count++;
                        struct tm* t = localtime(&tempNode->addTime);
                        printf("%s取件码: %s | 收件人: %s | 校验码: %s | 添加时间: %04d-%02d-%02d %02d:%02d:%02d%s\n",
                               COLOR_DISPLAY, tempNode->Did, tempNode->userName, tempNode->userON,
                               t->tm_year + 1900, t->tm_mon + 1, t->tm_mday,
                               t->tm_hour, t->tm_min, t->tm_sec, COLOR_RESET);
                    }
                }
                if(count == 0) {
                    printf("%s暂无快递!%s\n", COLOR_DISPLAY, COLOR_RESET);
                } else {
                    printf("%s共有 %d 件快递待取%s\n", COLOR_WELCOME, count, COLOR_RESET);
                }
                break;
                
            case 's':// 简单统计
                simpleStatistics(pListDelivery, pListUser, pListAdmin);
                printf("\n%s按任意键返回...%s", COLOR_DISPLAY, COLOR_RESET);
                getchar();
                getchar();
                break;
                
            case 'c':// 综合统计
                comprehensiveStatistics(pListDelivery, pListUser, pListAdmin);
                printf("\n%s按任意键返回...%s", COLOR_DISPLAY, COLOR_RESET);
                getchar();
                getchar();
                break;
                
            case 'e':// 导出报表
                exportStatisticsToFile(pListDelivery, pListUser, pListAdmin);
                printf("\n%s按任意键返回...%s", COLOR_DISPLAY, COLOR_RESET);
                getchar();
                getchar();
                break;
                
            case 'x':// 退出
                return;
                break;
                
            default:
                printf("%s输入错误!请重试...%s\n", COLOR_ERROR, COLOR_RESET);
                break;
        }
    }
}

/*
    欢迎菜单界面。
    先显示好提供用户可以进行的操作，
    之后在while循环内等待用户输入，
    保证用户输入错误之后，
    依然可以供用户继续输入。
    通过switch确定要进入哪个函数。
*/

void firstMenu(LIST_DELIVERY* pListDelivery, LIST_USER* pListUser, LIST_ADMIN* pListAdmin) {
    printf("%s\\== 欢迎进入社区快递柜管理系统 ==/%s\n", COLOR_WELCOME, COLOR_RESET);
    char button1;
    USER* currentUser = NULL;
    
    while(1) {
        printf("%s\\==%s%sq%s %s登录 ==/%s\n", COLOR_WELCOME, COLOR_RESET, COLOR_DISPLAY, COLOR_RESET, COLOR_WELCOME, COLOR_RESET);
        printf("%s\\==%s%sw%s %s注册 ==/%s\n", COLOR_WELCOME, COLOR_RESET, COLOR_DISPLAY, COLOR_RESET, COLOR_WELCOME, COLOR_RESET);
        printf("%s\\==%s%se%s %s快速取件 ==/%s\n", COLOR_WELCOME, COLOR_RESET, COLOR_DISPLAY, COLOR_RESET, COLOR_WELCOME, COLOR_RESET);
        printf("%s\\==%s%sr%s %s使用说明 ==/%s\n", COLOR_WELCOME, COLOR_RESET, COLOR_DISPLAY, COLOR_RESET, COLOR_WELCOME, COLOR_RESET);
        printf("%s\\==%s%st%s %s关于我们 ==/%s\n", COLOR_WELCOME, COLOR_RESET, COLOR_DISPLAY, COLOR_RESET, COLOR_WELCOME, COLOR_RESET);
        printf("%s\\==%s%su%s %s管理员相关 ==/%s\n", COLOR_WELCOME, COLOR_RESET, COLOR_DISPLAY, COLOR_RESET, COLOR_WELCOME, COLOR_RESET);
        printf("%s\\==%s%sx%s %s退出 ==/%s\n", COLOR_WELCOME, COLOR_RESET, COLOR_DISPLAY, COLOR_RESET, COLOR_WELCOME, COLOR_RESET);
        printf("%s请选择您要进行的操作:%s\n", COLOR_DISPLAY, COLOR_RESET);
        scanf(" %c", &button1);
        button1 = handleButton1(button1);// 处理用户大小写问题
        
        switch (button1) {
            case 'q':// 登录
                if(signin(pListUser, &currentUser)) {
                    userMenu(pListDelivery, pListUser, pListAdmin, currentUser);
                }
                break;
            case 'w':// 注册
                signup(pListUser);
                break;
            case 'e':// 快速取件
                quickGet(pListDelivery, pListUser);
                break;
            case 'r':// 使用说明
                readMeUsing();
                break;
            case 't':// 关于我们
                aboutUs();
                break;
            case 'u':// 管理员相关
                int statusAdmin = aboutAdmin(pListAdmin);
                switch (statusAdmin) {
                    case 1:
                        adminMenu(pListDelivery, pListUser, pListAdmin);
                        break;
                    case 2:
                        break;
                    default:
                        break;
                }
                break;
            case 'x':// 退出
                exitFirst(pListUser, pListDelivery, pListAdmin);
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