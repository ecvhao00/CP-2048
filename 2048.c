#define _CRT_SECURE_NO_WARNINGS
#pragma execution_character_set("utf-8")
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <conio.h>
#include <windows.h>

#define size 4

int full();
int score();
void over();
void input(char c);
void left();
void right();
void up();
void down();
void print_title();
void game_description();
void game_exit();

int board[size][size] = { 0 };
int move_count = 0;

// 커서 이동 함수
void gotoxy(int x, int y) {
    COORD Pos = { x, y };
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), Pos);
}

// 테두리 맵 (17x17 고정)
int map[17][17] = {
    {6,1,1,1,22,1,1,1,22,1,1,1,22,1,1,1,7},
    {2 ,0,0,0, 2,0,0,0, 2,0,0,0, 2,0,0,0, 2},
    {2 ,5,4,5, 2,5,4,5,2,5,4,5, 2,5,4,5, 2},
    {2 ,0,0,0, 2,0,0,0,2,0,0,0, 2,0,0,0, 2},
    {10,1,1,1, 3,1,1,1,3,1,1,1, 3,1,1,1,12},
    {2 ,0,0,0, 2,0,0,0,2,0,0,0, 2,0,0,0, 2},
    {2 ,5,4,5, 2,5,4,5,2,5,4,5, 2,5,4,5, 2},
    {2 ,0,0,0, 2,0,0,0,2,0,0,0, 2,0,0,0, 2},
    {10,1,1,1,3,1,1,1,3,1,1,1,3,1,1,1,12},
    {2 ,0,0,0, 2,0,0,0, 2,0,0,0, 2,0,0,0, 2},
    {2 ,5,4,5, 2,5,4,5,2,5,4,5, 2,5,4,5, 2},
    {2 ,0,0,0, 2,0,0,0,2,0,0,0, 2,0,0,0, 2},
    {10,1,1,1,3,1,1,1,3,1,1,1,3,1,1,1,12},
    {2 ,0,0,0, 2,0,0,0, 2,0,0,0, 2,0,0,0, 2},
    {2 ,5,4,5, 2,5,4,5,2,5,4,5, 2,5,4,5, 2},
    {2 ,0,0,0, 2,0,0,0,2,0,0,0, 2,0,0,0, 2},
    {9,1,1,1,13,1,1,1,13,1,1,1,13,1,1,1,8},

};

// 맵 출력
void draw_map() {
    system("cls");
    for (int i = 0; i < 17; i++) {
        for (int j = 0; j < 17; j++) {
            switch (map[i][j]) {
            case 0: printf("   "); break;
            case 1: printf("───"); break;
            case 2: printf("│"); break;
            case 3: printf("┼"); break;
            case 5: printf("    "); break;
            case 6: printf("┌"); break;
            case 7: printf("┐"); break;
            case 8: printf("┘"); break;
            case 9: printf("└"); break;
            case 10: printf("├"); break;
            case 22: printf("┬"); break;
            case 12: printf("┤"); break;
            case 13: printf("┴"); break;
            default: printf(" "); break;
            }
        }
        printf("\n");
    }
}

// 숫자 배열 출력
void draw_numbers() {
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            int y = 2 + i * 4;  // 각 셀 중앙 세로 위치
            int x = 4 + j * 9;  // 각 셀 중앙 가로 위치

            gotoxy(x, y);  // 중앙 정렬을 위한 미세 조정

            if (board[i][j] == 0)
                printf("    "); // 빈 칸
            else if (board[i][j] == -1)
                printf("  ★ ");
            else if (board[i][j] == -2)
                printf("  ÷ ");
            else if (board[i][j] == -3)
                printf("  X ");
            else
                printf("%3d", board[i][j]); // 일반 숫자
        }
    }
}




// 랜덤 일반 타일 생성
void add_random_tile() {
    int empty[16][2], count = 0;
    for (int i = 0; i < size; i++)
        for (int j = 0; j < size; j++)
            if (board[i][j] == 0) {
                empty[count][0] = i, empty[count][1] = j;
                count++;
            }

    if (count == 0) return;
    int r = rand() % count;
    int value = (rand() % 10 < 8) ? 2 : 4;
    board[empty[r][0]][empty[r][1]] = value;
}

