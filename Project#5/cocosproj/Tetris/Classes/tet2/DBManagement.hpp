#ifndef __DB_MGRMNT_H_INC_
#define __DB_MGRMNT_H_INC_

//#include "base/sqlite3.h"
#include "../db/sqlitelib/sqlite3.h"
#include "BaseDBManagement.hpp"
#include <ctime>
#include <string>
#include <cstring>
#include <sstream>
#include <queue>

namespace Tetris{
    namespace DBManagement{
        
        class DBManager{
        public:
            static DBManager* getInstance(){
                static DBManager ins;
                return &ins;
            }
            DBManager(){
                initVal();
                //open(getDBFileName());
                //initTable();
            }
            ~DBManager(){
                close();
            }
            void open(char* fname){
                if(!conn){
                    close();
                }
                err = sqlite3_open(fname,&conn);
                if(err!=SQLITE_OK){
                    close();
                    return;
                }
                initTable();
            }
            void close(){
                sqlite3_close(conn);
                err = 0;
                conn = NULL;
            }
            bool isOpened(){
                return conn!=NULL;
            }
            static char* getDBFileName(){
                return DB_FILE_NAME;
            }
            void saveScore(unsigned long long score){
                const time_t ts = time(NULL);
                stringstream ss;
                ss<<"insert into scoreboard(score,recoardtimeasts) values(";
                ss<<score<<","<<ts<<");";
                char* sql = NULL;
                sql = (char*) ss.str().c_str();
                if(!conn){
                    return;
                }
                char* errmsg = NULL;
                sqlite3_exec(conn,sql,NULL,NULL,&errmsg);
                cout<<"score saved - "<<(errmsg!=NULL?errmsg:"")<<endl;
            }
            queue<struct ScoreBoardAttributes> getScoreBoard(){
                queue<struct ScoreBoardAttributes> q;
                if(!conn){
                    return q;
                }
                sqlite3_stmt* res2 = NULL;
                char* sql = "select _id,score,recoardtimeasts from scoreboard;";
                sqlite3_prepare_v2(conn,sql,strlen(sql),&res2,NULL);
                int ret = 0;
                int colcnt = sqlite3_column_count(res2);
                while(true){
                    ret = sqlite3_step(res2);
                    if(ret==SQLITE_ROW){
                        struct ScoreBoardAttributes attrs={0,};
                        attrs.dbsaveorder= sqlite3_column_int(res2,0);
                        attrs.score= sqlite3_column_int(res2,1);
                        attrs.ts= (time_t)sqlite3_column_int(res2,2);
                        attrs.datasaved = true;
                        /*for(int i=0;i<colcnt;i++){
                            switch(sqlite3_column_type(res2, i)){case}
                        }*/ // type check for future
                        q.push(attrs);
                    }
                    else{
                        break;
                    }
                }
                sqlite3_finalize(res2);
                //sqlite3_prepare(conn,sql,strlen(sql),&res,NULL);
                return q;
            }
        private:
            
            sqlite3* conn = NULL;
            sqlite3_stmt* res = NULL;
            int err = 0;
            void initVal(){
                conn=NULL;
                res=NULL;
                err=0;
            }
            void initTable(){
                if(!conn)return;
                err=sqlite3_exec(conn,"create table if not exists scoreboard(_id integer primary key auto increment,score integer default 0,recordtimeasts integer not null);",NULL,NULL,NULL);
            }
        };
        
    }
}
#endif
