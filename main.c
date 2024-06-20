#include "address_book.h"

enum option  {
    EXIT = 0,
    ADD,
    DELETE,
    UPDATE,
    VIEW,
    LIST,
    SORT,
    CLEAR,
    SAVE
};

static void menu();

int main() {  // int argc, char const *argv[]
    int opt_result, input_opt = 0;

    // 设置SIGINT的信号处理函数  
    if (signal(SIGINT, handle_interrupt) == SIG_ERR) {  
        perror("signal");
        exit(1);  
    } 

    address_book_t book;
    opt_result = load_address_book(&book);
    if (opt_result == -1) {
        printf("加载失败，程序退出\n");
        return -1;
    }

    while (1) {
        menu();
        printf("请输入: ");
        safe_scanf_int(&input_opt);

        switch (input_opt) {
        case ADD:
            add_contact(&book);
            break;
        case DELETE:
            del_contact(&book);
            break;
        case UPDATE:
            update_contact(&book);
            break;
        case VIEW:
            show_contact(&book);
            break;
        case LIST:
            list_contact(&book);
            break;
        case SORT:
            sort_contact(&book);
            break;
        case CLEAR:
            clear_address_book(&book);
            break;
        case SAVE:
            save_address_book(&book);
            break;
        case EXIT:
            save_address_book(&book);
            return 0;
        default:
            printf("输入不正确，请重新输入\n");
            break;
        }
    }

    return 0;
}

static void menu() {
    printf("-----菜单-----\n");
    printf("1. 新增\n");
    printf("2. 删除\n");
    printf("3. 修改\n");
    printf("4. 查看\n");
    printf("5. 列表\n");
    printf("6. 排序\n");
    printf("7. 清空通讯录\n");
    printf("8. 保存通讯录到磁盘\n");
    printf("0. 退出\n");
    printf("---------------\n");
}
