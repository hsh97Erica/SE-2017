/**
 @file BaseDBManagement.hpp
 @brief
 */
#ifndef _Base_DB_MGRMNT_H_INC_
#define _Base_DB_MGRMNT_H_INC_

#define DB_FILE_NAME "tet_db.db"
#define DB_FILE_NAME2 "tet_db.dat"
namespace Tetris{
    namespace DBManagement{
        /**
         @brief 내부에서 데이터를 기록 단위로 읽기 위해서 데이터를 pack하기 위한 구조체
         */
      struct ScoreBoardAttributes{
            int dbsaveorder; ///< db에서 자동으로 생성되는 인덱스를 담음
            unsigned long long score; ///< 기록된 점수를 저장
            unsigned long long playtimesec; ///< 플레이한 시간을 초단위로 가져옴
            time_t ts; ///< 기록한 시간을 타임스템프로(TimeStamp)형식으로 저장
            bool datasaved; ///< 순수하게 구조체들은 파일로 저장하려고 했을때 사용하려고한 변수
        };
    }
}
#endif
