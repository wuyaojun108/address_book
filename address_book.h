#ifndef __ADDRESS_BOOK_H__
#define __ADDRESS_BOOK_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>

#define NAME_LEN 20
#define GENDER_LEN 5
#define PHONE_LEN 15
#define ADDRESS_LEN 50
#define DEFAULT_CAP 3
#define DEFAULT_INC 3   // 每次扩容默认加3

// 联系人
typedef struct contact {
    char name[NAME_LEN];
    short age;
    char gender[GENDER_LEN];
    char phone[PHONE_LEN];
    char address[ADDRESS_LEN];
} contact_t;

// 通讯录
typedef struct address_book {
    contact_t *data;
    int size;
    int capacity;
} address_book_t;

void handle_interrupt(int sig);

int init_address_book(address_book_t *p_book);

void add_contact(address_book_t *p_book);

void del_contact(address_book_t *p_book);

void update_contact(address_book_t *p_book);

void show_contact(address_book_t *p_book);

void list_contact(address_book_t *p_book);

void sort_contact(address_book_t *p_book);

void clear_address_book(address_book_t *p_book);

void save_address_book(address_book_t *p_book);

int load_address_book(address_book_t *p_book);

void safe_scanf_int(void *input);

#endif  // __ADDRESS_BOOK_H__
