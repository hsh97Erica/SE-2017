#include <iostream>
#include <cmath>
#include <cstdio>
#include <queue>
#include "ScoreManagement.hpp"
#include "BaseDBManagement.hpp"
#include "DBManagement.hpp"
using namespace std;
using namespace Tetris::DBManagement;
/*int main(){
    DBManager* mgr = DBManager::getInstance();
    mgr->open("/Users/hsh/Documents/tet/tet_db.db");
    queue<struct ScoreBoardAttributes> q = mgr->getScoreBoard();
    while(q.size()){
        struct ScoreBoardAttributes s = q.front();
        q.pop();
        cout<<"score: "<<(s.score)<<endl;
    }
    return 0;
}*/
