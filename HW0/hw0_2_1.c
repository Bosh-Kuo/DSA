#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

typedef struct Board{
    int row;
    int col;
    int** cell;
    int** status_rows;
    int** status_cols;
    int** valid_rows;
    int** valid_columns;
}board;

void readAndInit(board* theBoard, int row, int col);
void readClue(board* theBoard);
void printBoard(board* theBoard);
bool isVaild(board* theBoard);
void freeMem(board* theBoard);
bool isFilled(board* theBoard);
void findEmptyCell(board* theBoard, int* nextCell);
void paintCelltBlack(board* theBoard, int* nextCell);
void painCelltWhite(board* theBoard, int* nextCell);
void makeCellEmpty(board* theBoard, int* nextCell);
bool Recur(board* theBoard);

int main()
{
    clock_t start_t, end_t;
    double total_t;
    int row,col;
    printf("輸入高(N),寬(M):");
    scanf("%d%d",&row,&col);
    board theBoard;
    readAndInit(&theBoard,row,col);
    //Input
    printf("Input\n");
    readClue(&theBoard);
    //Recursive
    start_t = clock();
    bool succeed = Recur(&theBoard);
    end_t = clock();
    printf("執行時間：%f\n",(double)(end_t - start_t) / CLOCKS_PER_SEC);
    //printClue(&theBoard);
    freeMem(&theBoard);
    //printf("%d\n",succeed);
    return 0;
}
void readAndInit(board* theBoard, int row, int col){
    theBoard->row = row;
    theBoard->col = col;
    theBoard->cell = calloc(row,sizeof(int *));
    theBoard->valid_rows = calloc(row,sizeof(int *));
    theBoard->valid_columns = calloc(col,sizeof(int *));
    theBoard->status_rows = calloc(row,sizeof(int *));
    theBoard->status_cols = calloc(col,sizeof(int *));

    for(int i=0; i<row; i++){
        theBoard->cell[i] = calloc(col,sizeof(int));
        theBoard->valid_rows[i] = calloc(col,sizeof(int));
        theBoard->status_rows[i] = calloc(col,sizeof(int));
    }
    for(int j=0; j<col; j++){
        theBoard->valid_columns[j] = calloc(row,sizeof(int));
        theBoard->status_cols[j] = calloc(row,sizeof(int));
    }
    //（初始化）使所有cell一開始都為-1(尚未填色)
    for(int i=0; i<row; i++){
        for(int j=0;j<col;j++){
            theBoard->cell[i][j] = -1;
        }
    }
}
void readClue(board* theBoard){
    int tempLen;
    int temp;
    for(int i=0; i<theBoard->row; i++){
        scanf("%d",&tempLen);
        for(int j=0; j<tempLen; j++){
            scanf("%d",&temp);
            theBoard->valid_rows[i][j] = temp;
        }
    }
    for(int i=0; i<theBoard->col; i++){
        scanf("%d",&tempLen);
        for(int j=0; j<tempLen; j++){
            scanf("%d",&temp);
            theBoard->valid_columns[i][j] = temp;
        }
    }
}
void printBoard(board* theBoard){
    for(int i=0; i<theBoard->row; i++){
        for(int j=0; j<theBoard->col; j++){
            if(theBoard->cell[i][j]==0)
                printf("_");
            else if(theBoard->cell[i][j]<0)
                printf("-");
            else
                printf("o");
                
            if(j==theBoard->col-1)
                printf("\n");
        }
    }
}

