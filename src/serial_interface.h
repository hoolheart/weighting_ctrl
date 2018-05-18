#ifndef SERIAL_INTERFACE_H
#define SERIAL_INTERFACE_H

#include <QObject>
#include <QScopedPointer>
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QList>
#include <QTimer>

#include "data_structure.h"

/** serial port communication interface */
class SerialInterface : public QObject
{
    Q_OBJECT
public:
    explicit SerialInterface(QObject *parent = nullptr);
    ~SerialInterface();

    /**
     * @brief get the last error information
     * @return last error information
     */
    QString lastError() const;

    /**
     * @brief configure serial port
     * @param conf --- configuration
     * @return whether the configuration succeed
     */
    bool configure(const SerialConf_ptr conf);
    /**
     * @brief get current configuration
     * @return current configuration pointer
     */
    const SerialConf_ptr getConfiguration() const;

    /**
     * @brief open the serial port
     * @return whether the serial port is successfully open
     */
    bool open();
    /**
     * @brief close the serial port
     * @return whether the serial port is successfully closed
     */
    bool close();
    /**
     * @brief get the status of serial port
     * @return whether the serial port is open
     */
    bool isOpen() const;

    /**
     * @brief send a control command
     * @param cmd --- command pointer
     * @return whether the command is successfully sent
     */
    bool sendCommand(const ControlCommand_ptr cmd);
    /** whether there are pending messages */
    bool hasPendingMessage() const;
    /**
     * @brief take a pending message
     * @return pending message pointer, if there is no pending message, return a null pointer
     */
    ReceivedMessage_ptr takeMessage();
    /** clear all pending messages */
    void clearPendingMessage();

    /**
     * @brief interprete parity setting to string
     * @param parity --- parity setting
     * @return readable string
     */
    static QString interpreteParity(int parity);
    /**
     * @brief interprete stop bit setting to string
     * @param stop_bit --- stop bit setting
     * @return readable string
     */
    static QString interpreteStopBit(int stop_bit);

signals:
    /** signal emitted when a new message received */
    void messageReceived();
    /** signal emitted when an error occurs in serial port */
    void serialPortError(QString err);

private slots:
    void onDataAvailable();//process new received data from serial port
    void onDataWritten(qint64 len);//process data written event
    void onPortError(QSerialPort::SerialPortError port_err);//process serial port error
    void checkStatus();//check serial port status periodically

private:
    bool checkConfiguration(const SerialConf_ptr &conf, QString &err);//check configuration availability
    QString interpretePortError(QSerialPort::SerialPortError port_err);//interprete serial port error into string

private:
    SerialConf_ptr conf;/**< configuration */
    QScopedPointer<QSerialPort> port;/**< serial port pointer */
    //QScopedPointer<QSerialPortInfo> info;/**< serial port information pointer */
    QList<ReceivedMessage_ptr> messages;/**< pending message list */

    unsigned char *txBuf, *rxBuf;/**< buffers */
    int rxIndex;/**< current index of received buffer */

    QString err;/**< last error information */
    qint64 bytesToWrite;/**< count of bytes to write */
    QTimer chkTimer;/**< check timer */
};

#endif // SERIAL_INTERFACE_H
