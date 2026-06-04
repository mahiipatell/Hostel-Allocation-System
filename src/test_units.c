#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "../include/setting_up.h"
#include "../include/hash.h"
#include "../include/linked_list.h"
#include "../include/queue.h"
#include "../include/stack.h"

int tests_run = 0;
int tests_passed = 0;

#define TEST(name) printf("Testing %-40s", name); tests_run++
#define PASS() printf("PASS\n"); tests_passed++
#define FAIL(msg) printf("FAIL — %s\n", msg)

// ─── HASH TABLE TESTS ───────────────────────────────────────

void test_hash_create() {
    TEST("hash table creation");
    hash_table *ht = create_hash_table(60, 3, 20);
    assert(ht != NULL);
    assert(ht->size == 60);
    assert(ht->floors == 3);
    assert(ht->rooms_per_floor == 20);
    PASS();
}

void test_hash_function() {
    TEST("hash_function valid rooms");
    assert(hash_function(101, 3, 20) == 0);
    assert(hash_function(120, 3, 20) == 19);
    assert(hash_function(201, 3, 20) == 20);
    assert(hash_function(301, 3, 20) == 40);
    assert(hash_function(320, 3, 20) == 59);
    PASS();
}

void test_hash_function_invalid() {
    TEST("hash_function invalid rooms");
    assert(hash_function(999, 3, 20) == -1);
    assert(hash_function(0,   3, 20) == -1);
    assert(hash_function(121, 3, 20) == -1);
    PASS();
}

void test_reverse_hash() {
    TEST("reverse_hash_function");
    assert(reverse_hash_function(0,  20) == 101);
    assert(reverse_hash_function(19, 20) == 120);
    assert(reverse_hash_function(20, 20) == 201);
    assert(reverse_hash_function(40, 20) == 301);
    PASS();
}

void test_append_and_search() {
    TEST("append_to_hash and search_by_merit_no");
    hash_table *fixed = create_hash_table(60, 3, 20);
    hash_table *hold  = create_hash_table(60, 3, 20);

    student_node *node = (student_node *)malloc(sizeof(student_node));
    node->merit_no = 1;
    strcpy(node->name, "Test Student");
    node->mis = 123456789;
    strcpy(node->branch, "computer");
    strcpy(node->category, "open");
    node->cgpa = 9.0;
    node->weighted_cgpa = 1.5;
    node->room_no = 101;
    node->next = NULL;
    node->prev = NULL;

    int index = hash_function(101, 3, 20);
    int result = append_to_hash(fixed, index, node);
    assert(result == 1);
    assert(fixed->table[index] != NULL);
    assert(fixed->table[index]->merit_no == 1);
    PASS();
}

void test_append_invalid_index() {
    TEST("append_to_hash invalid index returns -1");
    hash_table *ht = create_hash_table(60, 3, 20);
    student_node *node = (student_node *)malloc(sizeof(student_node));
    node->next = NULL; node->prev = NULL;
    int result = append_to_hash(ht, -1, node);
    assert(result == -1);
    free(node);
    PASS();
}

void test_remove_from_hash() {
    TEST("remove_from_hash");
    hash_table *ht = create_hash_table(60, 3, 20);
    student_node *node = (student_node *)malloc(sizeof(student_node));
    node->merit_no = 5;
    node->next = NULL; node->prev = NULL;
    append_to_hash(ht, 0, node);
    assert(ht->table[0] != NULL);
    remove_from_hash(ht, 5);
    assert(ht->table[0] == NULL);
    PASS();
}

void test_swap_rooms() {
    TEST("swap_rooms");
    hash_table *ht = create_hash_table(60, 3, 20);

    student_node *a = (student_node *)malloc(sizeof(student_node));
    a->merit_no = 1; a->room_no = 101;
    strcpy(a->name, "Student A");
    a->next = NULL; a->prev = NULL;

    student_node *b = (student_node *)malloc(sizeof(student_node));
    b->merit_no = 2; b->room_no = 201;
    strcpy(b->name, "Student B");
    b->next = NULL; b->prev = NULL;

    append_to_hash(ht, hash_function(101, 3, 20), a);
    append_to_hash(ht, hash_function(201, 3, 20), b);

    swap_rooms(ht, 101, 201);

    assert(ht->table[hash_function(101, 3, 20)]->merit_no == 2);
    assert(ht->table[hash_function(201, 3, 20)]->merit_no == 1);
    PASS();
}