bool isVaild(board* theBoard){
    //初始化status
    for(int i=0; i<theBoard->row; i++){
        for(int j=0; j<theBoard->col;j++){
            theBoard->status_rows[i][j]=0;
            theBoard->status_cols[j][i]=0;
        }
    }
    int k;
    //紀錄與檢查N個rows狀態
    for(int i=0; i<theBoard->row; i++){
        k = 0;
        for(int j=0;j<theBoard->col-1;j++){
            if(theBoard->cell[i][j]==1){  //黑
                theBoard->status_rows[i][k]+=1;
                if(theBoard->cell[i][j+1]!=1){  //下一個為白
                    k+=1;
                }
                if(j==theBoard->col-2 && theBoard->cell[i][j+1]==1){  //最後一個為黑
                    theBoard->status_rows[i][k]+=1;
                }
            }
            else{  //白
                if(j==theBoard->col-2 && theBoard->cell[i][j+1]==1){  //最後一個為黑
                    theBoard->status_rows[i][k]+=1;
                }
            }
        }
        for(int j=0; j<theBoard->col; j++){
            if(theBoard->status_rows[i][j] != theBoard->valid_rows[i][j]){
                return false;
            }
        }
    }
    //紀錄與檢查M個cols狀態
    for(int j=0; j<theBoard->col; j++){
        k = 0;
        for(int i=0; i<theBoard->row-1; i++){
            if(theBoard->cell[i][j]==1){  //黑
                theBoard->status_cols[j][k]+=1;
                if(theBoard->cell[i+1][j]!=1){  //下一個為白
                    k+=1;
                }
                if(i==theBoard->row-2 && theBoard->cell[i+1][j]==1){  //最後一個為黑
                    theBoard->status_cols[j][k]+=1;
                }     
            }
            else{  //白
                if(i==theBoard->row-2 && theBoard->cell[i+1][j]==1){  //最後一個為黑
                    theBoard->status_cols[j][k]+=1;
                }
            }
        }
        for(int i=0; i<theBoard->row; i++){
            if(theBoard->status_cols[j][i] != theBoard->valid_columns[j][i]){
                return false;
            }
        }
    }
    return true;
}
//start from here!!!!!!!!!!!!!
void freeMem(board* theBoard){
    for(int i=0;i<theBoard->row;i++){
        free(theBoard->cell[i]);
        free(theBoard->status_rows[i]);
        free(theBoard->valid_rows[i]);
    }
    for(int i=0;i<theBoard->col;i++){
        free(theBoard->status_cols[i]);
        free(theBoard->valid_columns[i]);
    }
    free(theBoard->cell);
    free(theBoard->status_cols);
    free(theBoard->status_rows);
    free(theBoard->valid_columns);
    free(theBoard->valid_rows);
}
bool isFilled(board* theBoard){
    for(int i=0; i<theBoard->row; i++){
        for(int j=0; j<theBoard->col; j++){
            if(theBoard->cell[i][j] == -1)
                return false;
        }
    }
    return true;
}
void findEmptyCell(board* theBoard, int* nextCell){
    for(int i=0;i<theBoard->row;i++){
        for(int j=0; j<theBoard->col; j++){
            if(theBoard->cell[i][j] == -1){
                nextCell[0] = i;
                nextCell[1] = j;
            }
        }
    }
}
void paintCelltBlack(board* theBoard, int* nextCell){
    theBoard->cell[nextCell[0]][nextCell[1]] = 1;
}
void paintCelltWhite(board* theBoard, int* nextCell){
    theBoard->cell[nextCell[0]][nextCell[1]] = 0;
}
void makeCellEmpty(board* theBoard, int* nextCell){
    theBoard->cell[nextCell[0]][nextCell[1]] = -1;
}

bool Recur(board* theBoard){
    bool succeed;
    if (isFilled(theBoard)){
        if (isVaild(theBoard)){
            printBoard(theBoard);
            return true;
        }
        else{
            return false;
        }
    }
    else{
        int nextCell[2]={0};
        findEmptyCell(theBoard,nextCell);
        paintCelltBlack(theBoard,nextCell);
        succeed = Recur(theBoard);
        if(succeed==true)
            return true;
        paintCelltWhite(theBoard,nextCell);
        succeed = Recur(theBoard);
        if(succeed==true)
            return true;
        makeCellEmpty(theBoard,nextCell);
    }
    return false;
}