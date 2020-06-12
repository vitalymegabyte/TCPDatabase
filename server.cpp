#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <map>
#include <string>
#include <iostream>
#include <string.h>
#include <vector>
#include <sstream>

#include <fcntl.h> // for open
#include <unistd.h> // for close
#include<pthread.h>
using namespace std;
#define switchs(str); string compStr = str; //switchs -- версия 
//switch для строк. Изначально присваивает щначение str переменной
//compstr, чтобы с ней можно было работать в switchs. Теориетически,
//можно рассматривать s как суффикс, обозначающий "строковый".
#define cases(str) if(compStr == str) //Здесь просто сравнение


int PORT = 5000;

bool isNumber(const string s){
  return s.find_first_not_of( "0123456789-" ) == string::npos;
}

bool contains(map<int, string> &map, int key) {
    return map.find(key) != map.end();
}

bool add(map<int, string>  &map, int key, string value) {
    if(contains(map, key))
        return false;
    else
    {
        map.insert(make_pair(key, value));
        return true;
    }
}

string get(map<int, string> &map, int key) {
    if(contains(map, key)) {
        return map[key];
    } else {
        return "";
    }
}
string parseQuery(char *query, map<int, string> &map) {
    vector<char *> vec;
    char *fragment;
    fragment = strtok(query, " ");
    while(fragment != NULL) {
        vec.push_back(fragment);
        fragment = strtok(NULL, " ");
    }
    int key;
    string value;
    stringstream answer;
    bool success = false;
    if(vec.size() > 1 && isNumber(vec[1])) { //Общее условие почти для всех. Синтаксис предусматривает: [ACTION] [KEY] [VALUE]. 
                                             //При этом третье значение в некоторых типах запросов опускается. Поэтому 
                                             //обязательными условиями являются длина в 2 сектора, и валидный ключ
        key = atoi(vec[1]);
        switchs(vec[0]);
            cases("-a"){
                if(vec.size() > 2) {
                    value = vec[2];
                    success = add(map, key, value);
                    answer << "Successfully added " << key << "=>'" << value << "'";
                    success = true;
                }
            }
            cases("-g") {
                value = get(map, key);
                success = value != "";
                answer << key << "=>'" << value << "'";
                success = true;
            }
            cases("-c") {
                if(contains(map, key))
                    answer << "TRUE";
                else
                    answer << "FALSE";
                success = true;
            }
    } else {
        success = false;
    }
    cout.flush();
    if(success) {
        return answer.str();
    } else {
        return "Something went wrong. Check your query syntax.";
    }
}

struct threadParams {
    int * socket;
    map<int, string> * baseMap;
};

void * socketThread (void *arg) {
    //int sock = *((int *)arg);
    threadParams params = *((threadParams *)arg);
    int sock = *(params.socket);

    char buf[1024];
    int bytes_read;
    map<int, string> &map = *params.baseMap;
    while(1)
    {
        bytes_read = recv(sock, buf, 1024, 0);
        if(bytes_read <= 0) break;
        string a = parseQuery(buf, map);
        const char * answer = "";
        answer = a.c_str();
        send(sock, answer, (int)strlen(answer), 0);
        cout << "query passed" << endl;
        cout << answer << endl;
    }
    close(sock);
    cout << "Connection closed" << endl;    
}
int main(int argc, char *argv[])
{
    if(argc > 1) {
        for(int i = 1; i < argc; i++) {
            if(strcmp(argv[i], "-p") == 0 && i + 1 < argc && isNumber(argv[i + 1])) {
                PORT = atoi(argv[i + 1]);
                i++;
            }
        }
    }
    cout << "Listening on port " << PORT << endl;
    map<int, string> map;
    int listener;
    struct sockaddr_in addr;
    char buf[1024];
    int bytes_read;

    listener = socket(AF_INET, SOCK_STREAM, 0);
    if(listener < 0)
    {
        perror("socket");
        exit(1);
    }
    
    addr.sin_family = AF_INET;
    addr.sin_port = htons(PORT);
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    if(bind(listener, (struct sockaddr *)&addr, sizeof(addr)) < 0)
    {
        perror("bind");
        exit(2);
    }
    pthread_t thr; 
    listen(listener, 1);
    cout.flush();
    while(1)
    {
        int sock = accept(listener, NULL, NULL);
        if(sock < 0)
        {
            perror("accept");
            exit(3);
        }
        threadParams params;
        params.socket = &sock;
        params.baseMap = &map;
        if(pthread_create(&thr, NULL, socketThread, &params) != 0) {
            cout << "Failed to create thread";
        }
        cout.flush();
    }
    
    return 0;
}