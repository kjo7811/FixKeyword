#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <algorithm>

using namespace std;

class FixKeyword
{
public:
	struct PointOfKeyword
	{
		string keyword;
		double point;

		// 가중치가 높은 것부터 정렬하기 위해 부등호 반대로
		bool operator<(const PointOfKeyword& other) const {
			return point > other.point;
		}
	};

	void input(string fileName);
	string AddKeyword(const string& keyword, const string& day);
	// for UT
	PointOfKeyword GetDayNamePointOfKeyword(int dayName, int index) { return dayNamePointOfKeywordList[dayName][index]; }
	PointOfKeyword GetDayTypePointOfKeyword(int dayType, int index) { return dayTypePointOfKeywordList[dayType][index]; }

private:
	static const int INPUT_NUM = 500;
	static const int MAX_UZ = 2100000000;
	static const int MAX_POINT_OF_KEYWORD_SIZE = 10;

	static const int DAY_OF_WEEK = 7;
	static const int DAY_TYPE = 2;

	// MONDAY = 0
	static const int FRYDAY = 4;
	static const int WEEKDAY = 0;
	static const int HOLIDAY = 1;

	int GetDayName(const string& day);

	bool CheckPerfectHit(vector<PointOfKeyword>& KeywordList, const string& keyword, double& maxWeekBest);
	string CheckChalddukHit(const vector<PointOfKeyword>& KeywordList, const string& keyword);

	void RebasePoint();
	void PushKeyword(vector<PointOfKeyword>& KeywordList, const string& keyword, const double& point);

	bool GetSimiler(const std::string& a, const std::string& b);
	int CalulateDist(const std::string& a, const std::string& b);
	int Levenshtein(const std::string& a, const std::string& b);

	vector<PointOfKeyword> dayNamePointOfKeywordList[DAY_OF_WEEK];
	vector<PointOfKeyword> dayTypePointOfKeywordList[DAY_TYPE];
	int UZ = MAX_POINT_OF_KEYWORD_SIZE - 1;
};