#ifndef DATA_STRUCTURE_H
#define DATA_STRUCTURE_H

#include <QSharedPointer>
#include <QString>
#include <QByteArray>
#include <QDateTime>

#define CONTROL_COMMAND_LEN 10
#define RECEIVE_MESSAGE_LEN 14

/** configuration of serial port */
class SerialConf {
public:
    QString name;/**< name of serial port */
    int baudrate;/**< communication baudrate */
    int checkBit;/**< check bit setting */
    int dataBit;/**< data bit number */
    int stopBit;/**< stop bit number */

    /** constructor with name setting */
    explicit SerialConf(QString n = QString()):name(n) {}

    /** copy setting from another instance */
    void copyFrom(const SerialConf &other) {
        //copy every field
        name = other.name;
        baudrate = other.baudrate; checkBit = other.checkBit;
        dataBit = other.dataBit; stopBit = other.stopBit;
    }
};

/** shared pointer of serial port configuration */
typedef QSharedPointer<SerialConf> SerialConf_ptr;

/** control command of weighting equipment */
class ControlCommand {
private:
    unsigned char nodeIndex;/**< node index the command is sending to */
    unsigned char funcCode;/**< functionality code */
    qint32 weight;/**< calibration weight, unit: 0.01g */

public:
    /** empty constructor */
    ControlCommand():nodeIndex(0),funcCode(0),weight(0) {}
    /**
     * constructor with settings of all fields
     * @param node_index --- node index (0-254)
     * @param func_code --- functionality code
     * @param weight_g --- calibration weight, unit: 0.01g
     */
    ControlCommand(unsigned char node_index, unsigned char func_code, qint32 weight_g = 0):
        nodeIndex(node_index),funcCode(func_code),weight(weight_g) {}

    /**
     * @brief set node index
     * @param value --- node index
     */
    void setNodeIndex(unsigned char value) {
        nodeIndex = value;
    }
    /**
     * @brief set functionality code
     * @param value --- functionality code
     */
    void setFuncCode(unsigned char value) {
        funcCode = value;
    }
    /**
     * @brief set calibration weight
     * @param value --- calibration weight, unit: 0.01g
     */
    void setWeight(qint32 value) {
        weight = value;
    }

    /**
     * @brief serialize control command into byte arrays
     * @param data --- pointer of byte array
     */
    void serializeToByteArray(unsigned char *data) const {
        data[0] = 0xa5;//head
        data[1] = nodeIndex;//node index
        data[2] = 0x5;//data length
        data[3] = funcCode;//functionality code
        quint32 ui_weight = (quint32)weight;//transfer weight into unsigned number
        for(int i=0;i<4;i++) {
            data[7-i] = (unsigned char)(ui_weight&0xff);//higher byte in front
            ui_weight >>= 8;
        }
        data[8] = 0xff; data[9] = 0xff;//tail
    }
};

/** smart pointer of control command */
typedef QSharedPointer<ControlCommand> ControlCommand_ptr;

/** received message */
class ReceivedMessage {
private:
    qint64 ts;/**< timestamp when message received */
    unsigned char nodeIndex;/**< node index which sent the message */
    unsigned char funcCode;/**< functionality code */
    qint32 adValue;/**< AD value */
    double weight;/**< weight value, unit: g */

public:
    /** empty constructor */
    ReceivedMessage():ts(-1) {}

    /**
     * @brief parse message from byte array
     * @param data --- pointer of byte array
     */
    void parseFromByteArray(unsigned char *data) {
        ts = QDateTime::currentMSecsSinceEpoch();//timestamp
        nodeIndex = data[1];//node index
        funcCode = data[3];//functionality code
        //parse ad value
        quint32 tmp = 0;
        for(int i=0;i<4;i++) {
            tmp = (tmp<<8) + data[4+i];//higher byte in front
        }
        adValue = (qint32)tmp;//ad value
        //parse weight
        tmp = 0;
        for(int i=0;i<4;i++) {
            tmp = (tmp<<8) + data[8+i];//higher byte in front
        }
        weight = ((qint32)tmp)*0.01;//weight value
    }

    /** get timestamp, unit: millisecond since epoch */
    qint64 getTimestamp_epoch_ms() const {
        return ts;
    }
    /** get node index */
    unsigned char getNodeIndex() const {
        return nodeIndex;
    }
    /** get functionality code */
    unsigned char getFuncCode() const {
        return funcCode;
    }
    /** get AD value */
    qint32 getAdValue() const {
        return adValue;
    }
    /** get weight value, unit: g */
    double getWeight_g() const {
        return weight;
    }
};

/** smart pointer of received message */
typedef QSharedPointer<ReceivedMessage> ReceivedMessage_ptr;

#endif // DATA_STRUCTURE_H
