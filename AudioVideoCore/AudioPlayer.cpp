#include "AudioPlayer.h"

AudioPlayer::AudioPlayer(QObject* parent)
    :QObject(parent)
{
    m_pAudioPlayer = new AudioDecodec(this);
    QObject::connect(m_pAudioPlayer, &AudioDecodec::playFinished, this, &AudioPlayer::onPlayFinished);
}

AudioPlayer::~AudioPlayer()
{

}

// 打开一个音频文件
bool AudioPlayer::openAudioFile(const QString& fileName)
{
    return m_pAudioPlayer->openAudioFile(fileName);
}

// 关闭文件
void AudioPlayer::close(void)
{
    m_pAudioPlayer->closeFile();
}

// 获取音频信息
AudioPlayer::AudioInfo AudioPlayer::getInfo(void)
{
    AudioInfo audioInfo;
    audioInfo.totalTime = m_pAudioPlayer->getTotalTime();
    m_pAudioPlayer->getSampleInfos(audioInfo.sampleRate, audioInfo.sampleSize, audioInfo.channelSize);

    return audioInfo;
}

// 开始播放
void AudioPlayer::play(void)
{
    m_pAudioPlayer->play();
    m_playerStatus = t_playing;
}

// 暂停播放
void AudioPlayer::pause(void)
{
    m_pAudioPlayer->pause();
    m_playerStatus = t_pause;
}
// 结束
void AudioPlayer::stop(void)
{
    m_pAudioPlayer->stop();
    m_playerStatus = t_normal;
}

AudioPlayer::PlayStatus AudioPlayer::getCurrentPlayStatus(void)
{
    return m_playerStatus;
}

void AudioPlayer::onPlayFinished(void)
{
    this->stop();
    emit playFinished();
}
