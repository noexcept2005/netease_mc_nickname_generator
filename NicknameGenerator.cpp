/*************************************************
* Nickname Generator (Netease Minecraft Style)
* @author: Wormwaker
* @version: v1.0
**************************************************
*/
#include <string>
#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <vector>
#include <fstream>
#include <sstream>
#include <Windows.h>
#include <conio.h>
using namespace std;
#define CJZAPI __stdcall

template <typename _T1, typename _T2>
_T2 CJZAPI Convert(const _T1& value)
{
	stringstream ss;
	ss << value;
	_T2 res;
	ss >> res;
	return res;
}
template <typename _T>
string CJZAPI str(const _T& value)
{
	stringstream ss;
	ss << value;
	string res;
	ss >> res;
	return res;
}

int CJZAPI RandomRange(int Min=0, int Max=RAND_MAX, bool rchMin=true, bool rchMax=true)
{ //随机数值区间 
	int a;
	a = rand() * rand();
	if(rchMin && rchMax)	//[a,b]
		return (a%(Max - Min + 1)) + Min;
	else if(rchMin && !rchMax)		//[a,b)
		return (a%(Max - Min)) + Min;
	else if(!rchMin && rchMax)		//(a,b]
		return (a%(Max - Min)) + Min + 1;
	else							//(a,b)
		return (a%(Max - Min - 1)) + Min + 1;
}
template<typename _T>
inline _T CJZAPI Choice(initializer_list<_T> choices) {
	vector<_T> vec(choices);
	return vec[RandomRange(0, vec.size())];
}
template<typename _T>
inline _T CJZAPI Choice(const vector<_T>& choices_vector) {
	return choices_vector[RandomRange(0, choices_vector.size())];
}
template<typename _T>
inline _T& CJZAPI ChoiceRef(vector<_T>& choices_vector) {
	return choices_vector[RandomRange(0, choices_vector.size())];
}
VOID CJZAPI SetColor(UINT uFore,UINT uBack) 
{
	HANDLE handle=GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(handle,uFore+uBack*0x10);
}   //变色

#define LINE_MAXLEN 200
vector<string> CJZAPI ReadFileLines(const string& filePath)
{	//读取文件每一行 
	vector<string> ret {} ;
	fstream fin(filePath,ios::in);
	if(fin.fail())
		return ret;
	while(1)
	{
		CHAR s[LINE_MAXLEN];
		
		if(fin.eof())
			break;
		fin.getline(s, LINE_MAXLEN);
		ret.push_back(str(s));
	}
	fin.close();
	return ret;
}
vector<string> CJZAPI SplitToLines(const string& szParagraph, char lineEnding='\n')
{	//把一段话分成每行 
	vector<string> ret {} ;
	int p1=0,p2;
	for(int i=0;i<szParagraph.size();i++)
	{
		if(szParagraph.at(i) == lineEnding
			|| i==szParagraph.size()-1)	//别漏了最后一个数据 
		{
			p2=i;
			string szNew = szParagraph.substr(p1,p2-p1+(i==szParagraph.size()-1?1:0));
			ret.push_back(szNew);
			p1=i+1;
		}
	}
	return ret;
}

#define W_ATTR_PERSON_PATH "Attribute_Person.txt"
#define W_ATTR_THING_PATH  "Attribute_Thing.txt"
#define W_NOUN_PERSON_PATH "Noun_Person.txt"
#define W_NOUN_THING_PATH  "Noun_Thing.txt"
#define W_PRED_PERSON_PATH "Predicate_Person.txt"
#define W_PRED_THING_PATH  "Predicate_Thing.txt"
#define W_VT_PATH	"VerbTransitive.txt"

#define PATTERN_A_PERSON_PRED 0
#define PATTERN_A_PERSON_V_THING 1
#define PATTERN_A_PERSON 2
#define PATTERN_A_THING 3
#define PATTERN_A_THING_PRED 4

#define PATTERN_FIRST 0
#define PATTERN_LAST 4

class ComponentManager 
{
public:
	vector<string> words_attr_person;	//形容人的定语
	vector<string> words_attr_thing;	//形容物的定语
	vector<string> words_noun_person;	//形容人的名词，可作主语(主要)或宾语
	vector<string> words_noun_thing;	//形容物的名词，可作主语(主要)或宾语
	vector<string> words_pred_person;	//形容人的尽量包括宾语的谓语（动宾短语或不及物动词）
	vector<string> words_pred_thing;	//形容物的尽量包括宾语的谓语（动宾短语或不及物动词）
	vector<string> words_vt;			//及物动词
	
