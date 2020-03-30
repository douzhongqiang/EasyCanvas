#include "AudioPlayerThread.h"
#include "AudioData.h"
#include <QMutexLocker>
#include <QDebug>

AudioPlayerThread::AudioPlayerThread(AudioData& audioData, QObject* parent)
    :QThread(parent)
    ,m_isPlay(true)
    ,m_audioData(audioData)
{
    initAudioBuffer();
}

AudioPlayerThread::~AudioPlayerThread()
{
    m_isPlay = false;
    this->requestInterruption();
    m_audioData.m_audioPlayCondition.wakeAll();
    this->wait();

    stop();

    delete[] m_pAudioBuffer;

    qDebug() << __FUNCTION__;
}

void AudioPlayerThread::initAudioBuffer(void)
{
    m_pAudioBuffer = new char[m_nMaxBufferSize];
    memset(m_pAudioBuffer, 0, m_nMaxBufferSize);
}

int AudioPlayerThread::addAudioData(const char* pAudioData, int length)
{
    int canAddedSize = m_nMaxBufferSize - m_audioData.m_nTotalCanUsed;
    int destSize = qMin(canAddedSize, length);
    destSize = destSize / 4 * 4;

    if (destSize == 0)
        return destSize;

    int nTempEndIndex = m_audioData.m_nEndIndex;
    if (m_nMaxBufferSize - nTempEndIndex >= destSize)
        memcpy(m_pAudioBuffer + m_audioData.m_nEndIndex, pAudioData, destSize);
    else
    {
        int size = m_nMaxBufferSize - m_audioData.m_nEndIndex;
        memcpy(m_pAudioBuffer + m_audioData.m_nEndIndex, pAudioData, size);
        memcpy(m_pAudioBuffer + 0, pAudioData + size, destSize - size);
    }

    // 重新设置结束标志
    m_audioData.m_nEndIndex += destSize;
    if (m_audioData.m_nEndIndex >= m_nMaxBufferSize)
        m_audioData.m_nEndIndex -= m_nMaxBufferSize;

    m_audioData.m_nTotalCanUsed += destSize;
    m_audioData.m_audioPlayCondition.wakeAll();
    return destSize;
}

// 判断缓存区是否已经满了
bool AudioPlayerThread::isBufferFull(void)
{
    if (m_audioData.m_nTotalCanUsed >= m_nMaxBufferSize)
        return true;

    return false;
}

void AudioPlayerThread::setSampleInfo(int sampleRate, int sampleSize, int channelCount)
{
    m_format.setSampleRate(sampleRate);
    m_format.setSampleSize(sampleSize);
    m_format.setChannelCount(channelCount);
    m_format.setCodec("audio/pcm");
    m_format.setByteOrder(QAudioFormat::LittleEndian);
    m_format.setSampleType(QAudioFormat::SignedInt);

    if (m_pAudioOutput)
        delete m_pAudioOutput;
    m_pAudioOutput = new QAudioOutput(m_format, this);
    m_pIODevice = m_pAudioOutput->start();
    qDebug() << m_pAudioOutput->state();

    if (m_pAudioOutput == nullptr)
        qDebug() << "AudioOutPut Is Nullptr";
    if (m_pIODevice == nullptr)
        qDebug() << "m_pIODevice Is Nullptr" << m_pAudioOutput->error();
}

// 开始播放
void AudioPlayerThread::startPlay(void)
{
    if (!this->isRunning())
        this->start();

    m_audioData.m_nStartIndex = 0;
    m_audioData.m_nEndIndex = 0;
    m_audioData.m_nTotalCanUsed = 0;

    memset(m_pAudioBuffer, 0, m_nMaxBufferSize);
    m_isPlay = true;
}

void AudioPlayerThread::pausePlay(void)
{
    m_isPlay = false;
}

void AudioPlayerThread::run(void)
{
    while (!this->isInterruptionRequested())
    {
        // 线程挂起，等待被唤醒
        if (m_audioData.m_nTotalCanUsed <= 0 || !m_isPlay)
        {
            // 发送播放结束
            if (m_audioData.m_nTotalCanUsed <= 0)
                emit playFinished();

            m_audioData.m_audioPlayMutex.lock();
            m_audioData.m_audioPlayCondition.wait(&m_audioData.m_audioPlayMutex);
            m_audioData.m_audioPlayMutex.unlock();
        }

        // 数据比较少，立即播放
        int nTotalCanUsed = m_audioData.m_nTotalCanUsed;
        if (m_pAudioOutput->bytesFree() >= nTotalCanUsed && m_isPlay)
        {
            if (nTotalCanUsed + m_audioData.m_nStartIndex <= m_nMaxBufferSize)
            {
                // 直接写入数据
                m_pIODevice->write(m_pAudioBuffer + m_audioData.m_nStartIndex, nTotalCanUsed);
            }
            else
            {
                int size = m_nMaxBufferSize - m_audioData.m_nStartIndex;
                m_pIODevice->write(m_pAudioBuffer + m_audioData.m_nStartIndex, size);
                m_pIODevice->write(m_pAudioBuffer + 0, nTotalCanUsed - size);
            }

            // 设置起始位置
            m_audioData.m_nStartIndex += nTotalCanUsed;
            if (m_audioData.m_nStartIndex >= m_nMaxBufferSize)
                m_audioData.m_nStartIndex -= m_nMaxBufferSize;

            m_audioData.m_nTotalCanUsed -= nTotalCanUsed;

            // 唤醒解码线程
            m_audioData.m_audioDecodecCondition.wakeAll();

            continue;
        }

        // 数据比较多，循环播放
        int perWriteSize = m_pAudioOutput->periodSize();
        if (m_pAudioOutput->bytesFree() >= perWriteSize && m_isPlay)
        {
            if (perWriteSize + m_audioData.m_nStartIndex <= m_nMaxBufferSize)
            {
                // 直接写入数据
                m_pIODevice->write(m_pAudioBuffer + m_audioData.m_nStartIndex, perWriteSize);
            }
            else
            {
                int size = m_nMaxBufferSize - m_audioData.m_nStartIndex;
                m_pIODevice->write(m_pAudioBuffer + m_audioData.m_nStartIndex, size);
                m_pIODevice->write(m_pAudioBuffer + 0, perWriteSize - size);
            }

            m_audioData.m_nStartIndex += perWriteSize;
            if (m_audioData.m_nStartIndex >= m_nMaxBufferSize)
                m_audioData.m_nStartIndex -= m_nMaxBufferSize;

            m_audioData.m_nTotalCanUsed -= perWriteSize;

            // 唤醒解码线程
            m_audioData.m_audioDecodecCondition.wakeAll();
        }

        QThread::msleep(10);
    }
}

void AudioPlayerThread::stop(void)
{
    m_isPlay = false;
    m_audioData.m_nStartIndex = 0;
    m_audioData.m_nEndIndex = 0;
    m_audioData.m_nTotalCanUsed = false;

    memset(m_pAudioBuffer, 0, m_nMaxBufferSize);
}
