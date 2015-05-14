#include <QAbstractItemModel>
#include <QListView>
#include <QApplication>
#include <QQmlApplicationEngine>
#include <QtQml>

#include "lmsaudiomodel.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    QQmlApplicationEngine engine;

    LMSAudioModel *tracksModel = new LMSAudioModel(NULL);

    tracksModel->setQueryType(LMSAudioModel::Tracks);

    engine.rootContext()->setContextProperty("LMSTracksList", tracksModel);

    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));

    return app.exec();
}
