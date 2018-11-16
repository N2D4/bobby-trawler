#include <iostream>
#include <vector>
#include "engine.cpp"
using namespace std;


const vector<string> WHITESPACES = {" ", "\t", "\n", "\r\n"};



// function declarations
string waitforcmd(string str);
string waitforcmd(vector<string> v);
vector<string> readcmd();
pair<size_t, size_t> findany(string s, vector<string> find, size_t start);
vector<string> nonEmptySplits(string str);



int main() {
    // Do the UCI stuff (it's how we talk to the UI)
    waitforcmd("uci");
    cout << "id name BobbyTrawler" << endl;
    cout << "id author some cool people" << endl;
    cout << "uciok" << endl;

    // Tell the GUI we're ready
    waitforcmd("isready");
    cout << "readyok" << endl;

    


    return 0;
}





string waitforcmd(string str) {
    return waitforcmd((vector<string>) {str});
}

string waitforcmd(vector<string> v) {
    while (true) {
        vector<string> launchcmd = readcmd();
        if (std::find(v.begin(), v.end(), launchcmd[0]) != v.end()) {
            return launchcmd[0];
        }
    }
}

vector<string> readcmd() {
    vector<string> res;
    while (res.empty()) {
        string str;
        getline(cin, str);
        res = nonEmptySplits(str);        
    }
    return res;
}

vector<string> nonEmptySplits(string str) {
    vector<string> res;
    size_t pos = 0;
    pair<size_t, size_t> npos;

    while ((npos = findany(str, WHITESPACES, pos)).first < str.length()) {
        string s = str.substr(pos, npos.first - pos);
        if (s.length() > 0) {
            res.push_back(s);
        }
        pos = npos.second;
    }

    if (pos < str.length()) {
        res.push_back(str.substr(pos));
    }

    return res;
}

pair<size_t, size_t> findany(string s, vector<string> find, size_t start) {
    size_t min = s.length();
    size_t npos = min;
    for (string f : find) {
        size_t m = s.find(f, start);
        size_t n = m + f.length();
        if (m < min || (m == min && n > npos)) {
            min = m;
            npos = n;
        }
    }
    return pair<size_t, size_t>(min, npos);
}