#include "stdafx.h"
#include "TQCore.h"

using namespace std;


int CTQCore::Timetable::LoadData(const tstring &_file) {

	tifstream in(_file);
	in.imbue(locale(locale::classic(), new codecvt_utf8<tchar>));
	if (!in.is_open())
		return 1;
	unsigned int city_count = 0, line_count = 0;

	in >> city_count;
	for (unsigned int i = 0; i < city_count; i++) {
		tstring city_ID, city_name;
		in >> city_ID >> city_name;
		cityData[city_ID] = city_name;
		cityList.push_back(city_ID);
	}

	in >> line_count;
	for (unsigned int i = 0; i < line_count; i++) {
		Line line; tstring s_srcTime, s_destTime, s_period;
		in >> line.name >> line.src >> line.dest >> s_srcTime >> s_destTime >> s_period >> line.price;
		line.srcTime = CTQCore::time_to_uint(s_srcTime);
		line.destTime = CTQCore::time_to_uint(s_destTime);
		line.period = CTQCore::period_to_int(s_period);
		lineData.push_back(line);
	}

	in.close();
	return 0;
}

int CTQCore::Timetable::Refresh() {
	PG.Init(cityData, lineData);
	TG.Init(cityData, lineData);
	return 0;
}

int CTQCore::Timetable::AddCity(const tstring &_ID, const tstring &_name) {
	if (cityData.find(_ID) != cityData.end())
		return 1;
	cityData[_ID] = _name;
	cityList.push_back(_ID);
	return 0;
}

tstring CTQCore::Timetable::GetCity(const unsigned int _index) {
	if (_index >= cityList.size())
		return tstring();
	return cityList[_index];
}

tstring CTQCore::Timetable::GetCityName(const tstring &_ID) {
	if (cityData.find(_ID) == cityData.end())
		return tstring();
	return cityData[_ID];
}

int CTQCore::Timetable::DeleteCity(const tstring &_ID) {
	if (cityData.find(_ID) == cityData.end())
		return 1;
	cityData.erase(_ID);
	
	for (unsigned int i = 0; i < cityList.size(); i++)
		if (cityList[i] == _ID) {
			cityList.erase(cityList.begin() + i);
			break;
		}

	for (unsigned int i = 0; i < lineData.size(); i++)
		if (lineData[i].src == _ID || lineData[i].dest == _ID) {
			lineData.erase(lineData.begin() + i);
			i--;
		}

	return 0;
}

int CTQCore::Timetable::AddLine(const Line &_line) {
	if (_line.name == tstring())
		return 1;
	if (cityData.find(_line.src) == cityData.end() || cityData.find(_line.dest) == cityData.end())
		return 2;
	// Todo: check srcTime and destTime
	lineData.push_back(_line);
	return 0;
}

CTQCore::Line CTQCore::Timetable::GetLine(const unsigned int _index) {
	if (_index >= lineData.size())
		return { tstring(), tstring(), tstring(), 0, 0, 0, 0 };
	return lineData[_index];
}

int CTQCore::Timetable::DeleteLine(const unsigned int _index) {
	if (_index >= lineData.size())
		return 1;
	lineData.erase(lineData.begin() + _index);
	return 0;
}

CTQCore::Path CTQCore::Timetable::Query(const tstring &_src, const tstring &_dest, time_u _now, QueryType _type) {
	Path result;
	if (_type == QT_PRICE)
		result = PG.Query(_src, _dest);
	else if (_type == QT_TIME)
		result = TG.Query(_src, _dest, _now);
	return result;
}

void CTQCore::PriceGraph::Init(map <tstring, tstring> &_cityData, vector <Line> &_lineData) {
	map <tstring, tstring>::iterator map_iter = _cityData.begin();
	for (unsigned int i = 0; map_iter != _cityData.end(); i++, map_iter++)
		cityID[map_iter->first] = i;

	if (adjList)
		delete[] adjList;
	adjList = new list <adjNode>[cityID.size()];

	for (unsigned int i = 0; i < _lineData.size(); i++) {
		unsigned int srcID = cityID[_lineData[i].src], destID = cityID[_lineData[i].dest];
		list <adjNode>::iterator list_iter = adjList[srcID].begin();
		while (list_iter != adjList[srcID].end()) {
			if (list_iter->city >= destID)
				break;
			list_iter++;
		}
		if (list_iter != adjList[srcID].end() && list_iter->city == destID) {
			if (list_iter->price > _lineData[i].price)
				list_iter->line_index = i, list_iter->price = _lineData[i].price;
		}
		else
			adjList[srcID].insert(list_iter, { destID, i, _lineData[i].price });
	}

}

