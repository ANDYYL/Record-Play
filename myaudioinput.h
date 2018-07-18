#ifndef MYAUDIOINPUT_H
#define MYAUDIOINPUT_H

#include <QWidget>
#include "ui_myaudioinput.h"
#include <QFile>
#include <QAudioInput>
#include <QAudioOutput>

struct WAVFILEHEADER
{
	// RIFF 头;
	char RiffName[4];
	unsigned long nRiffLength;

	// 数据类型标识符;
	char WavName[4];

	// 格式块中的块头;
	char FmtName[4];
	unsigned long nFmtLength;

	// 格式块中的块数据;
	unsigned short nAudioFormat;
	unsigned short nChannleNumber;
	unsigned long nSampleRate;
	unsigned long nBytesPerSecond;
	unsigned short nBytesPerSample;
	unsigned short nBitsPerSample;

	// 数据块中的块头;
	char    DATANAME[4];
	unsigned long   nDataLength;
};

class MyAudioInput : public QWidget              //
{
	Q_OBJECT

public:
	MyAudioInput(QWidget *parent = 0);
	~MyAudioInput();

private Q_SLOTS:
	void onStartRecord();
	void onStopRecording();
	void onPlay();
	void onStopPlay();
	qint64 addWavHeader(QString catheFileName, QString wavFileName);
    void handleStateChanged(QAudio::State state);                       //
private:
	void timerEvent(QTimerEvent *event);

private:
	Ui::MyAudioInput ui;
	QFile destinationFile;
    QAudioInput* m_audioInput;                                        //
	QAudioOutput* m_audioOutput;
	QFile sourceFile;
	bool m_isRecord;
	bool m_isPlay;
	int m_RecordTimerId;
	int m_RecordTime;
};

#endif // MYAUDIOINPUT_H
