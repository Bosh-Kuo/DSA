#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>

bool Recur();
void printBoard(int** board, int N, int M);
void readClue(int** array , int dim1Length);
bool isVaild(int** board, int N, int M, int** valid_row, int** valid_col);
bool isFilled(int** board, int N, int M);
void findEmptyCell(int* cell, int** board, int N, int M);
void painCelltBlack(int** board, int* cell);
void painCelltWhite(int** board, int* cell);
void makeCellEmpty(int** board, int* cell);
bool Recur(int** board, int N, int M, int** valid_row, int** valid_col);
void freeMem(int** state_row, int** state_col, int N, int M);

int main()
{
    clock_t start_t, end_t;
    double total_t;
    int N,M;
    printf("輸入高(N),寬(M):");
    scanf("%d%d",&N,&M);
    //動態記憶體動態記憶體配置宣告 int[N][M]board
    int** board = calloc(N,sizeof(int *));
    for(int i=0;i<N;i++){
        board[i] = calloc(M,sizeof(int));
    }
    //使empty cell= -1
    for(int i=0;i<N;i++){
        for(int j=0;j<M;j++){
            board[i][j] = -1;
        }
    }
    //動態記憶體動態記憶體配置宣告判別矩陣
    int** valid_row = calloc(N,sizeof(int *));
    for(int i=0;i<N;i++){
        valid_row[i] = calloc(M,sizeof(int));
    }
    int** valid_col = calloc(M,sizeof(int *));
    for(int i=0;i<N;i++){
        valid_col[i] = calloc(N,sizeof(int));
    }
    //Input
    printf("Input\n");
    readClue(valid_row,N);
    readClue(valid_col,M);
    //Recursive
    start_t = clock();
    bool succeed = Recur(board,N,M,valid_row,valid_col);
    end_t = clock();
    printf("執行時間：%f\n",(double)(end_t - start_t) / CLOCKS_PER_SEC);
    printf("%d",succeed);
    return 0;
}

void printBoard(int** board, int N, int M){
    for(int i=0; i<N; i++){
        for(int j=0; j<M; j++){
            if(board[i][j]==0)
                printf("_");
            else if(board[i][j]<0)
                printf("-");
            else
                printf("o");
                
            if(j==M-1)
                printf("\n");
        }
    }
}
void readClue(int** array , int dim1Length){
    int tempLen;
    int temp;
    for(int i=0; i<dim1Length; i++){
        scanf("%d",&tempLen);
        for(int j=0; j<tempLen; j++){
            scanf("%d",&temp);
            array[i][j] = temp;
        }
    }
}
bool isVaild(int** board, int N, int M, int** valid_row, int** valid_col){
    int k;
    int** state_row = calloc(N,sizeof(int *));
    for(int i=0;i<N;i++){
        state_row[i] = calloc(M,sizeof(int));
    }
    int** state_col = calloc(M,sizeof(int *));
    for(int i=0;i<M;i++){
        state_col[i] = calloc(N,sizeof(int));
    }
    
    //紀錄與檢查N個rows狀態
    for(int i=0; i<N; i++){
        k = 0;
        for(int j=0;j<M-1;j++){
            if(board[i][j]==1){  //黑
                state_row[i][k]+=1;
                if(board[i][j+1]!=1){  //下一個為白
                    k+=1;
                }
                if(j==M-2 && board[i][j+1]==1){  //最後一個為黑
                    state_row[i][k]+=1;
                }
            }
            else{  //白
                if(j==M-2 && board[i][j+1]==1){  //最後一個為黑
                    state_row[i][k]+=1;
                }
            }
        }
    }
    //紀錄與檢查M個rows狀態
    for(int j=0; j<M; j++){
        k = 0;
        for(int i=0;i<N-1;i++){
            if(board[i][j]==1){  //黑
                state_col[j][k]+=1;
                if(board[i+1][j]!=1){  //下一個為白
                    k+=1;
                }
                if(i==N-2 && board[i+1][j]==1){  //最後一個為黑
                    state_col[j][k]+=1;
                }     
            }
            else{  //白
                if(i==N-2 && board[i+1][j]==1){  //最後一個為黑
                    state_col[j][k]+=1;
                }
            }
        }
    }
    //判斷狀態是否與條件相同
    for(int i=0; i<N; i++){
        for(int j=0; j<M; j++){
            if(state_row[i][j] != valid_row[i][j]){
                freeMem(state_row,state_col,N,M);
                return false;
            }
                
            if(state_col[j][i]!= valid_col[j][i]){ 
                freeMem(state_row,state_col,N,M);
                return false;
            }  
        }
    }
    return true;
}
void freeMem(int** state_row, int** state_col, int N, int M){
    for(int i=0;i<N;i++){
        free(state_row[i]);
    }
    for(int i=1;i<M;i++){
        free(state_col[i]);
    }
    free(state_row);
    free(state_col);
}
bool isFilled(int** board, int N, int M){
    for(int i=0; i<N; i++){
        for(int j=0; j<M; j++){
            if(board[i][j] == -1)
                return false;
        }
    }
    return true;
}
void findEmptyCell(int* cell, int** board, int N, int M){
    for(int i=0;i<N;i++){
        for(int j=0; j<M; j++){
            if(board[i][j] == -1){
                cell[0] = i;
                cell[1] = j;
            }
        }
    }
}
void painCelltBlack(int** board, int* cell){
    int row = cell[0];
    int col = cell[1];
    board[row][col] = 1;
}
void painCelltWhite(int** board, int* cell){
    int row = cell[0];
    int col = cell[1];
    board[row][col] = 0;
}
void makeCellEmpty(int** board, int* cell){
    int row = cell[0];
    int col = cell[1];
    board[row][col] = -1;
}

bool Recur(int** board, int N, int M, int** valid_row, int** valid_col){
    //int cell[2]={0};
    bool succeed;
    bool filled = isFilled(board,N,M);
    bool valid = isVaild(board,N,M,valid_row,valid_col);
    
    if (filled==true){
        if (valid==true){
            printBoard(board,N,M);
            return true;
        }
        else{
            printf(".\n");
            return false;
        }
    }
    else{
        int cell[2]={0};
        findEmptyCell(cell,board,N,M);
        painCelltBlack(board,cell);
        succeed = Recur(board,N,M,valid_row,valid_col);
        if(succeed==true)
            return true;
        painCelltWhite(board,cell);
        succeed = Recur(board,N,M,valid_row,valid_col);
        if(succeed==true)
            return true;
        makeCellEmpty(board,cell);
    }
}