CTQCore::Path CTQCore::PriceGraph::Query(const tstring &_src, const tstring &_dest) { // Dijkstra
	const double MAX_PRICE = double(1u << 20);
	unsigned int srcID = cityID[_src], destID = cityID[_dest];
	double *D = new double[cityID.size()];
	unsigned int *P = new unsigned int[cityID.size()];
	bool* finish = new bool[cityID.size()];

	for (unsigned int v = 0; v < cityID.size(); v++)
		finish[v] = false, D[v] = MAX_PRICE;
	list <adjNode>::iterator iter;
	for (iter = adjList[srcID].begin(); iter != adjList[srcID].end(); iter++)
		D[iter->city] = iter->price, P[iter->city] = srcID;
	D[srcID] = 0, finish[srcID] = true;

	for (unsigned int i = 1; i < cityID.size(); i++) {
		unsigned int v = 0;
		double curr_min = MAX_PRICE;
		for (unsigned int w = 0; w < cityID.size(); w++)
			if (!finish[w] && D[w] < curr_min)
				v = w, curr_min = D[w];
		if (v == destID || D[v] == MAX_PRICE)
			break;
		finish[v] = true;
		iter = adjList[v].begin();
		for (unsigned int w = 0; w < cityID.size(); w++) {
			if (iter == adjList[v].end())
				break;
			if (w < iter->city)
				continue;
			if (!finish[w] && curr_min + iter->price < D[w])
				D[w] = curr_min + iter->price, P[w] = v;
			iter++;
		}
	}

	Path result;
	if (D[destID] == MAX_PRICE)
		return result;

	unsigned int v, w = destID;
	do {
		v = P[w];
		iter = adjList[v].begin();
		while (iter->city != w) iter++;
		result.insert(result.begin(), iter->line_index);
		w = v;
	} while (v != srcID);
	delete[] D, P, finish;

	return result;
}

void CTQCore::TimeGraph::Init(map <tstring, tstring> &_cityData, vector <Line> &_lineData) {
	map <tstring, tstring>::iterator map_iter = _cityData.begin();
	for (unsigned int i = 0; map_iter != _cityData.end(); i++, map_iter++)
		cityID[map_iter->first] = i;

	if (cityEntry)
		delete cityEntry;
	cityEntry = new Node[cityID.size()];

	for (unsigned int i = 0; i < cityID.size(); i++)
		cityEntry[i].city = i, cityEntry[i].node_index = unsigned(-1), cityEntry[i].time = 0, cityEntry[i].next = { cityEntry + i, 0 }, cityEntry[i].adj = { NULL, 0 };

	if (nodeList)
		delete nodeList;
	nodeList = new Node[2 * _lineData.size()];

	for (unsigned int i = 0; i < _lineData.size(); i++) {
		unsigned int srcID = cityID[_lineData[i].src], destID = cityID[_lineData[i].dest];

		{
			nodeList[2 * i].city = srcID, nodeList[2 * i].node_index = 2 * i, nodeList[2 * i].time = _lineData[i].srcTime, nodeList[2 * i].adj = { NULL, 0 };
			Node* ptr = cityEntry + srcID;
			while (ptr->next.node->time > 0 && ptr->next.node->time <= nodeList[2 * i].time)
				ptr = ptr->next.node;
			nodeList[2 * i].next = { ptr->next.node, time_to_period(nodeList[2 * i].time, ptr->next.node->time) };
			ptr->next = { nodeList + 2 * i, time_to_period(ptr->time ,nodeList[2 * i].time) };
		}

		{
			nodeList[2 * i + 1].city = destID, nodeList[2 * i + 1].node_index = 2 * i + 1, nodeList[2 * i + 1].time = _lineData[i].destTime, nodeList[2 * i + 1].adj = { NULL, 0 };
			Node* ptr = cityEntry + destID;
			while (ptr->next.node->time > 0 && ptr->next.node->time <= nodeList[2 * i + 1].time)
				ptr = ptr->next.node;
			nodeList[2 * i + 1].next = { ptr->next.node, time_to_period(nodeList[2 * i + 1].time, ptr->next.node->time) };
			ptr->next = { nodeList + 2 * i + 1, time_to_period(ptr->time, nodeList[2 * i + 1].time) };
		}

		nodeList[2 * i].adj = { nodeList + 2 * i + 1, _lineData[i].period };
	}

	nodeCount = 2 * _lineData.size();

}

