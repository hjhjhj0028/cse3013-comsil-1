#include "stringClass.h"
#include <cstring>
#include <iostream>
#include <cstdlib>

using namespace std;

Str::Str(int leng) {
	// leng이 0 이하면 에러 메시지 출력
	if (leng <= 0) {
		cerr << "invalid leng error!\n";
		exit(-1);
	}
	// leng+1 크기의 문자형 배열을 동적 할당 받아 str에 주고 공백문자 처리
	str = new char[leng + 1];
	str[0] = '\0';
	len = leng;
}

Str::Str(char* neyong) {
	// neyong == NULL이면 에러 메시지 출력
	if(str == NULL) {
		cerr << "allocatoin error!\n";
        exit(-1);
	}
	// len에 값 주고 그만큼의 문자열 배열 동적 할당 받아 str에 준다
	len = strlen(neyong);
    str = new char[len + 1];
	// neyong을 str로 복사
	strcpy(str, neyong);
    str[len] = '\0';
}
// 소멸자
Str::~Str() {
    delete []str;
}
// str 길이 리턴
int Str::length() {
    return len;
}
// str 내용 리턴
char *Str::contents() {
    return str;
}
// a의 내용과 strcmp
int Str::compare(char *a) {
    return strcmp(str, a);
}
// a의 내용과 strcmp
int Str::compare(class Str &a) {
    return strcmp(str, a.contents());
}
// str 값 대입
void Str::operator=(char *a) {
    if (a == 0) { 
        len = 0;
        delete []str;
		str = new char[1];
        str[0] = '\0';
        return;
    }
    int newlen = strlen(a);
    if (len < newlen) {
		delete []str;
		str = new char[newlen + 1];
	}
    len = newlen;
    strcpy(str, a);
    str[len] = '\0';
}
// str 내용 대입
void Str::operator=(class Str &a) {
    delete[] str;
	len = a.length();
	str = new char[len+1];
	strcpy(str, a.contents());
}