// 특수 타일 생성
void add_special_tile() {
    int empty[16][2], count = 0;
    for (int i = 0; i < size; i++)
        for (int j = 0; j < size; j++)
            if (board[i][j] == 0) {
                empty[count][0] = i, empty[count][1] = j;
                count++;
            }
    if (count == 0) return;
    int r = rand() % count;
    int type = -1 * (1 + rand() % 3);
    board[empty[r][0]][empty[r][1]] = type;
}

// 게임 초기화
void init_game() {

    system("chcp 65001 > nul");

    srand((unsigned)time(NULL));

    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            board[i][j] = 0;
        }
    }
    move_count = 0;
    add_random_tile();
    add_random_tile();
    draw_map();
    draw_numbers();
}

// 전체 게임 루프
void start() {
    init_game();

    while (1) {
        if (full()) {
            draw_map();
            draw_numbers();
            over();
        }

        char c = _getch();
        input(c);

        move_count++;

        if (move_count % 8 == 0)
            add_special_tile();
        else
            add_random_tile();

        draw_map();
        draw_numbers();
    }
}
int main() {
    SetConsoleOutputCP(CP_UTF8);
    int choice = 0;

    while (1) {
        system("cls");
        print_title();

        printf("\n ▶ 메뉴를 선택하세요 (1~3): ");
        choice = _getch();

        switch (choice) {
        case '1':
            start();
            break;
        case '2':
            game_description();
            break;
        case '3':
            game_exit();
            return 0;
        default:
            printf("\n 잘못된 입력입니다. 아무 키나 누르세요.");
            (void)_getch();
        }
    }

    return 0;
}

void print_title() {
    printf("===================================\n");
    printf(" ::::::::   :::::::      :::      ::::::::  \n"
        ":+:    :+: :+:   :+:    :+:      :+:    :+: \n"
        "      +:+  +:+  :+:+   +:+ +:+   +:+    +:+ \n"
        "    +#+    +#+ + +:+  +#+  +:+    +#++:++#  \n"
        "  +#+      +#+#  +#+ +#+#+#+#+#+ +#+    +#+ \n"
        " #+#       #+#   #+#       #+#   #+#    #+# \n"
        "##########  #######        ###    ########  \n");
    printf("===================================\n");
    printf("1. 게임 시작 \n");
    printf("2. 게임 설명 \n");
    printf("3. 나가기 \n");
    printf("===================================\n");
}

void game_description() {
    system("cls");
    printf("========== 게임 설명 ==========\n");
    printf("▶ 같은 숫자의 타일을 합쳐\n");
    printf("   2048 타일을 만드는 게임입니다.\n");
    printf("▶ 'w,a,s,d로 타일을 이동시키세요.\n");
    printf("▶ 합쳐질 수 있는 같은 숫자가\n");
    printf("   만나면 자동으로 합쳐집니다.\n");
    printf("▶★(2배), ÷(2로 나누기), X(해당 줄 삭제) \n");
    printf(" 와 같은 특수 타일을 이용하여 높은 점수를 노려보세요!\n");
    printf("===============================\n");
    printf("\n아무 키나 누르면 메뉴로 돌아갑니다...");
    (void)_getch();  // 반환값 무시 경고 방지
}

void game_exit() {
    printf("\n게임을 종료합니다. 감사합니다!\n");
    Sleep(1000);  // 종료 전 1초 대기
}





// 게임판이 다 찼는지 확인
int full() {
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            if (board[i][j] == 0)
                return 0;
        }
    }
    return 1;
}

// 현재 점수 계산
int score() {
    int sum = 0;
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            if (board[i][j] > 0)
                sum += board[i][j];
        }
    }
    return sum;
}

// 게임 오버 처리
void over() {
    int s = score();
    printf("\n게임 끝! 점수는 %d\n", s);

    printf("다시 시작할래요? (y/n): ");
    char ch;
    scanf(" %c", &ch);

    if (ch == 'y' || ch == 'Y') {
        start(); // 다시 시작
    }
    else {
        exit(0); // 종료
    }
}

