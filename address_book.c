#include "address_book.h"

static char * FILE_NAME = "address_book.dat";

// 检测容量
static int check_cap(address_book_t *p_book);

static int find_by_name(address_book_t *p_book, char *name);

static void create_contact(address_book_t *p_book, int cur_position);

static void bubble_sort(void *arr, int mem_size, int mem_num
    , int (*cmp)(const void *, const void *));

static int cmp_by_name(const void *, const void *);

static int cmp_by_age(const void *, const void *);

static int cmp_by_address(const void *, const void *);

// 信号处理函数  
void handle_interrupt(int sig) {  
    if (sig == SIGINT) {  
        printf("\n再见\n");
        exit(0); // 退出程序  
    }  
} 

int init_address_book(address_book_t *p_book) {
    p_book->data = (contact_t *)calloc(DEFAULT_CAP, sizeof(contact_t));
    if (p_book->data == NULL) {
        perror("获取内存失败: ");
        return -1;
    }
    p_book->size = 0;
    p_book->capacity = DEFAULT_CAP;
    return 0;
}

void add_contact(address_book_t *p_book) {
    printf("-----新建联系人-----\n");
    int opt_result = check_cap(p_book);
    if (opt_result == -1) {
        printf("通讯录容量不足，无法新增\n");
        return;
    }

    create_contact(p_book, p_book->size);
    p_book->size++;
    printf("新增成功\n");
}

void del_contact(address_book_t *p_book) {
    char name[NAME_LEN];
    printf("-----删除联系人-----\n");
    printf("请输入联系人姓名：");
    scanf("%s", name);

    int position = find_by_name(p_book, name);
    if (position == -1) {
        printf("姓名不存在\n");
        return;
    }

    for (int i = position; i < p_book->size; i++) {
        if (i == p_book->size - 1) {
            memset(&p_book->data[i], 0, sizeof(contact_t));
        } else {
            p_book->data[i] = p_book->data[i + 1];
        }
    }
    p_book->size--;
    printf("删除成功\n");
}

void update_contact(address_book_t *p_book) {
    char name[NAME_LEN];
    printf("-----修改联系人------\n");
    printf("请输入联系人姓名：");
    scanf("%s", name);

    int position = find_by_name(p_book, name);
    if (position == -1) {
        printf("姓名不存在\n");
        return;
    }

    create_contact(p_book, position);
    printf("修改成功\n");
}

void show_contact(address_book_t *p_book) {
    char name[NAME_LEN];

    printf("-----查看联系人------\n");
    printf("请输入联系人姓名：");
    scanf("%s", name);

    int position = find_by_name(p_book, name);
    if (position == -1) {
        printf("姓名不存在\n");
        return;
    }

    printf("%-10s\t%-5s\t%-5s\t%-15s\t%-20s\n"
        , "姓名", "年龄", "性别", "电话", "地址");
    contact_t *p_contact = &p_book->data[position];
    printf("%-10s\t%-5d\t%-5s\t%-15s\t%-20s\n"
        , p_contact->name, p_contact->age, p_contact->gender
        , p_contact->phone, p_contact->address);
}

void list_contact(address_book_t *p_book) {
    printf("-----联系人列表-----\n");
    printf("%-10s\t%-5s\t%-5s\t%-15s\t%-20s\n"
        , "姓名", "年龄", "性别", "电话", "地址");
    for (int i = 0; i < p_book->size; i++) {
        contact_t *p_contact = &p_book->data[i];
        printf("%-10s\t%-5d\t%-5s\t%-15s\t%-20s\n"
            , p_contact->name, p_contact->age, p_contact->gender
            , p_contact->phone, p_contact->address);
    }
}

void sort_contact(address_book_t *p_book) {
    printf("-----联系人排序-----\n");
    int input_opt = 0;
    printf("请输入排序字段：1-姓名 2-年龄 3-地址 >>");
    safe_scanf_int(&input_opt);

    switch (input_opt) {
    case 1:
        printf("依据名称排序\n");
        bubble_sort(p_book->data, sizeof(contact_t), p_book->size, cmp_by_name);
        break;
    case 2:
        printf("依据年龄排序\n");
        bubble_sort(p_book->data, sizeof(contact_t), p_book->size, cmp_by_age);
        break;
    case 3:
        printf("依据地址排序\n");
        bubble_sort(p_book->data, sizeof(contact_t), p_book->size, cmp_by_address);
        break;
    default:
        printf("输入错误，无法排序\n");
        break;
    }

    list_contact(p_book);
}

