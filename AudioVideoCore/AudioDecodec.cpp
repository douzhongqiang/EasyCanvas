#include "AudioDecodec.h"
#include <QWaitCondition>
#include <QDebug>

AudioDecodec::AudioDecodec(QObject* parent)
    :QThread(parent)
    ,m_isPlay(true)
    ,m_isDecodecFinsihed(true)
{
    av_register_all();

    m_pPlayThread = new AudioPlayerThread(m_audioData, this);
    QObject::connect(m_pPlayThread, &AudioPlayerThread::playFinished, this, &AudioDecodec::onPlayFinished);
}

AudioDecodec::~AudioDecodec()
{
    this->stop();

    m_isPlay = false;
    this->requestInterruption();
    m_audioData.m_audioDecodecCondition.wakeAll();

    this->wait();
    closeFile();

    delete m_pPlayThread;

    qDebug() << __FUNCTION__;
}

bool AudioDecodec::openAudioFile(const QString& filePath)
{
    m_pAvFrame = av_frame_alloc();

    // 打开文件
    int result = avformat_open_input(&m_pFormatContent, filePath.toLocal8Bit().data(), nullptr, nullptr);
    if (result != 0)
        qDebug() << "Result is Not Zero!";
    if (m_pFormatContent == nullptr)
        qDebug() << "FormtContent is Nullptr";

    if (result || !m_pFormatContent)
        return false;

    // 查找流信息
    result = avformat_find_stream_info(m_pFormatContent, nullptr);
    if (result < 0)
        return false;

    // 查找音频流
    int streamCount = m_pFormatContent->nb_streams;
    for (int i=0; i<streamCount; ++i)
    {
        if (m_pFormatContent->streams[i]->codec->codec_type == AVMEDIA_TYPE_AUDIO)
        {
            m_nAudioStreamIndex = i;
            break;
        }
    }

    if (m_nAudioStreamIndex < 0)
    {
        avformat_close_input(&m_pFormatContent);
        return false;
    }

    // 获取总时长
    m_nTotalTime = m_pFormatContent->duration * 1.0 / AV_TIME_BASE * 1000;

    // 查找解码器
    m_pCodecContent = m_pFormatContent->streams[m_nAudioStreamIndex]->codec;
    AVCodec* codec = avcodec_find_decoder(m_pCodecContent->codec_id);
    if (codec == nullptr)
    {
        avformat_close_input(&m_pFormatContent);
        return false;
    }

    // 打开解码器
    result = avcodec_open2(m_pCodecContent, codec, nullptr);
    if (result != 0)
    {
        avformat_close_input(&m_pFormatContent);
        return false;
    }

    // 设置采样率等信息
    m_nSampleRate = m_pCodecContent->sample_rate;
    m_nSampleFormat = m_pCodecContent->sample_fmt;
    m_pCodecContent->channel_layout = av_get_default_channel_layout(m_pCodecContent->channels);
    m_nChannelSize = m_pCodecContent->channels;

    m_isDecodecFinsihed = false;

    return true;
}

void AudioDecodec::run(void)
{
    while (!this->isInterruptionRequested())
    {
        // 缓冲区满了，挂起线程
        if (m_pPlayThread->isBufferFull() || !m_isPlay)
        {
            m_audioData.m_audioPlayMutex.lock();
            m_audioData.m_audioDecodecCondition.wait(&m_audioData.m_audioPlayMutex);
            m_audioData.m_audioPlayMutex.unlock();
        }

        AVPacket pkt;
        // 获取帧出错，可能是结束或者出错
        int result = av_read_frame(m_pFormatContent, &pkt);
        if (result != 0)
        {
            // 释放包的内存
            av_packet_unref(&pkt);

            m_isPlay = false;
            m_isDecodecFinsihed = true;
            continue;
        }

        // 只处理音频
        if (pkt.stream_index != m_nAudioStreamIndex)
            continue;

        // 解码音频帧， 发送音频包
        if (avcodec_send_packet(m_pCodecContent, &pkt))
            continue;

        // 解码音频帧，接收音频解码帧
        if (avcodec_receive_frame(m_pCodecContent, m_pAvFrame))
            continue;

        int writedSize = 0;
        // 获取解码后的PCM数据
        int size = m_pPlayThread->addAudioData((char*)m_pAvFrame->data[0], m_pAvFrame->linesize[0]);
        writedSize += size;
        while (writedSize < m_pAvFrame->linesize[0] && m_isPlay)
        {
            m_audioData.m_audioPlayMutex.lock();
            m_audioData.m_audioDecodecCondition.wait(&m_audioData.m_audioPlayMutex);
            m_audioData.m_audioPlayMutex.unlock();

            if (m_isPlay)
            {
                int size = m_pPlayThread->addAudioData((char*)m_pAvFrame->data[0] + writedSize, \
                        m_pAvFrame->linesize[0] - writedSize);
                writedSize += size;
            }
        }

        // 释放包的内存
        av_packet_unref(&pkt);
    }
}

void AudioDecodec::play(void)
{

    int sampleSize = 16;
    m_pPlayThread->setSampleInfo(m_nSampleRate, sampleSize, m_nChannelSize);

    // 播放
    m_pPlayThread->startPlay();

    // 解码
    m_isPlay = true;
    if (!this->isRunning())
        this->start();
    m_audioData.m_audioDecodecCondition.wakeAll();
}

void AudioDecodec::pause(void)
{
    m_pPlayThread->pausePlay();
}

void AudioDecodec::stop(void)
{
    m_isPlay = false;
    m_audioData.m_audioDecodecCondition.wakeAll();

    m_pPlayThread->stop();
    if (m_nAudioStreamIndex >= 0)
        av_seek_frame(m_pFormatContent, m_nAudioStreamIndex, 0, AVSEEK_FLAG_BACKWARD);
    m_isDecodecFinsihed = false;
}

// 关闭文件
void AudioDecodec::closeFile(void)
{
    av_frame_free(&m_pAvFrame);

    if (m_pFormatContent)
        avformat_close_input(&m_pFormatContent);
}

bool AudioDecodec::isDecodecFinished(void)
{
    bool result = m_isDecodecFinsihed;
    return result;
}

// 获取音频信息
int AudioDecodec::getTotalTime(void)
{
    return m_nTotalTime;
}

void AudioDecodec::getSampleInfos(int &sampleRate, int &sampleSize, int &channelCount)
{
    sampleRate = m_nSampleRate;
    sampleSize = 16;
    channelCount = m_nChannelSize;
}

void AudioDecodec::onPlayFinished(void)
{
    if (m_isDecodecFinsihed)
        emit playFinished();
}
