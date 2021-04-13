#include "tetris.h"

static struct sigaction act, oact;

int main(){
	int exit=0;

	initscr();
	noecho();
	keypad(stdscr, TRUE);

	srand((unsigned int)time(NULL));

	while(!exit){
		clear();
		switch(menu()){
		case MENU_PLAY: play(); break;
		case MENU_EXIT: exit=1; break;
		default: break;
		}
	}

	endwin();
	system("clear");
	return 0;
}

//2주차 수정한 부분
void InitTetris(){
	int i,j;

	for(j=0;j<HEIGHT;j++)
		for(i=0;i<WIDTH;i++)
			field[j][i]=0;
	//블록의 크기가 2->3으로 증가되었기 때문에 다음 블록의 id를 임의로 생성한다.
	for(i = 0; i < BLOCK_NUM; i++)
		nextBlock[i] = rand()%7;

	blockRotate=0;
	blockY=-1;
	blockX=WIDTH/2-2;
	score=0;
	gameOver=0;
	timed_out=0;

	DrawOutline();
	DrawField();
	DrawBlockWithFeatures(blockY,blockX,nextBlock[0],blockRotate);
	DrawNextBlock(nextBlock);
	PrintScore(score);
}

void DrawOutline(){
	int i,j;
	/* 블럭이 떨어지는 공간의 테두리를 그린다.*/
	DrawBox(0,0,HEIGHT,WIDTH);

	/* next block을 보여주는 공간의 테두리를 그린다.*/
	move(2,WIDTH+10);
	printw("NEXT BLOCK");
	for(i = 0; i < BLOCK_NUM-1; i++)
		DrawBox(3+i*6, WIDTH+10, 4, 8);

	/* score를 보여주는 공간의 테두리를 그린다.*/
	move(3+(BLOCK_NUM-1)*6,WIDTH+10);
	printw("SCORE");
	DrawBox(4+(BLOCK_NUM-1)*6,WIDTH+10,1,8);
}

int GetCommand(){
	int command;
	command = wgetch(stdscr);
	switch(command){

		case KEY_UP:
			break;
		case KEY_DOWN:
			break;
		case KEY_LEFT:
			break;
		case KEY_RIGHT:
			break;
		case ' ':	/* space key*/
			command = FALL;
			break;
		case 'q':
		case 'Q':
			command = QUIT;
			break;
		default:
			command = NOTHING;
			break;
	}
	return command;
}

int ProcessCommand(int command){
	int ret=1;
	int drawFlag=0;
	switch(command){
		case QUIT:
			ret = QUIT;
			break;

		case KEY_UP:
			if((drawFlag = CheckToMove(field,nextBlock[0],(blockRotate+1)%4,blockY,blockX)))
				blockRotate=(blockRotate+1)%4;
			break;
		case KEY_DOWN:
			if((drawFlag = CheckToMove(field,nextBlock[0],blockRotate,blockY+1,blockX)))
				blockY++;
			break;
		case KEY_RIGHT:
			if((drawFlag = CheckToMove(field,nextBlock[0],blockRotate,blockY,blockX+1)))
				blockX++;
			break;
		case KEY_LEFT:
			if((drawFlag = CheckToMove(field,nextBlock[0],blockRotate,blockY,blockX-1)))
				blockX--;
			break;

		default:
			break;
	}
	if(drawFlag) DrawChange(field,command,nextBlock[0],blockRotate,blockY,blockX);
	return ret;
}

void DrawField(){
	int i,j;
	for(j=0;j<HEIGHT;j++){
		move(j+1,1);
		for(i=0;i<WIDTH;i++){
			if(field[j][i]==1){
				attron(A_REVERSE);
				printw(" ");
				attroff(A_REVERSE);
			}
			else printw(".");
		}
	}
}


void PrintScore(int score){
	move(5+(BLOCK_NUM-1)*6,WIDTH+11);
	printw("%8d",score);
}

//블록의 초기 위치를 변경하여 2번째 next 블록을 그리는 과정을 추가한다.
void DrawNextBlock(int *nextBlock){
	int i, j, k;
	for( k = 1; k < BLOCK_NUM; k++) {
		for( i = 0; i < 4; i++ ){
			move(4+i+(k-1)*6,WIDTH+13);
			for( j = 0; j < 4; j++ ){
				if( block[nextBlock[k]][0][i][j] == 1 ){
					attron(A_REVERSE);
					printw(" ");
					attroff(A_REVERSE);
				}
				else printw(" ");
			}
		}
	}
}
//일정 시간마다 떨어지는 현재 블록의 위치에서 그대로 쭉 내린 상황을 가정한다.
//블록이 필드에 놓여질 위치를 그림자로 표시한다.
void DrawBlock(int y, int x, int blockID,int blockRotate,char tile){
	int i,j;
	for(i=0;i<4;i++)
		for(j=0;j<4;j++){
			if(block[blockID][blockRotate][i][j]==1 && i+y>=0){
				move(i+y+1,j+x+1);
				attron(A_REVERSE);
				printw("%c",tile);
				attroff(A_REVERSE);
			}
		}

	move(HEIGHT,WIDTH+10);
}

