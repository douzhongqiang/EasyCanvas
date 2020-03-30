#ifndef AUDIOPLAYERTHREAD_H
#define AUDIOPLAYERTHREAD_H

#include <QThread>
#include <QAudioFormat>
#include <QAudioOutput>
#include <QByteArray>
#include <QMutex>
#include <QWaitCondition>
#include <atomic>
#include <QIODevice>

class DecodecVideo;
class AudioData;

class AudioPlayDecice : QIODevice
{
    Q_OBJECT

public:
};

class AudioPlayerThread : public QThread
{
    Q_OBJECT

public:
    AudioPlayerThread(AudioData& audioData, QObject* parent = nullptr);
    ~AudioPlayerThread() override;

    void run(void) override;

    // set sample infos
    void setSampleInfo(int sampleRate, int sampleSize, int channelCount);
    // 添加数据
    int addAudioData(const char* pAudioData, int length);
    // 开始播放
    void startPlay(void);
    // 暂停
    void pausePlay(void);
    // 结束
    void stop(void);
    // 判断缓存区是否已经满了
    bool isBufferFull(void);

private:
    QAudioFormat m_format;
    QAudioOutput* m_pAudioOutput = nullptr;
    QIODevice* m_pIODevice = nullptr;

    // 音频数据Buffer
    char* m_pAudioBuffer = nullptr;
    const int m_nMaxBufferSize = 1 * 1024 * 1024;      // 1M的音频数据Buffer
    void initAudioBuffer(void);

    std::atomic<bool> m_isPlay;
    AudioData& m_audioData;

signals:
    void playFinished(void);
};
#endif
