#include "Core.h"



// Парсит блок из 7 int-ов
tuple<int, int, int, int, int, int, int> ParseInt7Block(const string& block) {
    string s = block.substr(1, block.size() - 2); // обрезаем фигурные скобки
    stringstream ss(s);
    string val;
    vector<int> vals;

    while (getline(ss, val, '\\')) {
        val.erase(remove_if(val.begin(), val.end(),
            [](char c) { return c == '\r' || c == '\n' || isspace(c); }),
            val.end());

        if (!val.empty())
            vals.push_back(stoi(val));
    }

    if (vals.size() != 7) throw runtime_error("Expected exactly 7 integers in block.");

    return { vals[0], vals[1], vals[2], vals[3], vals[4], vals[5], vals[6] };
}

// Основной парсер
void ParseLine3(
    const string& line,
    vector<tuple<int, int, tuple<int, int, int, int, int, int, int>>>& outVec)
{
    regex rx(R"(([-]?\d+)\(([-]?\d+)\)\{([^\}]*)\})");

    string clean = line;
    clean.erase(
        remove_if(clean.begin(), clean.end(), [](char c) { return c == '\r' || c == '\n'; }),
        clean.end()
    );

    auto begin = sregex_iterator(clean.begin(), clean.end(), rx);
    auto end_ = sregex_iterator();

    for (auto it = begin; it != end_; ++it) {
        int a = stoi((*it)[1].str());
        int b = stoi((*it)[2].str());
        string rawBlock = "{" + (*it)[3].str() + "}";

        outVec.emplace_back(a, b, ParseInt7Block(rawBlock));
    }
}