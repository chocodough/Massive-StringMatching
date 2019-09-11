#include<stdio.h>
#include<string>
#include<vector>
#include<iostream>
#include<fstream>
#include<Windows.h>

using namespace std;

struct seq {      //문자열을 위한 구조체 선언
	string name;
	string s;
};

vector<seq> strings;  //문자열들을 저장할 벡터

char buffer_string[2000];

COORD bufferSize = { 5000, 10000 }; // 콘솔창 사이즈 지정

int Max(int a, int b);  //둘중 큰 수를 반환

void main()
{
	string seq1 = "";  // 첫번째 문자열
	string seq2 = "";  // 두번째 문자열
	string s1 = "";   // 늘어난 첫번째 문자열
	string s2 = "";   // 늘어난 두번째 문자열 
	string s3 = "";   // 매칭되는 부분을 표시하기 위한 문자열

	SetConsoleScreenBufferSize(GetStdHandle(STD_OUTPUT_HANDLE), bufferSize);  //콘솔버퍼를 조정

	string temp_data;    // 이름과 내용을 임시로 저장할 string 
	seq temp_seq;        // 읽어온 문자열을 받을 seq
	int name_end;   //이름이 끝나는 지점

	ifstream inFile("prac1.txt");  //파일 열기
	while (!inFile.eof()) {
		inFile.getline(buffer_string, 2000, '>');   // 텍스트를 >단위로 끊어읽고
		temp_data = buffer_string;
		name_end = temp_data.find(" ");             //첫번째 " "에서 문자열을 잘라냄
		temp_seq.name = temp_data.substr(0, name_end);
		temp_seq.s = temp_data.substr(name_end + 1, temp_data.length() - 1);
		
		strings.push_back(temp_seq);
		temp_seq = {};

	}
	inFile.close();
	strings.erase(strings.begin());

	for (int i = 0; i < strings.size(); i++)
		cout << strings.at(i).name << endl << strings.at(i).s << endl;    //읽은 문자열을 출력

	for (int n = 0; n < strings.size(); n++)
		for (int m = 0; m < strings.size(); m++)
		{
			if (m == n)
				continue;
			int score = 0;  //최대 부분 문자열의 점수
			seq1 = strings.at(n).s + ".";
			seq2 = strings.at(m).s + ".";

			int row = seq1.size() + 1; // 행
			int col = seq2.size() + 1; // 열

			int **tempMatrix = NULL;
			tempMatrix = new int*[col];
			for (int i = 0; i < col; i++)
				tempMatrix[i] = new int[row];  // 2차원 배열 동적 할당


			for (int i = 0; i < col; i++)
				for (int j = 0; j < row; j++)
					tempMatrix[i][j] = 0;      // 모든 값을 0 으로 초기화


			 //***********2차원 배열을 채운다**************
			for (int i = 1; i < col; i++)
				for (int j = 1; j < row; j++)
				{
					if (seq2.at(i - 1) == seq1.at(j - 1))
						tempMatrix[i][j] = tempMatrix[i - 1][j - 1] + 1;    // 같으면 대각선 위에서 1을 더함
					else
						tempMatrix[i][j] = Max(tempMatrix[i - 1][j], tempMatrix[i][j - 1]); // 다르면 왼쪽과 위중 큰값
				}

			//************2차원 배열을 추적한다(Global alignment)**************

			int k = row - 1;
			int l = col - 1;                     //대각선 맨 아래에서 시작
			while (l != 0 && k != 0)
			{
				if (tempMatrix[l][k] > tempMatrix[l - 1][k] &&
					tempMatrix[l][k] > tempMatrix[l][k - 1] &&
					tempMatrix[l][k] > tempMatrix[l - 1][k - 1])
				{
					s1 = seq2.at(l - 1) + s1;
					s2 = seq2.at(l - 1) + s2;
					s3 = "|" + s3;
					k--;
					l--;     
				}                        // 대각선 위, 왼쪽, 위쪽 보다 크면 대각선 위로 이동
				else if (tempMatrix[l][k] == tempMatrix[l][k - 1] &&
					tempMatrix[l][k] > tempMatrix[l - 1][k])
				{
					s2 = "-" + s2;
					s1 = seq1.at(k - 1) + s1;
					s3 = " " + s3;
					k--;
				}                        // 왼쪽과 같고 위보다 크면 왼쪽으로
				else
				{
					s1 = "-" + s1;
					s2 = seq2.at(l - 1) + s2;
					s3 = " " + s3;
					l--;
				}                         // 그외의 경우 위쪽으로
			}

			//**********2차원 다시 배열을 채운다(Logal alignment)**************
			for (int i = 1; i < col; i++)
				for (int j = 1; j < row; j++)
				{
					if (seq2.at(i - 1) == seq1.at(j - 1))
					{
						tempMatrix[i][j] = tempMatrix[i - 1][j - 1] + 2;    // 같으면 2 더함
					}
					else
					{
						tempMatrix[i][j] = Max(tempMatrix[i - 1][j], tempMatrix[i][j - 1]) - 6;  //다르면 - 6
						tempMatrix[i][j] = Max(tempMatrix[i][j], 0);//0 보다 작으면 0으로
					}
				}
			for (int i = 0; i < col; i++)
				for (int j = 0; j < row; j++)
					score = Max(score, tempMatrix[i][j]);  //최대 부분문자열의 점수를 저장한다.
		
			for (int i = 0; i < col; i++)
				delete[] tempMatrix[i];
			delete[] tempMatrix;            //동적할당된 배열을 풀어줌

			if (score >= 10)
			{
				cout << score << endl;

				cout << "seq1 (name, Number) : " << "(" << strings.at(n).name << "," << n << ")" << endl
					<< "seq2 (name, Number) : " << "(" << strings.at(m).name << "," << m << ")" << endl
					<< "score : " << score<< endl
					<< s1.substr(0, s1.length() - 1) << endl
					<< s3.substr(0, s3.length() - 2) << endl
					<< s2.substr(0, s2.length() - 1) << endl;
			}
			seq1 = "";
			seq2 = "";
			s1 = "";
			s2 = "";
			s3 = "";
		}
	return;
}

int Max(int a, int b)  // 둘중에 큰 수를 반환
{
	if (a >= b)
		return a;
	else
		return b;
}