// ─── QUEUE TESTS ────────────────────────────────────────────

void test_queue_init() {
    TEST("queue init and is_empty");
    queue q;
    init_queue(&q);
    assert(is_empty_queue(&q));
    PASS();
}

void test_queue_enqueue_dequeue() {
    TEST("queue enqueue and dequeue");
    queue q;
    init_queue(&q);

    student_node *node = (student_node *)malloc(sizeof(student_node));
    node->merit_no = 10;
    node->next = NULL; node->prev = NULL;

    enqueue(&q, node);
    assert(!is_empty_queue(&q));

    bool success;
    student_node *out = dequeue(&q, &success);
    assert(success);
    assert(out->merit_no == 10);
    assert(is_empty_queue(&q));
    PASS();
}

void test_queue_fifo_order() {
    TEST("queue FIFO order");
    queue q;
    init_queue(&q);

    student_node *a = (student_node *)malloc(sizeof(student_node));
    student_node *b = (student_node *)malloc(sizeof(student_node));
    a->merit_no = 1; a->next = NULL; a->prev = NULL;
    b->merit_no = 2; b->next = NULL; b->prev = NULL;

    enqueue(&q, a);
    enqueue(&q, b);

    bool success;
    assert(dequeue(&q, &success)->merit_no == 1);
    assert(dequeue(&q, &success)->merit_no == 2);
    PASS();
}

// ─── STACK TESTS ────────────────────────────────────────────

void test_stack_init() {
    TEST("stack init and is_empty");
    stack s;
    init_stack(&s, 5);
    assert(is_empty_stack(&s));
    assert(!is_stack_full(&s));
    PASS();
}

void test_stack_push_full() {
    TEST("stack push and is_full");
    stack s;
    init_stack(&s, 2);

    student_node a; a.merit_no = 1;
    student_node b; b.merit_no = 2;

    push(&s, a);
    assert(!is_stack_full(&s));
    push(&s, b);
    assert(is_stack_full(&s));
    PASS();
}

// ─── LINKED LIST TESTS ──────────────────────────────────────

void test_dll_insert_search() {
    TEST("DLL insert and search by MIS");
    student_node_dll dll;
    init_dll(&dll);

    student_node sn;
    sn.merit_no = 1;
    sn.mis = 999888777;
    strcpy(sn.name, "Test");
    sn.next = NULL; sn.prev = NULL;

    insert_end_dll(&dll, sn);

    student_node *found = search_student(&dll, 999888777);
    assert(found != NULL);
    assert(found->merit_no == 1);
    PASS();
}

void test_dll_search_not_found() {
    TEST("DLL search returns NULL for missing MIS");
    student_node_dll dll;
    init_dll(&dll);
    student_node *found = search_student(&dll, 000000001);
    assert(found == NULL);
    PASS();
}

// ─── MAIN ───────────────────────────────────────────────────

int main() {
    printf("\n=== UNIT TESTS ===\n\n");

    // Hash table
    test_hash_create();
    test_hash_function();
    test_hash_function_invalid();
    test_reverse_hash();
    test_append_and_search();
    test_append_invalid_index();
    test_remove_from_hash();
    test_swap_rooms();

    // Queue
    test_queue_init();
    test_queue_enqueue_dequeue();
    test_queue_fifo_order();

    // Stack
    test_stack_init();
    test_stack_push_full();

    // Linked list
    test_dll_insert_search();
    test_dll_search_not_found();

    printf("\n=== RESULTS: %d/%d tests passed ===\n\n", tests_passed, tests_run);
    return (tests_passed == tests_run) ? 0 : 1;
}