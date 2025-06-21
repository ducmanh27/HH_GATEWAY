#ifndef COLLECTORSERVICE_H
#define COLLECTORSERVICE_H

#include <QObject>

class CollectorService : public QObject
{
    Q_OBJECT
public:
    explicit CollectorService(QObject *parent = nullptr);

signals:
};

#endif // COLLECTORSERVICE_H
