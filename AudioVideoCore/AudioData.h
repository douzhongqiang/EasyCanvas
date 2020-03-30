#ifndef AUDIODATA_H
#define AUDIODATA_H

#include <QWaitCondition>
#include <QMutex>
#include <atomic>
#include <memory>
#include <QDebug>

struct AudioData
{
    QMutex m_audioPlayMutex;
    QWaitCondition m_audioPlayCondition;
    QWaitCondition m_audioDecodecCondition;

    std::atomic<int> m_nStartIndex;
    std::atomic<int> m_nEndIndex;
    std::atomic<int> m_nTotalCanUsed;

    AudioData()
        :m_nStartIndex(0)
        ,m_nEndIndex(0)
        ,m_nTotalCanUsed(0){}

    ~AudioData(){}
};

#endif
