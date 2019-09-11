#include<stdio.h>
#include<string>
#include<vector>
#include<iostream>
#include<fstream>
#include<Windows.h>

using namespace std;

struct seq {      //���ڿ��� ���� ����ü ����
	string name;
	string s;
};

vector<seq> strings;  //���ڿ����� ������ ����

char buffer_string[2000];

COORD bufferSize = { 5000, 10000 }; // �ܼ�â ������ ����

int Max(int a, int b);  //���� ū ���� ��ȯ

void main()
{
	string seq1 = "";  // ù��° ���ڿ�
	string seq2 = "";  // �ι�° ���ڿ�
	string s1 = "";   // �þ ù��° ���ڿ�
	string s2 = "";   // �þ �ι�° ���ڿ� 
	string s3 = "";   // ��Ī�Ǵ� �κ��� ǥ���ϱ� ���� ���ڿ�

	SetConsoleScreenBufferSize(GetStdHandle(STD_OUTPUT_HANDLE), bufferSize);  //�ֹܼ��۸� ����

	string temp_data;    // �̸��� ������ �ӽ÷� ������ string 
	seq temp_seq;        // �о�� ���ڿ��� ���� seq
	int name_end;   //�̸��� ������ ����

	ifstream inFile("prac1.txt");  //���� ����
	while (!inFile.eof()) {
		inFile.getline(buffer_string, 2000, '>');   // �ؽ�Ʈ�� >������ �����а�
		temp_data = buffer_string;
		name_end = temp_data.find(" ");             //ù��° " "���� ���ڿ��� �߶�
		temp_seq.name = temp_data.substr(0, name_end);
		temp_seq.s = temp_data.substr(name_end + 1, temp_data.length() - 1);
		
		strings.push_back(temp_seq);
		temp_seq = {};

	}
	inFile.close();
	strings.erase(strings.begin());

	for (int i = 0; i < strings.size(); i++)
		cout << strings.at(i).name << endl << strings.at(i).s << endl;    //���� ���ڿ��� ���

	for (int n = 0; n < strings.size(); n++)
		for (int m = 0; m < strings.size(); m++)
		{
			if (m == n)
				continue;
			int score = 0;  //�ִ� �κ� ���ڿ��� ����
			seq1 = strings.at(n).s + ".";
			seq2 = strings.at(m).s + ".";

			int row = seq1.size() + 1; // ��
			int col = seq2.size() + 1; // ��

			int **tempMatrix = NULL;
			tempMatrix = new int*[col];
			for (int i = 0; i < col; i++)
				tempMatrix[i] = new int[row];  // 2���� �迭 ���� �Ҵ�


			for (int i = 0; i < col; i++)
				for (int j = 0; j < row; j++)
					tempMatrix[i][j] = 0;      // ��� ���� 0 ���� �ʱ�ȭ


			 //***********2���� �迭�� ä���**************
			for (int i = 1; i < col; i++)
				for (int j = 1; j < row; j++)
				{
					if (seq2.at(i - 1) == seq1.at(j - 1))
						tempMatrix[i][j] = tempMatrix[i - 1][j - 1] + 1;    // ������ �밢�� ������ 1�� ����
					else
						tempMatrix[i][j] = Max(tempMatrix[i - 1][j], tempMatrix[i][j - 1]); // �ٸ��� ���ʰ� ���� ū��
				}

			//************2���� �迭�� �����Ѵ�(Global alignment)**************

			int k = row - 1;
			int l = col - 1;                     //�밢�� �� �Ʒ����� ����
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
				}                        // �밢�� ��, ����, ���� ���� ũ�� �밢�� ���� �̵�
				else if (tempMatrix[l][k] == tempMatrix[l][k - 1] &&
					tempMatrix[l][k] > tempMatrix[l - 1][k])
				{
					s2 = "-" + s2;
					s1 = seq1.at(k - 1) + s1;
					s3 = " " + s3;
					k--;
				}                        // ���ʰ� ���� ������ ũ�� ��������
				else
				{
					s1 = "-" + s1;
					s2 = seq2.at(l - 1) + s2;
					s3 = " " + s3;
					l--;
				}                         // �׿��� ��� ��������
			}

			//**********2���� �ٽ� �迭�� ä���(Logal alignment)**************
			for (int i = 1; i < col; i++)
				for (int j = 1; j < row; j++)
				{
					if (seq2.at(i - 1) == seq1.at(j - 1))
					{
						tempMatrix[i][j] = tempMatrix[i - 1][j - 1] + 2;    // ������ 2 ����
					}
					else
					{
						tempMatrix[i][j] = Max(tempMatrix[i - 1][j], tempMatrix[i][j - 1]) - 6;  //�ٸ��� - 6
						tempMatrix[i][j] = Max(tempMatrix[i][j], 0);//0 ���� ������ 0����
					}
				}
			for (int i = 0; i < col; i++)
				for (int j = 0; j < row; j++)
					score = Max(score, tempMatrix[i][j]);  //�ִ� �κй��ڿ��� ������ �����Ѵ�.
		
			for (int i = 0; i < col; i++)
				delete[] tempMatrix[i];
			delete[] tempMatrix;            //�����Ҵ�� �迭�� Ǯ����

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

int Max(int a, int b)  // ���߿� ū ���� ��ȯ
{
	if (a >= b)
		return a;
	else
		return b;
}

