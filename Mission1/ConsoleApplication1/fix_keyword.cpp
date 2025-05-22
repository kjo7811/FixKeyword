#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <algorithm>

using namespace std;

const int INPUT_NUM = 500;

const int MAX_UZ = 2100000000;
const int MAX_POINT_OF_KEYWORD_SIZE = 10;

const int DAY_OF_WEEK = 7;
const int DAY_TYPE = 2;

// MONDAY = 0
const int FRYDAY = 4;
const int WEEKDAY = 0;
const int HOLIDAY = 1;

struct PointOfKeyword
{
	string keyword;
	double point;

	bool operator<(const PointOfKeyword& other) const {
		return point < other.point;
	}
};

// 월 ~ 일
vector<PointOfKeyword> dayNamePointOfKeywordList[DAY_OF_WEEK];
// 평일, 주말
vector<PointOfKeyword> dayTypePointOfKeywordList[DAY_TYPE];
int UZ = MAX_POINT_OF_KEYWORD_SIZE - 1;

// 레벤슈타인 거리 계산 알고리즘 (문자열 유사도 검사)
int Levenshtein(const std::string& a, const std::string& b) {
	const size_t len_a = a.size();
	const size_t len_b = b.size();

	std::vector<std::vector<int>> d(len_a + 1, std::vector<int>(len_b + 1));

	for (size_t i = 0; i <= len_a; ++i) d[i][0] = i;
	for (size_t j = 0; j <= len_b; ++j) d[0][j] = j;

	for (size_t i = 1; i <= len_a; ++i) {
		for (size_t j = 1; j <= len_b; ++j) {
			if (a[i - 1] == b[j - 1])
				d[i][j] = d[i - 1][j - 1];
			else
				d[i][j] = 1 + std::min({ d[i - 1][j], d[i][j - 1], d[i - 1][j - 1] });
		}
	}
	return d[len_a][len_b];
}

int CalulateDist(const std::string& a, const std::string& b) {
	// 레벤슈타인에서 다른 알고리즘으로 변경되는 경우를 대비하여 함수를 한 번 감싸줌
	return Levenshtein(a, b);
}

// 점수 환산
bool GetSimiler(const std::string& a, const std::string& b) {
	if (a.empty() || b.empty()) return false;

	int max_len = std::max(a.length(), b.length());
	// 유사도 비율 (1.0: 완전히 같음, 0.0: 전혀 다름)
	double similarity = 1.0 - (double)CalulateDist(a, b) / (double)std::max(a.length(), b.length());
	int score = 1 + static_cast<int>(similarity * 99);

	if (score >= 80) return true;
	return false;
}

string CheckChalddukHit(const vector<PointOfKeyword>& KeywordList, const string& keyword)
{
	for (PointOfKeyword node : KeywordList) {
		if (GetSimiler(node.keyword, keyword)) {
			return node.keyword;
		}
	}
	return "";
}

void PushKeyword(vector<PointOfKeyword>& KeywordList, const string& keyword, const double& point)
{
	// 일단 넣은 후, 가장 Point가 작은 Keyword 제거.
	KeywordList.push_back({ keyword, point });
	std::sort(KeywordList.begin(), KeywordList.end());

	// 한 번에 한 개의 Keyword만 들어오기 때문에 size가 11을 초과하지 않지만, 만약을 위해 while문과 > 를 사용.
	while (KeywordList.size() > MAX_POINT_OF_KEYWORD_SIZE) {
		KeywordList.pop_back();
	}
}

bool CheckPerfectHit(vector<PointOfKeyword>& KeywordList, const string& keyword, double& maxWeekBest)
{
	// 소수점 계산을 위해 point 관련 변수를 double형으로 변경
	for (PointOfKeyword& node : KeywordList) {
		if (node.keyword == keyword) {
			node.point += (node.point * 0.1);
			maxWeekBest = node.point;
			return true;
		}
	}
	return false;
}

int GetDayName(const string& day)
{
	const string strDay[DAY_OF_WEEK] = { "monday","tuesday" ,"wednesday" ,"thursday" ,"friday","saturday" ,"sunday" };

	for (int i = 0; i < DAY_OF_WEEK; i++)
		if (strDay[i] == day)
			return i;

	return -1;
}

void RebasePoint(double maxWeekBest, double maxTwoBest)
{
	if (UZ >= MAX_UZ || maxWeekBest >= MAX_UZ || maxTwoBest >= MAX_UZ) {
		UZ = MAX_POINT_OF_KEYWORD_SIZE - 1;
		for (int i = 0; i < DAY_OF_WEEK; i++) {
			// 가중치가 큰 순서대로 정렬되어 있으므로 num-- 하며 점수 부여.
			int num = dayNamePointOfKeywordList[i].size();
			for (PointOfKeyword& node : dayNamePointOfKeywordList[i]) {
				node.point = num;
				num--;
			}
		}
		for (int i = 0; i < DAY_TYPE; i++) {
			int num = dayTypePointOfKeywordList[i].size();
			for (PointOfKeyword& node : dayTypePointOfKeywordList[i]) {
				node.point = num;
				num--;
			}
		}
	}
}

string FixKeyword(const string& keyword, const string& day) {
	if (keyword.empty() || day.empty()) return "";

	// 요일
	int dayName = GetDayName(day);
	// day가 잘못 들어온 경우 예외처리
	if (dayName == -1) return "";
	// 평일 or 주말
	int dayType = dayName <= FRYDAY ? WEEKDAY : HOLIDAY;

	// 명세서에는 없지만, keyword와 day가 제대로 들어온 경우에만 UZ++
	UZ++;

	double maxDayNamePoint = 0;
	double maxDayTypePoint = 0;

	// 완벽 HIT
	bool flag = false;
	flag = CheckPerfectHit(dayNamePointOfKeywordList[dayName], keyword, maxDayNamePoint);
	flag = CheckPerfectHit(dayTypePointOfKeywordList[dayType], keyword, maxDayTypePoint);

	// 점수가 21억을 초과하는 경우 재배치
	RebasePoint(maxDayNamePoint, maxDayTypePoint);

	if (flag) return keyword;

	// 찰떡 HIT
	string hitKeyword = "";
	hitKeyword = CheckChalddukHit(dayNamePointOfKeywordList[dayName], keyword);
	if (hitKeyword != "") return hitKeyword;

	hitKeyword = CheckChalddukHit(dayTypePointOfKeywordList[dayType], keyword);
	if (hitKeyword != "") return hitKeyword;

	// 완벽 HIT, 찰떡 HIT 둘다 아닌경우.
	// 명세서에 명확히 표기되어 있지 않아 강사님 답변 받음: 재정렬이 먼저 일어나고, 그 후에 Keyword 추가되므로 10 Point Keyword 추가.
	UZ++;
	PushKeyword(dayNamePointOfKeywordList[dayName], keyword, UZ);
	PushKeyword(dayTypePointOfKeywordList[dayType], keyword, UZ);

	return keyword;
}

void input() {
	// 500개 데이터 입력
	ifstream fin{ "keyword_weekday_500.txt" };

	// 파일 입출력 에러처리
	if (fin.is_open()) {
		for (int i = 0; i < INPUT_NUM; i++) {
			string keyword, day;
			fin >> keyword >> day;
			string ret = FixKeyword(keyword, day);
			std::cout << ret << "\n";
		}
	}
	else {
		std::cerr << "파일 열기 실패" << std::endl;
		return;
	}
	// 파일 닫기
	fin.close();
}

int main()
{
	input();
}