// 특별 타일 적용 함수
void special_tile_function(int row, int col, int* last, int now, int* temp, int* t, int direction) {
    int value = *last;

    switch (now) {
    case -1:
        value *= 2;
        break;

    case -2:
        if (value % 2 == 0) {
            value /= 2;
        }
        else {
            temp[*t] = value;
            *last = now;
            (direction == 0 || direction == 2) ? (*t)++ : (*t)--;
            return;
        }
        break;

    case -3:
        if (direction == 0 || direction == 1) {
            for (int j = 0; j < size; j++) board[row][j] = 0;
        }
        else {
            for (int i = 0; i < size; i++) board[i][col] = 0;
        }
        for (int k = 0; k < size; k++) temp[k] = 0;

        *last = 0;
        *t = (direction == 0 || direction == 2) ? 0 : size - 1;
        return;
    }
    temp[*t] = value;
    (direction == 0 || direction == 2) ? (*t)++ : (*t)--;
    *last = 0;
}



// 왼쪽으로 블록 이동
void left() {
    for (int i = 0; i < size; i++) {
        int temp[size] = { 0 };
        int t = 0;
        int last = 0;

        for (int j = 0; j < size; j++) {
            int now = board[i][j];
            if (now == 0) continue;

            if ((last > 0 && now < 0)) {
                special_tile_function(i, j, &last, now, temp, &t, 0);
                continue;
            }
            else if ((last < 0 && now > 0)) {
                special_tile_function(i, j, &now, last, temp, &t, 0);
                last = 0;
                continue;
            }


            if (last == 0) {
                last = now;
            }
            else if (now > 0 && last == now) {
                temp[t++] = last + now;
                last = 0;
            }
            else if (now > 0) {
                temp[t++] = last;
                last = now;
            }
        }
        if (last != 0) temp[t] = last;

        for (int j = 0; j < size; j++) {
            board[i][j] = temp[j];
        }
    }
}

// 오른쪽으로 블록 이동
void right() {
    for (int i = 0; i < size; i++) {
        int temp[size] = { 0 };
        int t = size - 1;
        int last = 0;

        for (int j = size - 1; j >= 0; j--) {
            int now = board[i][j];
            if (now == 0) continue;

            if ((last > 0 && now < 0)) {
                special_tile_function(i, j, &last, now, temp, &t, 1);
                continue;
            }
            else if ((last < 0 && now > 0)) {
                special_tile_function(i, j, &now, last, temp, &t, 1);
                last = 0;
                continue;
            }

            if (last == 0) {
                last = now;
            }
            else if (last == now) {
                temp[t--] = last + now;
                last = 0;
            }
            else {
                temp[t--] = last;
                last = now;
            }
        }
        if (last != 0) temp[t] = last;

        for (int j = 0; j < size; j++) {
            board[i][j] = temp[j];
        }
    }
}

// 위로 블록 이동
void up() {
    for (int j = 0; j < size; j++) {
        int temp[size] = { 0 };
        int t = 0;
        int last = 0;

        for (int i = 0; i < size; i++) {
            int now = board[i][j];
            if (now == 0) continue;

            if ((last > 0 && now < 0)) {
                special_tile_function(i, j, &last, now, temp, &t, 2);
                continue;
            }
            else if ((last < 0 && now > 0)) {

                special_tile_function(i, j, &now, last, temp, &t, 2);
                last = 0;
                continue;
            }

            if (last == 0) {
                last = now;
            }
            else if (last == now) {
                temp[t++] = last + now;
                last = 0;
            }
            else {
                temp[t++] = last;
                last = now;
            }
        }
        if (last != 0) temp[t] = last;

        for (int i = 0; i < size; i++) {
            board[i][j] = temp[i];
        }
    }
}

// 아래로 블록 이동
void down() {
    for (int j = 0; j < size; j++) {
        int temp[size] = { 0 };
        int t = size - 1;
        int last = 0;

        for (int i = size - 1; i >= 0; i--) {
            int now = board[i][j];
            if (now == 0) continue;

            if ((last > 0 && now < 0)) {
                special_tile_function(i, j, &last, now, temp, &t, 3);
                continue;
            }
            else if ((last < 0 && now > 0)) {
                special_tile_function(i, j, &now, last, temp, &t, 3);
                last = 0;
                continue;
            }

            if (last == 0) {
                last = now;
            }
            else if (last == now) {
                temp[t--] = last + now;
                last = 0;
            }
            else {
                temp[t--] = last;
                last = now;
            }
        }
        if (last != 0) temp[t] = last;

        for (int i = 0; i < size; i++) {
            board[i][j] = temp[i];
        }
    }
}

// 입력 처리
void input(char c) {
    if (c == 'a') left();
    else if (c == 'd') right();
    else if (c == 'w') up();
    else if (c == 's') down();
}