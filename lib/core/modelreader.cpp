#include "modelreader.h"
#include <QSettings>

using namespace media;

void ModelReader::readFile(const QString& aFileName)
    {
    QSettings modelFile(aFileName, QSettings::IniFormat);
    for (int i = 0;; ++i)
        {
        QString pluginName = modelFile.value(QString("nodes/%1").arg(i)).toString();
        if (pluginName.isEmpty())
            break;
        //else
        emit createElement(i, pluginName);
        }

    for (int i = 0;; ++i)
        {
        QString connectionPair = modelFile.value(QString("edges/%1").arg(i)).toString();
        if (connectionPair.isEmpty())
            break;

        QStringList connectionList = connectionPair.split(" ");
        emit connectElements(connectionList.front().toInt(), connectionList.back().toInt());
        }
    }
