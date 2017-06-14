/**
  @file AudioSourceManager.hpp
  @brief 실패해서 안씀
 */
#ifndef _AudioSRC_MGR_H_INC_
#define _AudioSRC_MGR_H_INC_

#include "AudioEnabler.h"
#include "cocos2d.h"
using namespace cocos2d;
#if USE_AUDIO_ENGINE
#include "audio/include/AudioEngine.h"
using namespace cocos2d::experimental;
#elif USE_SIMPLE_AUDIO_ENGINE
#include "audio/include/SimpleAudioEngine.h"
using namespace CocosDenshion;
#endif

namespace Tetris{
namespace Audio{
    class AudioSourceManager{
        public:
            static AudioSourceManager* getInstance(){
                static AudioSourceManager Ins;
                return &Ins;
            }
            unsigned int getMainMenuBGMPlayID(){
                return mainmenuid;
            }
        void playRemovingBlocksEffect(){
            CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("res/bgms/rmblksbgm.mp3");
        }
            //void
        private:
            unsigned int  mainmenuid;
           // unsigned int 
    };
}
}
#endif
