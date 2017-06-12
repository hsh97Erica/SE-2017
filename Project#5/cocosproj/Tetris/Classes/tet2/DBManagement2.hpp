#ifndef _DB_MGRMNT_H_INC_
#define _DB_MGRMNT_H_INC_

#include <iostream>
#include <cstdio>
#include <cstring>
#include <queue>
#include <ctime>
#include "BaseDBManagement.hpp"
using namespace std;
namespace Tetris{
    namespace DBManagement{
        class DBManager2{
            public:
                static DBManager2* getInstance(){
                    static DBManager2 ins;
                    return &ins;
                }
                DBManager2(){
                    
                }
                ~DBManager2(){

                }
            FILE* getReadOnlyDBFile(){
                return fopen(DB_FILE_NAME2,"rb");
            }
            FILE* getWritableDBFile(){
                return fopen(DB_FILE_NAME2,"ab");
            }
            void closeDBFile(FILE** f){
                FILE* file = *f;
                fclose(file);
                *f = NULL;
            }
            void saveNewRecord(FILE** f,unsigned long long score,bool closefile){
                const int hlen = getHeaderLen();
                if(getFileSize(f)<hlen+sizeof(unsigned long long)){
                    writeHeader(f,false);
                }
                fseek(*f,hlen,SEEK_SET);
                unsigned long long cnt = 999;
                fread(&cnt,sizeof(unsigned long long),1,*f);
                //fread(&cnt,sizeof(unsigned long long),1,*f);
                fseek(*f,hlen,SEEK_SET);
                cout<<"cnt at save: "<<cnt<<endl;
                cnt++;
                fwrite(&cnt,sizeof(unsigned long long),1,*f);
                //fwrite(&cnt,sizeof(unsigned long long),1,*f);
                fseek(*f,0,SEEK_END);
                struct ScoreBoardAttributes attrs = {0,};
                attrs.score = score;
                attrs.ts = time(NULL);
                attrs.dbsaveorder = 0;
                cout<<"attr info before saving: "<<(attrs.score)<<" | ts= "<<(attrs.ts)<<endl;
                fwrite(&attrs,sizeof(struct ScoreBoardAttributes),1,*f);
                if(closefile){
                    fclose(*f);
                    *f = NULL;
                }
            }
            queue<struct ScoreBoardAttributes> getRecords(FILE** f,bool closefile){
                queue<struct ScoreBoardAttributes> q;
                const int hlen = getHeaderLen();
                if(getFileSize(f)<hlen+sizeof(unsigned long long)){
                    writeHeader(f,false);
                    fclose(*f);
                    return q;
                }
                fseek(*f,hlen,SEEK_SET);
                unsigned long long cnt = 0;
                cout<<"hlen: "<<(hlen)<<" ||cur pos: "<<(ftell(*f))<<endl;
                fread(&cnt,sizeof(char),sizeof(unsigned long long),*f);
                cout<<"restore count: "<<cnt<<endl;
                fseek(*f,hlen+sizeof(unsigned long long),SEEK_SET);
                int r;
                struct ScoreBoardAttributes attrs = {0,};
                while((r=fread(&attrs,sizeof(struct ScoreBoardAttributes),1,*f))>0){
                    cout<<"restore attr: "<<(attrs.score)<<" | ts:: "<<(attrs.ts)<<endl;
                    q.push(attrs);
                }
                if(closefile){
                    fclose(*f);
                    *f =NULL;
                }
                return q;
            }
            int getFileSize(FILE** f){
                const int originalpos = ftell(*f);
                fseek(*f,0,SEEK_END);
                const int rst = ftell(*f);
                fseek(*f,originalpos,SEEK_SET);
                return rst;
            }
            char* getHeaderText(){
                return "  TET SCBRD 1.0  \n";
            }
            int getHeaderLen(){
                return strlen(getHeaderText());
            }
        protected:
            void writeHeader(FILE** f,bool closefile){
                char* headername = getHeaderText();
                const int headernamelen = strlen(headername);
                fwrite(headername,sizeof(char),headernamelen,*f);
                unsigned long long boardsz = 0;
                fwrite(&boardsz,sizeof(unsigned long long),1,*f);
                if(closefile){
                    fclose(*f);
                    *f = NULL;
                }
            }
            //private:FILE* f = NULL;
        };
    }
}

#endif
