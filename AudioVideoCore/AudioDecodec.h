#ifndef AUDIODECODEC_H
#define AUDIODECODEC_H

extern "C" {
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libavutil/avutil.h>
}
#include <QString>
#include <QThread>
#include "AudioData.h"
#include "AudioPlayerThread.h"
#include "audiovideocore_global.h"

class AUDIOVIDEOCORESHARED_EXPORT AudioDecodec : public QThread
{
    Q_OBJECT

public:
    AudioDecodec(QObject* parent = nullptr);
    ~AudioDecodec();

    // 打开音频文件
    bool openAudioFile(const QString& filePath);
    // 关闭文件
    void closeFile(void);

    // 获取是否结束
    bool isDecodecFinished(void);

    // 获取音频信息
    int getTotalTime(void);
    void getSampleInfos(int &sampleRate, int &sampleSize, int &channelCount);

    // 开始解码并播放
    void play(void);
    // 暂停
    void pause(void);
    // 结束播放
    void stop(void);

    void run(void) override;

private:
    AVFormatContext* m_pFormatContent = nullptr;
    AVCodecContext* m_pCodecContent = nullptr;

    int m_nAudioStreamIndex = -1;
    qreal m_nTotalTime = 0.0; // ms

    int m_nSampleRate = 0;      // 采样率
    AVSampleFormat m_nSampleFormat;     // 采样大小
    int m_nChannelSize = 2;     // 通道数

    AVFrame *m_pAvFrame = nullptr;
    AudioPlayerThread* m_pPlayThread= nullptr;

    std::atomic<bool> m_isPlay;
    std::atomic<bool> m_isDecodecFinsihed;

    // 音频相关数据
    AudioData m_audioData;
    friend AudioPlayerThread;

signals:
    void playFinished(void);

private slots:
    void onPlayFinished(void);
};

#endif
