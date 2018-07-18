#include "myaudioinput.h"
#include <QAudioDeviceInfo>
#include <QDebug>
#include <QMessageBox>

#define RAW_RECORD_FILENAME "C:/Users/ANDY/Desktop/MulitimediaExperience/Project/luyin/test.raw"			// ¼���ļ���(it should be in the same address with the project);
#define WAV_RECORD_FILENAME	"C:/Users/ANDY/Desktop/MulitimediaExperience/Project/luyin/test.wav"			// ¼���ļ�תwav��ʽ�ļ���;

const qint64 TIME_TRANSFORM = 1000 * 1000;				// ΢��ת��;

MyAudioInput::MyAudioInput(QWidget *parent)
	: QWidget(parent)
	, m_isRecord(false)
	, m_isPlay(false)
	, m_RecordTimerId(0)
    , m_RecordTime(0)
{
	ui.setupUi(this);
	// ¼�������� �Ȱ�ť �źŲ�;
	connect(ui.pButtonRecord, SIGNAL(clicked()), this, SLOT(onStartRecord()));
	connect(ui.pButtonStopRecord, SIGNAL(clicked()), this, SLOT(onStopRecording()));
	connect(ui.pButtonPlay, SIGNAL(clicked()), this, SLOT(onPlay()));
	connect(ui.pButtonStopPlay, SIGNAL(clicked()), this, SLOT(onStopPlay()));
	
	// �����ǰ�豸֧�ֵ���Ƶ�����ʽ;
	QAudioDeviceInfo info(QAudioDeviceInfo::defaultOutputDevice());
	qDebug()<< "AudioDevice supportedCodecs : " << info.supportedCodecs();
}

MyAudioInput::~MyAudioInput()
{

}

void MyAudioInput::onStartRecord()
{
	// ������ڲ�����ֹͣ����;
	if (m_isPlay)
	{
		onStopPlay();
	}
	// �����ǰû�п�ʼ¼��������¼��;
	if (!m_isRecord)
	{
		// �жϱ����豸�Ƿ�֧�ָø�ʽ
		QAudioDeviceInfo audioDeviceInfo = QAudioDeviceInfo::defaultInputDevice();
		// �жϱ����Ƿ���¼���豸;
		if (!audioDeviceInfo.isNull())
		{
			m_isRecord = true;
			destinationFile.setFileName(RAW_RECORD_FILENAME);
			destinationFile.open(QIODevice::WriteOnly | QIODevice::Truncate);

			// ������Ƶ�ļ���ʽ;
			QAudioFormat format;
			// ���ò���Ƶ��;
			format.setSampleRate(8000);
			// ����ͨ����;
			format.setChannelCount(1);
			// ����ÿ�β����õ�����������λֵ;
			format.setSampleSize(16);
			// ���ñ��뷽��;
			format.setCodec("audio/pcm");
			// ���ò����ֽڴ洢˳��;
			format.setByteOrder(QAudioFormat::LittleEndian);
			// ���ò�������;
			format.setSampleType(QAudioFormat::UnSignedInt);
			
			// �жϵ�ǰ�豸�����Ƿ�֧�ָ���Ƶ��ʽ;
			if (!audioDeviceInfo.isFormatSupported(format))
			{
				qDebug() << "Default format not supported, trying to use the nearest.";
				format = audioDeviceInfo.nearestFormat(format);
			}
			// ��ʼ¼��;
			m_audioInput = new QAudioInput(format, this);
			m_audioInput->start(&destinationFile);

			// ����ʱ�ӣ����ڸ��µ�ǰ¼��ʱ��;
			if (m_RecordTimerId == 0)
			{
				m_RecordTimerId = startTimer(100);
			}
		}
		else
		{
			// û��¼���豸;
			QMessageBox::information(NULL, tr("Record"), tr("Current No Record Device"));
		}
	}
	else
	{
		// ��ǰ����¼��;
		QMessageBox::information(NULL, tr("Record"), tr("Current is Recording"));
	}	
}

void MyAudioInput::onStopRecording()
{
	// ��ǰ����¼��ʱ��ֹͣ¼��;
	if (m_isRecord)
	{
		// �ر�ʱ�ӣ�����ʼ������;
		killTimer(m_RecordTimerId);
		m_RecordTime = 0;
		m_RecordTimerId = 0;
		m_isRecord = false;
		ui.labelTime->setText(QString("Idle : %1/S").arg(m_RecordTime));

		if (m_audioInput != NULL)
		{
			m_audioInput->stop();
			destinationFile.close();
			delete m_audioInput;
			m_audioInput = NULL;
		}

		// �����ɵ�.raw�ļ�ת��.wav��ʽ�ļ�;
		if (addWavHeader(RAW_RECORD_FILENAME, WAV_RECORD_FILENAME) > 0)
			QMessageBox::information(NULL, tr("Save"), tr("RecordFile Save Success"));
	}
}

