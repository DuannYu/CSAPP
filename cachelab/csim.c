#include "cachelab.h"
#include <stdio.h>
#include <getopt.h> 
#include <stdlib.h> 
#include <unistd.h>
#include <string.h>

#define uint unsigned int
#define BOOL int
#define TRUE 1
#define FALSE 0
#define VALID 0
#define TAG 1
#define LRU 2
#define MAX_LINE 200
#define INF 0x3f3f3f3f

char path[MAX_LINE];
struct _cache {
    uint sets;      // 组数
    uint lines;     // 行数
    uint blocks;    // 块大小
    uint ***info;   // 第一维表示组数，第二维表示每组的行数，第三维就是每行的三个值有效位，标志位以及LRU值{S, E, 3}
} cache;

void print_help_info();                     // 打印帮助信息
void init_cache(int argc, char *argv[]);    // 缓冲区初始化
void free_cache();                          // 清除缓冲区内存

void execute_instruction();                 // 处理指令
void check_and_solve(uint addr);            // 处理load指令
void modify(uint addr);                     // 处理modify指令
void store(uint addr);                      // 处理store指令
void replace(uint _s, uint _E, uint tag);   //
uint get_min_LRU(uint _s);                  // 获取最小的LRU
int is_hit(uint _s, uint _tag);             // 判断是否命中，命中返回行数

uint get_valid(uint _sets, uint _E);        // 获取valid值
uint get_tag(uint _sets, uint _E);          // 获取tag值
uint get_LRU(uint _sets, uint _E);          // 获取LRU值

BOOL is_verbose = FALSE;    // 是否打印信息
uint set_bits = 0;          // 组数的位信息
uint lines = 0;             // 行数的位信息
uint block_bits = 0;        // 块数的位信息
FILE *fp = NULL;            // 文件指针

int hit_count = 0;          // 命中数
int miss_count = 0;         // 未命中数
int eviction_count = 0;     // 重写数

int main(int argc, char *argv[]) {
    // 1. initialization
    init_cache(argc, argv);
    // 2. execute instructions
    execute_instruction();
    // 3. print result
    printSummary(hit_count, miss_count, eviction_count);
    // 4. free cache
    free_cache();

    return 0;
}

/**
 * print_help_info: print help message for users.
 */
void print_help_info() {
    printf("Usage: ./csim-ref [-hv] -s <num> -E <num> -b <num> -t <file>\n");
    printf("Options:\n");
    printf("  -h         Print this help message.\n");
    printf("  -v         Optional verbose flag.\n");
    printf("  -s <num>   Number of set index bits.\n");
    printf("  -E <num>   Number of lines per set.\n");
    printf("  -b <num>   Number of block offset bits.\n");
    printf("  -t <file>  Trace file.\n");
    printf("Examples:\n");
    printf("  linux>  ./csim-ref -s 4 -E 1 -b 4 -t traces/yi.trace\n");
    printf("  linux>  ./csim-ref -v -s 8 -E 2 -b 4 -t traces/yi.trace\n");
}

/**
 * cache initialization
 */
void init_cache(int argc, char *argv[]) {
    // get program's arguments
    int opt;
    char *string = "hvs:E:b:t:";
    while ((opt = getopt(argc, argv, string)) != -1) {
        switch (opt) {
            case 'h':
                print_help_info(); break;
            case 'v':
                is_verbose = TRUE; break;
            case 's':
                set_bits = atol(optarg); break;
            case 'E':
                lines = atol(optarg); break;
            case 'b':
                block_bits = atol(optarg); break;
            case 't':
                fp = fopen(optarg, "r");
                break;
            default:
                print_help_info();
        }
    }

    // initialize cache
    cache.sets = 1 << set_bits;
    cache.lines = lines;
    cache.blocks = 1 << block_bits;

    // malloc
    cache.info = (uint ***)malloc(cache.sets * sizeof(uint **));
    for (int i = 0; i < cache.sets; i++)
        cache.info[i] = (uint **)malloc(cache.sets * sizeof(uint *));
    for (int i = 0; i < cache.sets; i++)
        for (int j = 0; j < cache.lines; j++)
            cache.info[i][j] = (uint *)malloc(3 * sizeof(uint));

    // initialize valid, tag and lru
    for (int i = 0; i < cache.sets; i++)
        for (int j = 0; j < cache.lines; j++) {
            cache.info[i][j][0] = FALSE;    // valid
            cache.info[i][j][1] = 0;        // tag
            cache.info[i][j][2] = INF;      // LRU
        }
}



/**
 * execute instructions from files.
 */
void execute_instruction() {
    if (fp == NULL) {
        fprintf(stderr, "open file error\n");
        exit(0);
    }

    char op;
    uint address;
    while((fscanf(fp, " %c %x%*[^\n]", &op, &address)) == 2) {
        if (op == 'L' || op == 'S')
            check_and_solve(address);
        else if (op == 'M') {
            check_and_solve(address);
            check_and_solve(address);
        }
        printf("\n");
    }
}
/**
 * free cache
 */
void free_cache() {
    for (int i = 0; i < cache.sets; i++)
        for (int j = 0; j < cache.lines; j++)
            free(cache.info[i][j]);

    for (int i = 0; i < cache.sets; i++)
        free(cache.info[i]);

    free(cache.info);
}

uint get_valid(uint _sets, uint _E) { return cache.info[_sets][_E][VALID]; }
uint get_tag(uint _sets, uint _E) { return cache.info[_sets][_E][TAG]; }
uint get_LRU(uint _sets, uint _E) { return cache.info[_sets][_E][LRU]; }

void check_and_solve(uint addr) {

    uint set_index_mask = (1 << set_bits) - 1;
    uint _s = (addr >> block_bits) & set_index_mask;
    uint tag = (addr >> block_bits) >> set_bits;

    int line = is_hit(_s, tag);
    if (line == -1) {   // 未命中
        miss_count++;
        if (is_verbose == TRUE) printf("miss ");
        replace(_s, get_min_LRU(_s), tag);
    } else {            // 命中
        hit_count++;
        replace(_s, line, tag);
        if (is_verbose == TRUE) printf("hit ");
    }

}

void replace(uint _s, uint _E, uint tag) {
    if (get_valid(_s, _E) == TRUE && get_tag(_s, _E) != tag) {
        eviction_count++;
    }

    cache.info[_s][_E][VALID] = TRUE;
    cache.info[_s][_E][TAG] = tag;
    cache.info[_s][_E][LRU] = INF;

    for (int line = 0; line < cache.lines; line++)
        cache.info[_s][line][LRU]--;

}

uint get_min_LRU(uint _s) {
    uint lru = INF, pos = 0;
    for (int line = 0; line < cache.lines; line++) {
        if (get_valid(_s, line) == FALSE)
            return line;
        if (lru > get_LRU(_s, line)) {
            lru = get_LRU(_s, line);
            pos = line;
        }
    }
    return pos;
}

int is_hit(uint _s, uint _tag) {
    for (int i = 0; i < cache.lines; i++)
        if (cache.info[_s][i][VALID] == TRUE && cache.info[_s][i][TAG] == _tag) {
            return i;
        }

    return -1;
}

