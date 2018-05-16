#ifndef COLOR_LED_H
#define COLOR_LED_H

#include <QWidget>
#include <QVariantHash>

#ifndef LED_STATE_
#define LED_STATE_
typedef enum {
    LED_CLOSE=0,/**< closed (unavailable) */
    LED_OPEN,/**< open (default color) */
    LED_OK,/**< normal */
    LED_FAULT,/**< failure */
    LED_STATE_NUM
}LED_STATE;
#endif

/** color led UI */
class ColorLED : public QWidget
{
    Q_OBJECT
private:
    QVariantHash attr;/**< UI attributes */
    bool available;/**< flag of availibity */
    int value;/**< led value */

public:
    explicit ColorLED(QWidget *parent = 0);

    //interface
    void build(QVariantHash &a);/**< build UI with attributes */
    void disable();/**< set UI unavailable */
    void setValue(int v);/**< set value (int) */

protected:
    virtual void paintEvent(QPaintEvent * event);
};

#endif // COLOR_LED_H
