#ifndef QMLBACKENDFACTORY_H
#define QMLBACKENDFACTORY_H

#include <iostream>
#include <QObject>
#include <QQmlEngine>

#include "backend.h"

/// Mediator between  Backend and QML definition of GUI. Takes care about Backend class instances.
class QmlBackendFactory : public QObject 
{
    Q_OBJECT
    Q_PROPERTY(QObject* getBackend READ getBackend)
public:
    QmlBackendFactory(QObject *parent = 0);
    ~QmlBackendFactory();

    QObject* getBackend() const;
private:
	static int factoryInstanceCounter;


};

#endif
