#include <iostream>
#include <cstdio>
#include <cstring>
using namespace std;

int main() {
    char input_time[20];
    char ampm[3];
    int h, m, s;
    while (cin >> input_time) { // 注意 while 处理多个 case
        std::sscanf(input_time, "%d:%d:%d%2s", &h, &m, &s, ampm);
        if (std::strcmp(ampm, "PM") == 0) {
            h += 12;
        }
        if (h == 24) {
            h = 0;
            m = 0;
            s = 0;
        }
        std::printf("%02d:%02d:%02d", h, m, s);
    }
}