#ifndef _Base_DB_MGRMNT_H_INC_
#define _Base_DB_MGRMNT_H_INC_

#define DB_FILE_NAME "tet_db.db"
#define DB_FILE_NAME2 "tet_db.dat"
namespace Tetris{
    namespace DBManagement{
      struct ScoreBoardAttributes{
            int dbsaveorder;
            unsigned long long score;
            unsigned long long playtimesec;
            time_t ts;
            bool datasaved;
        };
    }
}
#endif
