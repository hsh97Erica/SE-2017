/**
 @file DBManagement.hpp
 @brief
 */
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
using namespace std;
namespace Tetris{
    namespace DBManagement{
        /**
         @class DBManager
         @brief sqlite3를 통한 db통신
         */
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
                err = sqlite3_open_v2(fname,&conn,SQLITE_OPEN_READWRITE|SQLITE_OPEN_CREATE,NULL);
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
            /**
             @return 일관된 db폴터리턴
             */
            static string getDBLocationForTetrisGame(){
                string toprootfolder = FileUtils::getInstance()->getWritablePath();
                string workfolder = toprootfolder;
                workfolder.append("tet/");
                if(!FileUtils::getInstance()->isDirectoryExist(workfolder)){
                    FileUtils::getInstance()->createDirectory(workfolder);
                }
                string dbfile = workfolder;
                dbfile.append(getDBFileName());
                cout<<"db file path: "<<dbfile<<endl;
                return dbfile;
            }
            /**
             @return db가 제대로 연결됬는지  확인
             */
            bool isOpened(){
                return conn!=NULL;
            }
            /**
             @return 기본적인 db파일리턴
             */
            static char* getDBFileName(){
                return DB_FILE_NAME;
            }
            bool isConnected(){
                return isOpened();
            }
            /**
             @return 리턴은 없고 db에 점수저장
             */
            void saveScore(unsigned long long score,unsigned long long playtime){
                cout<<"call saveScore ain DBMGR"<<endl;
                const time_t ts = time(NULL);
                stringstream ss;
                ss<<"insert into scoreboard(score,playtimesec,recordtimeasts) values(";
                ss<<score<<","<<playtime<<","<<ts<<");";
                string str = (string)ss.str();
                cout<<"[insert query] "<<str<<endl;
                char* sql = NULL;
                sql = (char*) str.c_str();
                if(!conn){
                    return;
                }
                char* errmsg = NULL;
                sqlite3_exec(conn,sql,NULL,NULL,&errmsg);
                cout<<"score saved:: "<<(errmsg!=NULL?errmsg:"")<<endl;
            }
            /**
             @return db에 저장된 기록들을 정렬해 순차적으로 불러와 리턴
             */
            queue<struct ScoreBoardAttributes> getScoreBoard(){
                queue<struct ScoreBoardAttributes> q;
                if(!conn){
                    return q;
                }
                sqlite3_stmt* res2 = NULL;
                char* sql = "select _id,score,playtimesec,recordtimeasts from scoreboard order by score desc,playtimesec desc, recordtimeasts desc;";
                sqlite3_prepare_v2(conn,sql,strlen(sql),&res2,NULL);
                int ret = 0;
                int colcnt = sqlite3_column_count(res2);
                while(true){
                    ret = sqlite3_step(res2);
                    if(ret==SQLITE_ROW){
                        struct ScoreBoardAttributes attrs={0,};
                        attrs.dbsaveorder= sqlite3_column_int(res2,0);
                        attrs.score= sqlite3_column_int(res2,1);
                        attrs.playtimesec = sqlite3_column_int(res2, 2);
                        attrs.ts= (time_t)sqlite3_column_int(res2,3);
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
            /**
             @return 해당 키가 db에 있는지 유무
             */
            bool isExistAppSetting(string key){
                sqlite3_stmt* res2 = NULL;
                string tmpsql = "";
                tmpsql.append("select count(*) as cnt from appsetting where setting_name='");
                tmpsql.append(key);
                tmpsql.append("';");
                char* sql = NULL;
                sql =(char*) tmpsql.c_str();
                sqlite3_prepare_v2(conn,sql,strlen(sql),&res2,NULL);
                int ret = 0;
                int colcnt = sqlite3_column_count(res2);
                while(true){
                    ret = sqlite3_step(res2);
                    if(ret==SQLITE_ROW){
                        if(sqlite3_column_int(res2, 0)<=0)
                        {
                            cout<<"there isn't "<<key<<endl;
                            return false;
                        }
                    }
                    else{
                        break;
                    }
                }
                return true;
            }
            /**
             @return 해당 키값을 가진 값리턴
             */
            bool readAppSettingAsBool(string key){
                return readAppSettingAsInt(key)==0?false:true;
            }
            /**
             @return 해당 키값을 가진 값리턴
             */
            int readAppSettingAsInt(string key){
                if((!conn)||(conn&&!isExistAppSetting(key))){
                    return 0;
                }
                int result =0;
                sqlite3_stmt* res2 = NULL;
                string tmpsql = "";
                tmpsql.append("select value from appsetting where setting_name='");
                tmpsql.append(key);
                tmpsql.append("';");
                char* sql = NULL;
                sql =(char*) tmpsql.c_str();
                sqlite3_prepare_v2(conn,sql,strlen(sql),&res2,NULL);
                int ret = 0;
                int colcnt = sqlite3_column_count(res2);
                while(true){
                    ret = sqlite3_step(res2);
                    if(ret==SQLITE_ROW){
                        cout<<"before setting int result"<<endl;
                        //sqlite3_
                        result = sqlite3_column_int(res2, 0);
                        cout<<"after setting int result"<<result<<endl;
                    }
                    else{
                        break;
                    }
                }
                sqlite3_finalize(res2);
                return result;
            }
            /*void* readAppSettingAsBlob(string key){
                if((!conn)||(conn&&!isExistAppSetting(key))){
                    return NULL;
                }
                void* result = NULL;
                sqlite3_stmt* res2 = NULL;
                string tmpsql = "";
                tmpsql.append("select value from appsetting where setting_name='");
                tmpsql.append(key);
                tmpsql.append("';");
                unsigned char* tmprst = NULL;
                char* sql = NULL;
                sql =(char*) tmpsql.c_str();
                sqlite3_prepare_v2(conn,sql,strlen(sql),&res2,NULL);
                int ret = 0;
                int colcnt = sqlite3_column_count(res2);
                while(true){
                    ret = sqlite3_step(res2);
                    if(ret==SQLITE_ROW){
                        result = sqlite3_column_text(res2, 0);
                    }
                    else{
                        break;
                    }
                }
                sqlite3_finalize(res2);
                return result;
            }*/
            /**
             @return 해당 키값을 가진 값리턴
             */
            string readAppSettingAsText(string key){
                string result ="";
                if((!conn)||(conn&&!isExistAppSetting(key))){
                    return result;
                }
                sqlite3_stmt* res2 = NULL;
                string tmpsql = "";
                tmpsql.append("select value from appsetting where setting_name='");
                tmpsql.append(key);
                tmpsql.append("';");
                char* sql = NULL;
                sql =(char*) tmpsql.c_str();
                sqlite3_prepare_v2(conn,sql,strlen(sql),&res2,NULL);
                int ret = 0;
                int colcnt = sqlite3_column_count(res2);
                while(true){
                    ret = sqlite3_step(res2);
                    if(ret==SQLITE_ROW){
                        
                        result.append((char*)sqlite3_column_text(res2, 0));
                    }
                    else{
                        break;
                    }
                }

                sqlite3_finalize(res2);
                return result;
            }
            /**
             @return 해당 키값을 가진 값이 변경 완료되면 변경성공유무
             */
            bool changeIntSetting(string key,int value){
                if((!conn)||(conn&&!isExistAppSetting(key))){
                    return false;
                }
                string sqlstr = "";
                sqlite3_exec(conn, "BEGIN TRANSACTION;", NULL, NULL, NULL);
                stringstream ss;
                ss<<"update appsetting set value="<<value<<" where setting_name='"<<key<<"';";
                sqlstr = (string)ss.str();
                cout<<"[update query] "<<sqlstr<<endl;
                char* errmsg = NULL;
                err=sqlite3_exec(conn,(char*)sqlstr.c_str(),NULL,NULL,&errmsg);
                logdberr("changeIntSetting");
                sqlite3_exec(conn, "END TRANSACTION;", NULL, NULL, NULL);
                if(err){
                    cout<<errmsg<<endl;
                }
                else{
                    err=sqlite3_exec(conn,"commit;",NULL,NULL,&errmsg);
                    
                }
                return true;
            }
            bool changeBoolSetting(string key,bool value){
                return changeIntSetting(key,value?1:0);
            }
            static string getSoundEnablerKey(){
                return "effect_output";
            }
        private:
            
            sqlite3* conn = NULL; ///< sqlite통신객체
            sqlite3_stmt* res = NULL;
            int err = 0;
            void initVal(){
                conn=NULL;
                res=NULL;
                err=0;
            }
            void logdberr(string functioncategory){
                if(err!=SQLITE_OK){
                    cout<<functioncategory<<" error"<<endl;
                    cout<<"[error state] ";
                    switch(err){
                        case SQLITE_DENY:{
                            cout<<"deny"<<endl;
                            break;
                        }
                        case SQLITE_FAIL:{
                            cout<<"fail"<<endl;
                            break;
                        }
                        case SQLITE_IOERR:{
                            cout<<"io error"<<endl;
                            break;
                        }
                        default:{
                            cout<<"else error"<<endl;
                            break;
                        }
                    }
                }else{
                    cout<<functioncategory<<" OK"<<endl;
                }
            }
            
            void initTable(){
                if(!conn)return;
                err=sqlite3_exec(conn,"create table if not exists scoreboard(_id integer primary key autoincrement,score integer default 0,playtimesec integer default 0,recordtimeasts integer not null);",NULL,NULL,NULL);
                logdberr("initTable1");
                initAppSettingValuesIfNotExist();
                
                
            }
            
            void initAppSettingValuesIfNotExist(){
                err=sqlite3_exec(conn,"create table if not exists appsetting(setting_name text not null, value blob);",NULL,NULL,NULL);
                logdberr("initAppSettingValuesIfNotExist");
                if(!isExistAppSetting(getSoundEnablerKey())){
                    err=sqlite3_exec(conn,"insert into appsetting values('effect_output',1);",NULL,NULL,NULL);
                    logdberr("initAppSettingValuesIfNotExist::insert::effect_output");
                }
            }
        };
        
    }
}
#endif