void MyAudioInput::onPlay()
{
	// ��ǰû��¼���Ų���;
	if (!m_isRecord)
	{
		// ������ڲ�����رղ��ţ�׼�����²���;
		if (m_isPlay)
		{
			onStopPlay();
		}
		m_isPlay = true;
		sourceFile.setFileName(WAV_RECORD_FILENAME);
		sourceFile.open(QIODevice::ReadOnly);

		// ���ò�����Ƶ��ʽ;
		QAudioFormat format;
		format.setSampleRate(8000);
		format.setChannelCount(1);
		format.setSampleSize(16);
		format.setCodec("audio/pcm");
		format.setByteOrder(QAudioFormat::LittleEndian);
		format.setSampleType(QAudioFormat::UnSignedInt);

		QAudioDeviceInfo info(QAudioDeviceInfo::defaultOutputDevice());
		qDebug() << info.supportedCodecs();
		if (!info.isFormatSupported(format))
		{
			qWarning() << "Raw audio format not supported by backend, cannot play audio.";
			return;
		}

		m_audioOutput = new QAudioOutput(format, this);
		connect(m_audioOutput, SIGNAL(stateChanged(QAudio::State)), this, SLOT(handleStateChanged(QAudio::State)));
		m_audioOutput->start(&sourceFile);
		// ����ʱ�ӣ����ڸ��µ�ǰ����ʱ��;
		if (m_RecordTimerId == 0)
		{
			m_RecordTimerId = startTimer(100);
		}
	}	
	else
	{
		// ��ǰ����¼��;
		QMessageBox::information(NULL, tr("Record"), tr("Current is Recording"));
	}
}

void MyAudioInput::onStopPlay()
{
	// �����ǰ�ڲ�����ֹͣ����;
	if (m_isPlay)
	{
		// �ر�ʱ�ӣ�����ʼ������;
		killTimer(m_RecordTimerId);
		m_RecordTime = 0;
		m_RecordTimerId = 0;
		m_isPlay = false;
		ui.labelTime->setText(QString("Idle : %1/S").arg(m_RecordTime));
		if (m_audioOutput != NULL)
		{
			m_audioOutput->stop();
			sourceFile.close();
			delete m_audioOutput;
			m_audioOutput = NULL;
		}
	}	
}

// ����״̬����;
void MyAudioInput::handleStateChanged(QAudio::State state)
{
	switch (state) {
	case QAudio::IdleState:
		// Finished playing (no more data)
		onStopPlay();
		break;

	case QAudio::StoppedState:
		// Stopped for other reasons
		if (m_audioOutput->error() != QAudio::NoError) {
			// Error handling
		}
		break;

	default:
		// ... other cases as appropriate
		break;
	}
}

// ʱ���¼�,����״̬;
void MyAudioInput::timerEvent(QTimerEvent *event)
{
	if (event->timerId() == m_RecordTimerId)
	{
		QString strState;
		if (m_isRecord)
		{
			strState = "Recording";
			m_RecordTime = m_audioInput->elapsedUSecs() / TIME_TRANSFORM;
		}
		else if (m_isPlay)
		{
			strState = "Playing";
			m_RecordTime = m_audioOutput->elapsedUSecs() / TIME_TRANSFORM;
		}
		
		ui.labelTime->setText(QString("%1 : %2/S").arg(strState).arg(m_RecordTime));
	}
}

// �����ɵ�.raw�ļ�ת��.wav��ʽ�ļ�;
qint64 MyAudioInput::addWavHeader(QString catheFileName , QString wavFileName)
{
	// ��ʼ׼��WAV���ļ�ͷ  
	WAVFILEHEADER WavFileHeader;
	qstrcpy(WavFileHeader.RiffName, "RIFF");
	qstrcpy(WavFileHeader.WavName, "WAVE");
	qstrcpy(WavFileHeader.FmtName, "fmt ");

	WavFileHeader.nFmtLength = 16;  //  ��ʾ FMT �ĳ���  
	WavFileHeader.nAudioFormat = 1; //�����ʾ PCM ����; 

	qstrcpy(WavFileHeader.DATANAME, "data");
	
	WavFileHeader.nBitsPerSample = 16;
	WavFileHeader.nBytesPerSample = 2;  
	WavFileHeader.nSampleRate = 8000;
	WavFileHeader.nBytesPerSecond = 16000;
	WavFileHeader.nChannleNumber = 1;

	QFile cacheFile(catheFileName);
	QFile wavFile(wavFileName);

	if (!cacheFile.open(QIODevice::ReadWrite))
	{
		return -1;
	}
	if (!wavFile.open(QIODevice::WriteOnly))
	{
		return -2;
	}

	int nSize = sizeof(WavFileHeader);
	qint64 nFileLen = cacheFile.bytesAvailable();

	WavFileHeader.nRiffLength = nFileLen - 8 + nSize;
	WavFileHeader.nDataLength = nFileLen;
	
	// �Ƚ�wav�ļ�ͷ��Ϣд�룬�ٽ���Ƶ����д��;
	wavFile.write((char *)&WavFileHeader, nSize);
	wavFile.write(cacheFile.readAll());
	
	cacheFile.close();
	wavFile.close();

	return nFileLen;
}
