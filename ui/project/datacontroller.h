#pragma once

#include <QObject>
#include <QImage>
#include <QPixmap>
#include <QtQml>
#include "filemanager.h"

class QWidget;
class ImageDataPtr;
class DataController : public QObject {
    Q_OBJECT
    QML_SINGLETON
    Q_PROPERTY(QString sourceComponent READ getSourceComponent WRITE setSourceComponent NOTIFY sourceChanged)
    Q_PROPERTY(bool isVisible READ getIsVisible WRITE setIsVisible NOTIFY isVisibleChanged)
    Q_PROPERTY(QAbstractListModel* filesList READ getFilesList /*WRITE setFilesList*/ NOTIFY filesListChanged)
    Q_PROPERTY(QString path READ getPath NOTIFY pathChanged)
public:
    static DataController* Instance();

    Q_INVOKABLE void select(const QString &screen);
    Q_INVOKABLE void navigate(int index);
    Q_INVOKABLE void selectImage(int index);
    Q_INVOKABLE QString save();

    QAbstractListModel* getFilesList() { return m_fileslist; }

    void updateDirectoryPath(const QString &path);
    void setDefaultImageFolderPath(const QString &path);

    int getCurrentIndex() const { return m_currentIndex; }
    QString getPath() const;
    QSharedPointer<ImageDataPtr> getImage() const;
private:
    QString getSourceComponent() const { return m_sourceComponent; }
    void setSourceComponent(const QString &source);

    bool getIsVisible() const { return m_isVisible;}
    void setIsVisible(bool visible);

Q_SIGNALS:
    void textChanged();
    void sourceChanged();
    void imageChanged();
    void isVisibleChanged();
    void filesListChanged();
    void pathChanged();
    void indexChanged();

private:
    explicit DataController(QObject *parent = nullptr);
    Q_DISABLE_COPY_MOVE(DataController);

private:
    QString m_sourceComponent = "startScreen";
    FileManager m_filemanager;
    QAbstractListModel *m_fileslist;
    bool m_isVisible = true;
    QPointer<QWidget> m_widget;
    int m_currentIndex = -1;
};