	void ReadComponents()
	{
		SetColor(15, 0);
		cout << "\n读取组件...";
		words_attr_person = ReadFileLines(W_ATTR_PERSON_PATH);
		words_attr_thing =  ReadFileLines(W_ATTR_THING_PATH);
		words_noun_person = ReadFileLines(W_NOUN_PERSON_PATH);
		words_noun_thing =  ReadFileLines(W_NOUN_THING_PATH);
		words_pred_person = ReadFileLines(W_PRED_PERSON_PATH);
		words_pred_thing =  ReadFileLines(W_PRED_THING_PATH);
		words_vt   =	    ReadFileLines(W_VT_PATH);
		
		LogStats();
	}
	void LogStats()
	{
		SetColor(3, 0);
		cout << "\n===== 组件统计 =====";
		
		SetColor(2, 0);
		cout << "\n定语： 人";
		SetColor(14, 0);
		cout << words_attr_person.size();
		SetColor(2, 0);
		cout << " 个，物 ";
		SetColor(14, 0);
		cout << words_attr_thing.size();
		SetColor(2, 0);
		cout << " 个";
		
		SetColor(2, 0);
		cout << "\n名词：人 ";
		SetColor(14, 0);
		cout << words_noun_person.size();
		SetColor(2, 0);
		cout << " 个，物 ";
		SetColor(14, 0);
		cout << words_noun_thing.size();
		SetColor(2, 0);
		cout << " 个";
		
		SetColor(2, 0);
		cout << "\n谓语：人 ";
		SetColor(14, 0);
		cout << words_pred_person.size();
		SetColor(2, 0);
		cout << " 个，物 ";
		SetColor(14, 0);
		cout << words_pred_thing.size();
		SetColor(2, 0);
		cout << " 个";
		
		SetColor(2, 0);
		cout << "\n及物动词：";
		SetColor(14, 0);
		cout << words_vt.size();
		SetColor(2, 0);
		cout << " 个";
		
		SetColor(3, 0);
		cout << "\n====================";
	}
	string MakeOneNickname_Raw() const
	{
		UINT uPattern = RandomRange(PATTERN_FIRST, PATTERN_LAST);
		switch(uPattern)
		{
			case PATTERN_A_PERSON:{
				return Choice(words_attr_person)+Choice(words_noun_person);
			}
			case PATTERN_A_PERSON_PRED:{
				return Choice(words_attr_person)+Choice(words_noun_person)+Choice(words_pred_person);
			}
			case PATTERN_A_PERSON_V_THING:{
				return Choice(words_attr_person)+Choice(words_noun_person)+Choice(words_vt)+Choice(words_noun_thing);
			}
			case PATTERN_A_THING:{
				return Choice(words_attr_thing)+Choice(words_noun_thing);
			}
			case PATTERN_A_THING_PRED:{
				return Choice(words_attr_thing)+Choice(words_noun_thing)+Choice(words_pred_thing);
			}
		}
		return "Invalid Nickname";
	}
	string MakeOneNickname() const
	{
		string s{""};
		while(s.length() <= 12)
			s = MakeOneNickname_Raw();
		return s;
	}
}cmgr;
////////////////////////////////////////////////////////
int main()
{
	system("title 网易MC昵称生成器 - by Wormwaker");
	srand(time(NULL));
	cmgr.ReadComponents();
	SetColor(3, 0);
	cout << "\n\n";
	cout << "输入需要的昵称数量：";
	
	SetColor(2, 0);
	size_t cnt;
	cin >> cnt;
	
	for(size_t i = 0; i < cnt; ++i)
	{
		try{
			string s = cmgr.MakeOneNickname();
			
			UINT color = 7;
			if(s.length() > 20)
				color = 4;
			else if(s.length() > 16)
				color = 12;
			else if(s.length() > 12)
				color = 6;
			else if(s.length() > 8)
				color = 14;
			else if(s.length() > 4)
				color = 2;
			else 
				color = 10;
				
			SetColor(color, 0);
			cout << s << "\t\t\t\t\t\t" << i << endl;
//			if(s == "狂笑的蛇将写散文" || s == "张哥之足球梦想"
//				|| s == "史诗吕小妹灌篮")
//				break;
		}catch(exception& e)
		{	}
	}
	_getch();
	return 0;
}