void DeleteBlock(int y, int x, int blockID, int blockRotate) {
	int i,j;
	char tile = '.';
	for(i=0;i<4;i++)
		for(j=0;j<4;j++){
			if(block[blockID][blockRotate][i][j]==1 && i+y>=0){
				move(i+y+1,j+x+1);
				printw("%c",tile);
			}
		}

	move(HEIGHT,WIDTH+10);
}

//현재 블록을 수직 방향으로 쭉 내렸을 때 가장 아래인 위치를 그림자의 위치로 정한다.
void DrawShadow(int y, int x, int blockID,int blockRotate){
	char tile = '/';
	int shadowY;
	shadowY = shadowHere(y, x, blockID, blockRotate);
	DrawBlock(shadowY, x, blockID, blockRotate, '/');
}

int shadowHere(int y, int x, int blockID, int blockRotate){
    int shadowY = y;
    while(CheckToMove(field, nextBlock[0], blockRotate, shadowY+1, x))
        shadowY++;
    return shadowY;
}

//drawblock() drawshadow() 함수를 호출한다.
//기존의 drawblock() 함수의 위치에 삽입하여 움직임이 갱신될 때마다 현재 블록과 함께 그림자를 그린다.
void DrawBlockWithFeatures(int y, int x, int blockID,int blockRotate) {
	DrawShadow(y, x, blockID, blockRotate);
	DrawBlock(y, x, blockID, blockRotate, ' ');
}

void DrawBox(int y,int x, int height, int width){
	int i,j;
	move(y,x);
	addch(ACS_ULCORNER);
	for(i=0;i<width;i++)
		addch(ACS_HLINE);
	addch(ACS_URCORNER);
	for(j=0;j<height;j++){
		move(y+j+1,x);
		addch(ACS_VLINE);
		move(y+j+1,x+width+1);
		addch(ACS_VLINE);
	}
	move(y+j+1,x);
	addch(ACS_LLCORNER);
	for(i=0;i<width;i++)
		addch(ACS_HLINE);
	addch(ACS_LRCORNER);
}

void play(){
	int command;
	clear();
	act.sa_handler = BlockDown;
	sigaction(SIGALRM,&act,&oact);
	InitTetris();
	do{
		if(timed_out==0){
			alarm(1);
			timed_out=1;
		}

		command = GetCommand();
		if(ProcessCommand(command)==QUIT){
			alarm(0);
			DrawBox(HEIGHT/2-1,WIDTH/2-5,1,10);
			move(HEIGHT/2,WIDTH/2-4);
			printw("Good-bye!!");
			refresh();
			getch();

			return;
		}
	}while(!gameOver);

	alarm(0);
	getch();
	DrawBox(HEIGHT/2-1,WIDTH/2-5,1,10);
	move(HEIGHT/2,WIDTH/2-4);
	printw("GameOver!!");
	refresh();
	getch();
	newRank(score);
}

char menu(){
	printw("1. play\n");
	printw("2. rank\n");
	printw("3. recommended play\n");
	printw("4. exit\n");
	return wgetch(stdscr);
}

/////////////////////////첫주차 실습에서 구현해야 할 함수/////////////////////////

int CheckToMove(char f[HEIGHT][WIDTH],int currentBlock,
                int blockRotate, int blockY, int blockX){
	// 블록이 회전하거나 블록을 이동하기 전에 블록의 예상 회전 혹은 예상 좌표로 이동 혹은 회전이 가능한지 확인한다.
	int i, j;
	int x, y;
	for(i = 0; i < 4; i++) {		// i = dy
		for(j = 0; j < 4; j++) {	// j = dx
			if(block[currentBlock][blockRotate][i][j] == 1) {
				y = blockY+i;
				x = blockX+j;
				// if the block is to be placed out of grid
				if(!(0 <= x && x < WIDTH && 0 <= y && y < HEIGHT)) {
					return 0;
				}
				// if the grid is already occupied
				if(f[y][x] == 1) {
					return 0;
				}
			}
		}
	}

	return 1;
}

