#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <vector>

using namespace std;

#define FILENAME 10

FILE *fp;

vector<int> h_wall; //가로벽
vector<int> v_wall; //세로벽
vector<int> maze; //전체 배열

int N, M; //미로 너비, 높이
int room_id = 0; //방에 적힌 숫자
int before_wall = -1;
char filename[FILENAME] = "out.maz";

void print_outline() { // +-+-+-+
    for (int i = 0; i < N; i++)
        fprintf(fp, "+-"); //+는 방의 모서리, -는 벽
    fprintf(fp, "+\n");
}

void print_v_wall() { // 세로벽 그리기
    fprintf(fp, "|");
    for (int i = 0; i < N - 1; i++) {
        fprintf(fp, " ");
        if (v_wall[i] == 1) 
            fprintf(fp, "|");
        else 
            fprintf(fp, " ");
    }
    fprintf(fp, " |\n");
}

void print_h_wall() { // 가로벽 그리기
    fprintf(fp, "+");
    for (int i = 0; i < N; i++) {
        if (h_wall[i] == 1) 
            fprintf(fp, "-+");
        else 
            fprintf(fp, " +");
    }
    fprintf(fp, "\n");
} 

//벽의 유무 결정
void connect_ver() {
    int before_num = maze[0];
    int connect = 0;
    
    for (int i = 0; i < N; i++) {
        h_wall[i] = rand() % 2;
        if (h_wall[i] == 0)
            connect = 1;

        if (i < N - 1) {
            if (before_num != maze[i + 1]) { 
                if (connect == 0) 
                    h_wall[i] = 0;
                else 
                    connect = 0;
                before_num = maze[i + 1];
            }
        }
        if ((i == N - 1) && (connect == 0))
            h_wall[i] = 0;

        if (h_wall[i]) {
            maze[i] = room_id;
            room_id++;
        }
    }
}

void connect_room() {
    int connect = 0;
    int before_num;

    for (int i = 0; i < N - 1; i++) {
        if (maze[i] != maze[i + 1]) {
            connect = rand() % 2; //만날지 여부 결정
            if (connect) {
                before_num = maze[i + 1];
                maze[i + 1] = maze[i];
                for (int j = 0; j < N; j++)
                    if (maze[j] == before_num) 
                        maze[j] = maze[i];
                v_wall[i] = 0;
            } else 
                v_wall[i] = 1;
        } else 
            v_wall[i] = 1;
    }
}

void connect_last() {
    int before_num;
    for (int i = 0; i < N - 1; i++) {
        if (maze[i] != maze[i + 1]) {
            v_wall[i] = 0;
            before_num = maze[i + 1];
            maze[i + 1] = maze[i];
            for (int j = 0; j < N; j++) {
                if (maze[j] == before_num) 
                    maze[j] = maze[i];
            }
        } else v_wall[i] = 1;
    }
}

int main() {
    fp = fopen(filename, "wt");
    //미로의 너비, 높이 입력받기
    scanf("%d %d", &N, &M);

    //given N, M에 따라서 가로벽, 세로벽, 미로 사이즈 조정
    h_wall.resize(N);
    v_wall.resize(N-1);
    maze.resize(N, 0);

    print_outline();

    for (int i = 0; i < N - 1; i++) {
        v_wall[i] = rand() % 2; //0 아니면 1
        //1일 경우, 막혀 있다는 의미 -> 그 이전 칸까지 모두 뚫기
        if (v_wall[i] == 1) { 
            for (int j = (before_wall + 1); j <= i; j++)
                maze[j] = room_id; //방에 적힌 숫자를 같게 함
            before_wall = i; // 이전 벽 위치로 돌아가기
            room_id++; // 이제 다음 집합에 적힌 숫자를 1씩 증가
        }
    }
    for (int i = before_wall + 1; i < N; i++)
        maze[i] = room_id; //나머지 곳도 숫자 맞추기.
    room_id++;

    print_v_wall();

    for (int k = 1; k < M - 1; k++) { 
        connect_ver();
        print_h_wall();
        connect_room();
        print_v_wall();
    }

    //마지막 line
    connect_ver();
    print_h_wall();
    connect_last();
    print_v_wall();
    print_outline();
    return 0;
}
