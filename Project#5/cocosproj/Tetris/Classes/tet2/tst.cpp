#include <iostream>
#include <cmath>
#include <cstdio>
#include <queue>
#include "ScoreManagement.hpp"
#include "BaseDBManagement.hpp"
#include "DBManagement2.hpp"
using namespace std;
using namespace Tetris::DBManagement;
/*int main(){
    DBManager2* mgr = DBManager2::getInstance();
    FILE* fp = mgr->getWritableDBFile();
    for(unsigned long long i=1;i<4;i++){
    mgr->saveNewRecord(&fp,i,false);
    }
    mgr->closeDBFile(&fp);
    cout<<"finish saving"<<endl;
    fp = mgr->getReadOnlyDBFile();
    queue<struct ScoreBoardAttributes> q = mgr->getRecords(&fp,true);
    while(q.size()){
        struct ScoreBoardAttributes attr={0,};
        attr = q.front();
        q.pop();
        cout<<"attr info: "<<(attr.score)<<" | "<<(attr.ts)<<endl;
    }
    cout<<"finish program."<<endl;
    //struct ScoreBoardAttributes attr;
    return 0;
}*/