void DrawChange(char f[HEIGHT][WIDTH],int command,int currentBlock,
                int blockRotate, int blockY, int blockX){
	//1. 이전 블록 정보를 찾는다. ProcessCommand의 switch문을 참조할 것
	//2. 이전 블록 정보를 지운다. DrawBlock함수 참조할 것.
	//3. 새로운 블록 정보를 그린다. 
	// 이전 블록을 지우기 위해 drawblock함수를 참조하여 deleteblock 함수를 만들어 이용한다.

	int shadowY;
	
	switch(command) {
		case KEY_UP:
			shadowY = shadowHere(blockY, blockX, currentBlock, (blockRotate+3)%4);
			DeleteBlock(shadowY, blockX, currentBlock, (blockRotate+3)%4);
			DeleteBlock(blockY, blockX, currentBlock, (blockRotate+3)%4);
			break;
		case KEY_DOWN:
			DeleteBlock(blockY-1, blockX, currentBlock, blockRotate);
			break;
		case KEY_LEFT:
			shadowY = shadowHere(blockY, blockX+1, currentBlock, blockRotate);
			DeleteBlock(shadowY, blockX+1, currentBlock, blockRotate);
			DeleteBlock(blockY, blockX+1, currentBlock, blockRotate);
			break;
		case KEY_RIGHT:
			shadowY = shadowHere(blockY, blockX-1, currentBlock, blockRotate);
			DeleteBlock(shadowY, blockX-1, currentBlock, blockRotate);
			DeleteBlock(blockY, blockX-1, currentBlock, blockRotate);
	}
	// draw the new image
	DrawBlockWithFeatures(blockY, blockX, currentBlock, blockRotate);
}

//블록을 더 이상 아래로 못 내릴 때, 1번째 next block은 현재 블록의 id를 갖는다.
//2번째 next block은 1번째 next block의 id를 갖고 마지막 2번째 블록의 id는 새로 생성된다.
void BlockDown(int sig){
	int i;
	// checkToMove함수를 호출하여 한 칸 down 가능한지 확인한다. 가능하면 y좌표 증가(한 칸 다운)
	if(CheckToMove(field, nextBlock[0], blockRotate, blockY+1, blockX)) {
		blockY++;
		// 블록을 한 칸 다운한 위치에 다시 그린다.
		DrawChange(field, KEY_DOWN, nextBlock[0], blockRotate, blockY, blockX);
	}
	else {
		score += AddBlockToField(field, nextBlock[0], blockRotate, blockY, blockX);
		// 만약 블록의 y좌표가 -1이면 gameover 변수는 1이다.
		if(blockY == -1) {
			gameOver = 1;
		}
		else {
			// 완전히 채워진 라인을 지우고 점수를 업데이트 한다.
			score += DeleteLine(field);
			
			for(i = 0; i < BLOCK_NUM-1; i++) {
				nextBlock[i] = nextBlock[i+1];
			}
			nextBlock[BLOCK_NUM-1] = rand()%7;
			DrawNextBlock(nextBlock);

			// InitTetris()에서의 값으로 초기화한다.
			blockY = -1;
			blockX = WIDTH/2-2;
			blockRotate = 0;
			PrintScore(score);
		}
		//field와 current block을 화면에 갱신하여 출력한다.
		DrawField();
	}
	//time reset
	timed_out = 0;
}

//2주차 수정한 부분
//현재 블록과 필드가 맞닿아 있는 면적을 카운트 하는 변수로 touched를 선언한다.
int AddBlockToField(char f[HEIGHT][WIDTH], int currentBlock, int blockRotate, int blockY, int blockX){
	//Block이 추가된 영역의 필드값을 바꾼다.	
	int i, j;
	int touched; 
	touched = 0;
	for(i = 0; i < 4; i++) {
		for(j = 0; j < 4; j++) {
			if(block[currentBlock][blockRotate][i][j] == 1) {
				if(0 <= blockY+i && blockY+i < HEIGHT && 0 <= blockX+j && blockX+j < WIDTH) {
					f[blockY+i][blockX+j] = 1;
					if(blockY+i == HEIGHT-1)
						touched++;
				}
			}
		}
	}
	return touched * 10;
}

int DeleteLine(char f[HEIGHT][WIDTH]){
	//1. 필드를 탐색하여, 꽉 찬 구간이 있는지 탐색한다.
	//2. 꽉 찬 구간이 있으면 해당 구간을 지운다. 즉, 해당 구간으로 필드값을 한칸씩 내린다.

	//count는 지워진 라인의 개수이고 flag는 한 줄이 채워졌는지 검사하는 변수다.
	int count;
	int flag;		
	int i, j, k;

	count = 0;
	for(i = 0; i < HEIGHT; i++) { 
		flag = 1;				  
		for(j = 0; j < WIDTH; j++) {
			if(f[i][j] == 0) {
				flag = 0;
				break;
			}
		}
		
		if(flag) {
			count++;
			// 지워진 line 바로 위에서부터 필드에 쌓인 블록의 정보를 한 줄씩 내려준다.
			for(k = i-1; k >= 0; k--) {
				for(j = WIDTH; j > 0; j--) {
					f[k+1][j] = f[k][j];
				}
			}
		}
	}

	return count*count*100;
}

void createRankList(){
	// user code
}

void rank(){
	// user code
}

void writeRankFile(){
	// user code
}

void newRank(int score){
	// user code
}

void DrawRecommend(int y, int x, int blockID,int blockRotate){
	// user code
}

int recommend(RecNode *root){
	int max=0; // 미리 보이는 블럭의 추천 배치까지 고려했을 때 얻을 수 있는 최대 점수

	// user code

	return max;
}

void recommendedPlay(){
	// user code
}

