#pragma once

#include <string>
#include <sstream>
#include <vector>
#include <map>
#include <list>
#include <algorithm>
#include <fstream>
#include <locale>
#include <codecvt>

using namespace std;

#ifdef _UNICODE
	typedef wchar_t tchar;
	typedef wstring tstring;
	typedef wifstream tifstream;
	typedef wstringstream tstringstream;
#else
	typedef char tchar;
	typedef string tstring;
	typedef in tin;
	typedef stringstream tstringstream;
#endif

class CTQCore {
public:
	CTQCore() { }
	~CTQCore() { }

	typedef unsigned int time_u;
	typedef vector <unsigned int> Path;

	struct Line {
		tstring name;
		tstring src;
		tstring dest;
		time_u srcTime;
		time_u destTime;
		int period;
		double price;
	};

	static time_u time_to_uint(tstring _time);
	static tstring uint_to_time(time_u _time);
	static int period_to_int(tstring _period);
	static tstring int_to_period(int _period);
	static int time_to_period(time_u _t1, time_u _t2);

private:
	class PriceGraph {
	private:
		struct adjNode {
			unsigned int city;
			unsigned int line_index;
			double price;
		};

		map <tstring, unsigned int> cityID;
		list <adjNode>* adjList;

	public:
		PriceGraph() { cityID.clear(); adjList = NULL; }
		~PriceGraph() { if (adjList) delete[] adjList; }
		void Init(map <tstring, tstring> &_cityData, vector <Line> &_lineData);
		Path Query(const tstring &_src, const tstring &_dest);

	};

	class TimeGraph {
	private:
		struct Node;

		struct Arc {
			Node* node;
			int period;
			bool operator== (const Arc &a) const {
				return node == a.node;
			}
		};

		struct Node {
			unsigned int city;
			unsigned int node_index;
			time_u time;
			Arc next;
			Arc adj;
		};

		map <tstring, unsigned int> cityID;
		Node* cityEntry;
		Node* nodeList;
		unsigned int nodeCount;

	public:
		TimeGraph() { cityID.clear(); cityEntry = nodeList = NULL; nodeCount = 0; }
		~TimeGraph() { if (cityEntry) delete[] cityEntry; if (nodeList) delete[] nodeList; }
		void Init(map <tstring, tstring> &_cityData, vector <Line> &_lineData);
		Path Query(const tstring &_src, const tstring &_dest, const time_u &_now);

	};

public:
	class Timetable {
	private:
		tstring name;
		vector <tstring> cityList;
		vector <Line> lineData;
		map <tstring, tstring> cityData;
		PriceGraph PG;
		TimeGraph TG;

	public:
		enum QueryType { QT_PRICE, QT_TIME };

		Timetable() { name = _T(""); cityList.clear(); lineData.clear(); cityData.clear(); }
		~Timetable() { }
		tstring GetName() { return name; }
		void SetName(const tstring &_name) { name = _name; }
		unsigned int GetCityCount() { return cityData.size(); }
		unsigned int GetLineCount() { return lineData.size(); }
		int LoadData(const tstring &_file);
		int Refresh();
		int AddCity(const tstring &_ID, const tstring &_name);
		tstring GetCity(const unsigned int _index);
		tstring GetCityName(const tstring &_ID);
		int DeleteCity(const tstring &_ID);
		int AddLine(const Line &_line);
		Line GetLine(const unsigned int _index);
		int DeleteLine(const unsigned int _index);
		Path Query(const tstring &_src, const tstring &_dest, time_u _now, QueryType _type);

	};
};

