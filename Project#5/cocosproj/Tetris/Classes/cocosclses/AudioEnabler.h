#ifndef __Audio_EN_H_INC_
#define __Audio_EN_H_INC_
/**
    @file AudioEnabler.h
    @brief 음성 라이브러리 로드
*/
// #define USE_AUDIO_ENGINE 1
 #define USE_SIMPLE_AUDIO_ENGINE 1
#if USE_AUDIO_ENGINE && USE_SIMPLE_AUDIO_ENGINE
#error "Don't use AudioEngine and SimpleAudioEngine at the same time. Please just select one in your game!"
#endif


#endif
