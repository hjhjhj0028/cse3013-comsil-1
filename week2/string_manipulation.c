#include "Header.h"

// 문자열 뒤 연속된 공백 제거
void Remove_Blanks_At_The_End( char *line ) {
	int i, k, newline_flag = 0;

	// 전체 문장에 대하여 line[k] 가 개행문자라면 flag는 1로, '\0'이면break, 루프 탈출
	for ( k = 0; ; k++ ) {
		if ( line[k] == '\n' ) {
			newline_flag = 1;
			break;
		}
		else if (line[k] == '\0' ) {
			break;
		}
	}

	// 전체 문장에 대하여 line[i]가 공백문자가 아니라면 break, 루프 탈출
	for ( i = k-1; i >= 0; i-- ) {
		if (line[i] != ' ' ) {
			break;
		}
	}

	// flag가 1일때 문장의 마지막 개행문자, null , flag가 1이 아니라면 '\0'
	if ( newline_flag == 1 ) {
		line[i+1] = '\n';
		line[i+2] = '\0';
	}
	else {
		line[i+1] = '\0';
	}
}

// 문자열의 공백 & 문자를 카운트
void Get_Blanks_Chars( char *line, int Start, int *N_Blanks, int *N_Chars ) {
	int i, blank_flag = 0;
	*N_Blanks = *N_Chars = 0;

	// line[i]가 개행문자 또는 null일 경우 루프 탈출
	for ( i = Start; ; i++ ) {
		if ( (line[i] == '\n') || (line[i] == '\0') ) {
			break;
		}
		//line[i]가 공백문자일 경우
		else if ( line[i] == ' ' ) {
			//blank_flag가 0일 때 
			if ( blank_flag == 0 ) {
				++(*N_Blanks);
			}
			//blank_flag가 0이 아닐 때 루프 탈출
			else {
				break;
			}
		}
		else {
			blank_flag = 1;
			++(*N_Chars);
		}
	}
}
