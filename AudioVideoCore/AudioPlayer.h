#ifndef AUDIOPLAYER_H
#define AUDIOPLAYER_H

#include <QObject>
#include "AudioDecodec.h"
#include "audiovideocore_global.h"

class AUDIOVIDEOCORESHARED_EXPORT AudioPlayer : public QObject
{
    Q_OBJECT

public:
    struct AudioInfo
    {
        int sampleRate;     // 采样率
        int sampleSize;     // 采样大小
        int channelSize;    // 通道数

        int totalTime;      // 总时长ms
    };

    enum PlayStatus
    {
        t_normal,
        t_playing,
        t_pause
    };

public:
    AudioPlayer(QObject* parent = nullptr);
    ~AudioPlayer();

    // 打开一个音频文件
    bool openAudioFile(const QString& fileName);
    // 关闭文件
    void close(void);
    // 获取音频信息
    AudioInfo getInfo(void);

    // 开始播放
    void play(void);
    // 暂停播放
    void pause(void);
    // 结束
    void stop(void);

    // 获取当前播放状态
    PlayStatus getCurrentPlayStatus(void);

private:
    AudioDecodec* m_pAudioPlayer = nullptr;
    PlayStatus m_playerStatus = t_normal;

private slots:
    void onPlayFinished(void);

signals:
    void playFinished(void);
};

#endif