void safe_scanf_int(void *input) {
    int *inp = (int *)input;
    while (scanf("%d", inp) != 1) {
        printf("输入无效，请输入数字：");
        while (getchar() != '\n');  // 清除输入缓冲区
    }
}

void clear_address_book(address_book_t *p_book) {
    printf("-----清空通讯录-----\n");
    free(p_book->data);
    p_book->data = NULL;

    init_address_book(p_book);
}

void save_address_book(address_book_t *p_book) {
    FILE * p_file = fopen(FILE_NAME, "wb");
    for (int i = 0; i < p_book->size; i++) {
        fwrite(&p_book->data[i], sizeof(contact_t), 1, p_file);
    }
    fclose(p_file);

    printf("写入成功\n");
}

int load_address_book(address_book_t *p_book) {
    int opt_result = init_address_book(p_book);
    if (opt_result == -1) {
        return opt_result;
    }

    FILE * p_file = fopen(FILE_NAME, "rb");
    if (p_file == NULL) {
        return 0;
    }
    int read_len = 0;
    while (1) {
        opt_result = check_cap(p_book);
        if (opt_result == -1) {
            return opt_result;
        }
        read_len = fread(&p_book->data[p_book->size], sizeof(contact_t), 1, p_file);
        if (read_len <= 0) {
            break;
        }
        p_book->size++;
    }
    fclose(p_file);
    return 0;
}

static int cmp_by_name(const void * curr, const void * next) {
    return strcmp(((contact_t *)curr)->name, ((contact_t *)next)->name);
}

static int cmp_by_age(const void * curr, const void * next) {
    return ((contact_t *)curr)->age - ((contact_t *)next)->age;
}

static int cmp_by_address(const void * curr, const void * next) {
    return strcmp(((contact_t *)curr)->address, ((contact_t *)next)->address);
}

static void bubble_sort(void *arr, int mem_size, int mem_num
    , int (*cmp)(const void *, const void *)) {

    char *array = (char *)arr;
    for (int i = 0; i < mem_num - 1; i++) {
        for (int j = 0; j < mem_num - 1 - i; j++) {
            char *curr = &array[j * mem_size];
            char *next = &array[(j + 1) * mem_size];
            if (cmp(curr, next) > 0) {
                for (int k = 0; k < mem_size; k++) {
                    char tmp = curr[k];
                    curr[k] = next[k];
                    next[k] = tmp;
                }
            }
        }
    }
}

static void create_contact(address_book_t *p_book, int cur_position) {
    char name[NAME_LEN];
    contact_t *p_contact = &p_book->data[cur_position];

    printf("姓名：");
    scanf("%s", name);

    if (find_by_name(p_book, name) != -1) {
        printf("name '%s' 已存在于通讯录中\n", name);
        return;
    }

    strcpy(p_contact->name, name);
    printf("年龄：");
    safe_scanf_int(&p_contact->age);
    printf("性别：");
    scanf("%s", p_contact->gender);
    printf("电话：");
    scanf("%s", p_contact->phone);
    printf("地址：");
    scanf("%s", p_contact->address);
}

static int find_by_name(address_book_t *p_book, char *name) {
    for (int i = 0; i < p_book->size; i++) {
        if (strcmp(p_book->data[i].name, name) == 0) {
            return i;
        }
    }
    return -1;
}

static int check_cap(address_book_t *p_book) {
    if (p_book->size < p_book->capacity) {
        return 0;
    }
    int new_cap = p_book->capacity + DEFAULT_INC;
    void * ptr = realloc(p_book->data, new_cap * sizeof(contact_t));
    if (ptr == NULL) {
        perror("扩容失败：");
        return -1;
    }
    p_book->data = (contact_t *)ptr;
    p_book->capacity = new_cap;
    return 0;
}
