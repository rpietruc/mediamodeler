#ifndef MEDIA_MODELREADER_H
#define MEDIA_MODELREADER_H

#include <QObject>
#include <QString>

namespace media
{

class ModelReader : public QObject
    {
    Q_OBJECT

public:
    explicit ModelReader(QObject *aParent = 0) : QObject(aParent) {}
    void readFile(const QString& aFileName);

signals:
    void createElement(int aIndex, const QString& aPluginName);
    void connectElements(int aSourceIndex, int aDestinationIndex);
    };

} // namespace media

#endif // MEDIA_MODELREADER_H
