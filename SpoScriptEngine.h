#ifndef SCRIPT_H
#define SCRIPT_H
#include <QScriptEngine>
#include <QJSEngine>
#include <QJSValue>
#include <QObject>
#include <QString>
#include <QJsonObject>
#include <QJsonDocument>
#include <cmath>

#define PI 3.14159265

class Obj1:public QObject{
    Q_OBJECT
public slots:
    float sum(float a, float b) {return a+b;}
    float mul(float a, float b) {return a*b;}
};

class Obj2:public QObject{
    Q_OBJECT
public slots:

    float sub(float a, float b) {return a-b;}
    float div(float a, float b) {return a/b;}

};

class Obj3:public QObject{
    Q_OBJECT
public slots:

    float power(float a, float b) {return pow(a,b);}
    float sinus(float a) {return sin(a * PI /180);}
    float cosinus(float a) {return cos(a * PI /180);}
    float tangens(float a) {return tan(a * PI /180);}
    float cotangens(float a) {return cos(a * PI /180)/sin(a * PI /180);}
    float exponenta(float a) {return exp(a);}
    float ln(float a) {return log(a);}
    float logarifm(float a, float b)
    {
        return ln(b) / ln(a);
    }
};

class SpoJSon : public QObject
{
    Q_OBJECT

public:
    SpoJSon(){}
    void addObject(const QString name, QObject *obj){
        QJSValue _newEngineObject = _engine.newQObject(obj);
        _engine.globalObject().setProperty(name,_newEngineObject);
    }
    QString parseMsg(const QString &msg) {
         QJSValue result = _engine.evaluate(msg);
         QString res = result.toString();
         if(result.isError()) {
            QJsonObject jobj;
            jobj.insert("error", "script:"+res);
            jobj.insert("src", msg);
            QJsonDocument doc(jobj);
            return doc.toJson();
         }
         else {
             return res;
         }
    }
private:
    QJSEngine _engine;
};

#endif // SCRIPT_H

