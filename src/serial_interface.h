#ifndef SERIAL_INTERFACE_H
#define SERIAL_INTERFACE_H

#include <QObject>

class SerialInterface : public QObject
{
    Q_OBJECT
public:
    explicit SerialInterface(QObject *parent = nullptr);

signals:

public slots:
};

#endif // SERIAL_INTERFACE_H