CTQCore::Path CTQCore::TimeGraph::Query(const tstring &_src, const tstring &_dest, const time_u &_now) { // Dijkstra
	const unsigned int MAX_PERIOD = 1u << 20;
	Path result;
	unsigned int srcID = cityID[_src], destID = cityID[_dest];
	unsigned int *D = new unsigned int[nodeCount], *P = new unsigned int[nodeCount];
	bool* finish = new bool[nodeCount];

	for (unsigned int v = 0; v < nodeCount; v++)
		finish[v] = false, D[v] = MAX_PERIOD;

	Node* ptr = cityEntry + srcID;
	if (ptr->next.node == ptr)
		return result;
	while (ptr->time == 0) ptr = ptr->next.node;
	while (ptr->time > 0 && ptr->time < _now) ptr = ptr->next.node;
	if (ptr->node_index == unsigned(-1))
		ptr = ptr->next.node;
	unsigned int start = ptr->node_index, fin = unsigned(-1);
	{
		unsigned int real_next = ptr->next.node->node_index == unsigned(-1) ? ptr->next.node->next.node->node_index : ptr->next.node->node_index;
		unsigned int real_period = ptr->next.period;
		if (ptr->next.node->node_index == unsigned(-1))
			real_period += ptr->next.node->next.period;
		D[real_next] = real_period, P[real_next] = start;
		if (ptr->adj.node)
			D[ptr->adj.node->node_index] = ptr->adj.period, P[ptr->adj.node->node_index] = start;
		D[start] = 0, finish[start] = true;
	}

	for (unsigned int i = 1; i < nodeCount; i++) {
		unsigned int v = 0;
		unsigned int curr_min = MAX_PERIOD;
		for (unsigned int w = 0; w < nodeCount; w++)
			if (!finish[w] && D[w] < curr_min)
				v = w, curr_min = D[w];
		if (nodeList[v].city == destID) {
			fin = v;
			break;
		}

		finish[v] = true;
		unsigned int w = nodeList[v].next.node->node_index == unsigned(-1) ? nodeList[v].next.node->next.node->node_index : nodeList[v].next.node->node_index;
		unsigned int real_period = nodeList[v].next.period;
		if (nodeList[v].next.node->node_index == unsigned(-1))
			real_period += nodeList[v].next.node->next.period;

		if (!finish[w] && curr_min + real_period < D[w])
			D[w] = curr_min + real_period, P[w] = v;

		if (nodeList[v].adj.node) {
			w = nodeList[v].adj.node->node_index;
			if (!finish[w] && curr_min + nodeList[v].adj.period < D[w])
				D[w] = curr_min + nodeList[v].adj.period, P[w] = v;
		}
	}

	if (fin == unsigned(-1))
		return result;

	unsigned int v, w = fin;
	do {
		v = P[w];
		if (nodeList[v].city != nodeList[w].city) {
			result.insert(result.begin(), v / 2);
		}
		w = v;
	} while (nodeList[v].city != srcID);

	delete[] D, P, finish;

	return result;
}

CTQCore::time_u CTQCore::time_to_uint(tstring _time) {
	size_t colon = _time.find_first_of(':');
	if (colon == tstring::npos || colon + 1 >= _time.length())
		return 0;
	tstring s_hour, s_minute;
	s_hour = _time.substr(0, colon);
	s_minute = _time.substr(colon + 1, _time.length() - colon - 1);

	tstringstream ss;
	time_u t_hour = 0, t_minute = 0;
	ss.clear(); ss << s_hour; ss >> t_hour;
	ss.clear(); ss << s_minute; ss >> t_minute;
	return t_hour * 60 + t_minute;
}

tstring CTQCore::uint_to_time(time_u _time) {
	time_u t_hour, t_minute;
	t_hour = _time / 60; t_minute = _time % 60;

	tstringstream ss;
	ss.fill('0');
	ss.width(2); ss << t_hour << ":";
	ss.width(2); ss << t_minute;
	return ss.str();
}

int CTQCore::period_to_int(tstring _period) {
	if (_period[0] == '-') {
		_period.erase(0, 1);
		return (signed int)0 - time_to_uint(_period);
	}
	return (signed int)time_to_uint(_period);
}

tstring CTQCore::int_to_period(int _period) {
	tstring result = uint_to_time((unsigned int)0 - _period);
	if (_period < 0)
		result = tchar('-') + result;
		
	return uint_to_time((unsigned int)_period);
}

inline int CTQCore::time_to_period(time_u _t1, time_u _t2) {
	return (_t2 - _t1 + 1440) % 1440;
}