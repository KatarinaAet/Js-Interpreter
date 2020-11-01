#ifndef CLIENTWIDGETS
#define CLIENTWIDGETS

#include <QVBoxLayout>
#include <QTextEdit>
#include <QToolBar>
#include <QLineEdit>
#include <QTcpSocket>
#include <QWidget>
#include <QPushButton>
#include <QSplitter>
#include <QLabel>
#include <QByteArray>
#include <QAction>
#include <QSpinBox>
#include <QDebug>
#include <QTime>
#include <QCheckBox>
#include "SpoScriptEngine.h"
#include <QFileDialog>
#include <QFile>
#include <QApplication>
#include <QFont>


class ClientWidgets : public QWidget
{
    Q_OBJECT
signals:
    void signalSendMessage(QString message);
public:
    ClientWidgets(SpoJSon *Engine,QWidget *parent = 0): QWidget(parent){
        _tcpSocket = new QTcpSocket(this);

        _engine = Engine;
        _engine->addObject("console",this);


        QFont newFont("", 12, QFont::Bold, true);
        _toolBar = new QToolBar(this);
        _secondSplitter= new QSplitter(Qt::Vertical,  this);
        _firstSplitter = new QSplitter(Qt::Horizontal,this);
        _thirdSplitter = new QSplitter(Qt::Horizontal,this);
        _lay = new QVBoxLayout(this);
        _calculateButton = new QAction("Calculate",this);
        //connects для работы внутреннего скрипта

        connect(_calculateButton, SIGNAL(triggered(bool)),this, SLOT(slotOnPushButtonCalculate()));

        _checkBoxForClean = new QCheckBox("AutoClean",this);
        _cleanButton = new QAction("Clean Result and logs",this);
        connect(_cleanButton, SIGNAL(triggered(bool)),this,SLOT(slotCleanWindow()));
        _loadFromButton = new QAction("Load",this);
        connect(_loadFromButton,SIGNAL(triggered(bool)),this,SLOT(slotOnPushDialogLoad()));
        _saveToButton = new QAction("Save",this);
        connect(_saveToButton,SIGNAL(triggered(bool)),this,SLOT(slotOnPushDialogSave()));
        connect(this,SIGNAL(signalSendMessage(QString)),this,SLOT(slotReceiveMessageFromClientScript(QString)));


        _textEditInput = new QTextEdit(this);
        _textEditInfo = new QTextEdit(this);
        _textEditResult = new QTextEdit(this);
        _textEditLog = new QTextEdit(this);

        _textEditResult->setFont(newFont);
        _textEditInput->setFont(newFont);
        _textEditLog->setFont(newFont);


        _lay->addWidget(_toolBar);
            _toolBar->addAction(_calculateButton);
            _toolBar->addWidget(_checkBoxForClean);
            _toolBar->addAction(_cleanButton);
            _toolBar->addAction(_saveToButton);
            _toolBar->addAction(_loadFromButton);

        _lay->addWidget(_firstSplitter);
            _firstSplitter->addWidget(_secondSplitter);
                _secondSplitter->addWidget(_textEditInput);
                _secondSplitter->addWidget(_thirdSplitter);
                    _thirdSplitter->addWidget(_textEditResult);
                    _thirdSplitter->addWidget(_textEditLog);
                    _thirdSplitter->setSizes(QList<int>()<<300<<100);
                _secondSplitter->setSizes(QList<int>()<<200<<100);
            _firstSplitter->addWidget(_textEditInfo);
            _firstSplitter->setSizes(QList<int>()<<500<<250);
    }
public slots:
    void log(const QString& text) {
        QTextCursor _cursorLog = _textEditLog->textCursor();
        QTextCharFormat format;

        format.setForeground(Qt::green);
        _cursorLog.movePosition(QTextCursor::Start);
        if(!_textEditLog->toPlainText().isEmpty()) _cursorLog.insertText("\n");
        _cursorLog.movePosition(QTextCursor::Start);

        _cursorLog.setBlockCharFormat(format);
        _cursorLog.insertText("log:"+text);

    }
private:
    QTcpSocket *_tcpSocket;
    quint16     _nextBlockSize;
    QTextEdit *_textEditInput, *_textEditInfo, *_textEditResult,*_textEditLog;
    QVBoxLayout *_lay;
    QToolBar *_toolBar;

    QSplitter *_firstSplitter,*_secondSplitter,*_thirdSplitter;

    QAction *_cleanButton, *_calculateButton,*_loadFromButton,*_saveToButton;
    QCheckBox *_checkBoxForClean;
    SpoJSon *_engine;
private slots:

    void slotCleanWindow(){
        _textEditResult->setText("");
        _textEditLog->setText("");

    }

    void slotReceiveMessageFromClientScript(QString message){
        QString result = _engine->parseMsg(message);
        QTextCursor _cursorDown = _textEditResult->textCursor();
        QTextCharFormat format;

        if(_checkBoxForClean->isChecked()){
            _textEditInput->setText("");
        }

        format.setForeground(Qt::blue);
        _cursorDown.movePosition(QTextCursor::Start);
        if(!_textEditResult->toPlainText().isEmpty()) _cursorDown.insertText("\n");
        _cursorDown.movePosition(QTextCursor::Start);
        _cursorDown.setBlockCharFormat(format);
        _cursorDown.insertText("->"+result);

    }
    void slotOnPushButtonCalculate(){
            emit signalSendMessage(_textEditInput->toPlainText());
    }
    void slotOnPushDialogLoad(){
        QString fileName = QFileDialog::getOpenFileName(0,tr("load"),QApplication::applicationDirPath(),tr("*.*"));
        QFile file(fileName);
        if (!file.open(QIODevice::ReadOnly)){
        }
        else{
            QString line = file.readAll();
            _textEditInput->setText(line);
        }
    }
    void slotOnPushDialogSave(){
        QString fileName = QFileDialog::getSaveFileName(0,tr("save"),QApplication::applicationDirPath(),tr("*.*"));
        if(fileName!=""){
            QFile file(fileName);
            if (!file.open(QIODevice::WriteOnly)){
            }
            else{
                QTextStream stream (&file);
                stream << _textEditInput -> toPlainText();
                file.close();
            }
        }
    }
};

#endif // CLIENTWIDGETS

