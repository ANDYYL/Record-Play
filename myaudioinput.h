#ifndef MYAUDIOINPUT_H
#define MYAUDIOINPUT_H

#include <QWidget>
#include "ui_myaudioinput.h"
#include <QFile>
#include <QAudioInput>
#include <QAudioOutput>

struct WAVFILEHEADER
{
	// RIFF ͷ;
	char RiffName[4];
	unsigned long nRiffLength;

	// �������ͱ�ʶ��;
	char WavName[4];

	// ��ʽ���еĿ�ͷ;
	char FmtName[4];
	unsigned long nFmtLength;

	// ��ʽ���еĿ�����;
	unsigned short nAudioFormat;
	unsigned short nChannleNumber;
	unsigned long nSampleRate;
	unsigned long nBytesPerSecond;
	unsigned short nBytesPerSample;
	unsigned short nBitsPerSample;

	// ���ݿ��еĿ�ͷ;
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
