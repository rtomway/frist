#ifndef _EVENTFILTEROBJECT_H_
#define _EVENTFILTEROBJECT_H_


#include<QObject>
#include<QPoint>


class EventFilterObject :public QObject
{
public:
	EventFilterObject(QObject* parent = nullptr);
protected:
	bool eventFilter(QObject* watch, QEvent* ev)override;
private:
	QPoint m_presspos;
};

#endif // !_EVENTFILTEROBJECT_H_
