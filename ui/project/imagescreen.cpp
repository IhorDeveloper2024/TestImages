#include "rawimagedata.h"
#include "imagescreen.h"
#include "datacontroller.h"
#include "delegate.h"
#include "fileslist.h"
#include <QPainter>
#include <QPushButton>
#include <QHBoxLayout>
#include <QListWidget>
#include <QKeyEvent>
#include <QLabel>
#include <QDialog>
#include <QMessageBox>

namespace {
class Label : public QLabel {
    QMetaObject::Connection m_connection;
public:
    Label(QWidget *parent = nullptr) : QLabel(parent) {
        m_connection = connect(DataController::Instance() , &DataController::pathChanged,
                [this](){ setText(QString("Path: %1").arg(DataController::Instance()->getPath())); });
        setText(QString("Path: %1").arg(DataController::Instance()->getPath()));
    }
    ~Label() {
        disconnect(m_connection);
    }
};

class Widget : public QWidget {
    QMetaObject::Connection m_connection;
public:
    Widget(QWidget* parent = nullptr) : QWidget(parent) {
        show();
        setMinimumSize(800, 600);
        setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Expanding);
        m_connection = connect(DataController::Instance(), &DataController::indexChanged, [this](){ update(); });
    }
    ~Widget() {
        disconnect(m_connection);
    }

    virtual void paintEvent(QPaintEvent *event) override {
        QWidget::paintEvent(event);
        QPainter painter(this);
        auto image = DataController::Instance()->getImage();
        static QImage emptyImage(100, 100, QImage::Format_Grayscale8);
        emptyImage.fill(Qt::white);
        if (image.isNull() || !image->isValid()) {
            painter.drawImage(QRectF{{0,0},size()}, emptyImage.scaled(size(), Qt::KeepAspectRatio));
            return ;
        }
        double ratio = (double)image->rowData().width / image->rowData().height;
        QImage picture({image->rowData().data, image->rowData().width,
                            image->rowData().height - 5, QImage::Format_Grayscale8});
        QSize pic_size{static_cast<int>(size().height() * ratio), size().height()};
        QRectF pic_rect{{0,0}, pic_size};
        pic_rect.moveCenter(QPointF{qreal(size().width()) / 2, qreal(size().height()) / 2});
        picture = picture.scaled(pic_size, Qt::KeepAspectRatio);
        painter.drawImage(pic_rect, picture);
    }
};

class ListWidget : public QListView {
public:
    ListWidget(QWidget *parent = nullptr) : QListView(parent) {
        setModel(DataController::Instance()->getFilesList());
        setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Expanding);

        auto *delegate = new Delegate(this);
        initPalette();

        setItemDelegate(delegate);
    }

    void keyPressEvent(QKeyEvent *event) override {
        QListView::keyPressEvent(event);
        if (event->key() == Qt::Key_Enter || event->key() == Qt::Key_Return) {
            DataController::Instance()->navigate(this->currentIndex().row());
        } else {
            DataController::Instance()->selectImage(this->currentIndex().row());
        }
    }

    void mousePressEvent(QMouseEvent *event) override{
        QListView::mousePressEvent(event);
        DataController::Instance()->selectImage(this->currentIndex().row());
    }
private:
    void initPalette() {
        QPalette p(palette());

        setFont(QFont("MSShellDlg 2", 12));
        p.setBrush(QPalette::WindowText, {"#303030"});
        p.setBrush(QPalette::Base, {"#F0F1F2"});
        p.setBrush(QPalette::Light, {"#FFFFFF"});
        p.setBrush(QPalette::Midlight, {"#D3D6D8"});
        p.setBrush(QPalette::Mid, {"#C5C9Cb"});
        p.setBrush(QPalette::Dark, {"#9AA0A4"});
        p.setBrush(QPalette::Text, {"#616b71"});
        p.setBrush(QPalette::Highlight, {"#E2E4E5"});

        setPalette(p);
    }
};

struct ReturnButton : public QPushButton {
    explicit ReturnButton(QWidget *parent = nullptr)
        : QPushButton(parent) {
        connect(this, &QPushButton::clicked, this, [](){
            DataController::Instance()->select("previous");
        });
        setText("PREVIOUS SCREEN");
        show();
    }
};

struct SaveButton : public QPushButton {
    explicit SaveButton(QWidget *parent = nullptr)
        : QPushButton(parent) {
        connect(this, &QPushButton::clicked, this, []() {
            auto result = DataController::Instance()->save();
            const auto model = DataController::Instance()->getFilesList();
            const auto index = model->index(DataController::Instance()->getCurrentIndex());
            auto is_dir = index.data(FilesList::IsDirRole).toBool();
            if (is_dir) {
                QMessageBox dialog;
                dialog.setWindowTitle("Impossible to save");
                dialog.setStandardButtons(QMessageBox::Ok);
                dialog.setText("This is a folder");
                dialog.exec();
            } else {
                if (!result.isEmpty()) {
                    QMessageBox dialog;
                    dialog.setWindowTitle("Incorrect file");
                    dialog.setStandardButtons(QMessageBox::Ok);
                    dialog.setText(result);
                    dialog.exec();
                }
            }
        });
        setText("SAVE FILE");
        show();
    }
};
}

ImageScreen::ImageScreen(QWidget *parent)
    : QWidget{parent} {
    auto h_layout = new QHBoxLayout();
    h_layout->addWidget(new ListWidget(this));
    h_layout->addWidget(new Widget(this));
    auto v_layout = new QVBoxLayout();
    auto hlow_layout = new QHBoxLayout();
    hlow_layout->addWidget(new SaveButton(this));
    hlow_layout->addItem(new QSpacerItem(10,10, QSizePolicy::Expanding));
    hlow_layout->addWidget(new ReturnButton(this));
    v_layout->addWidget(new Label(this));
    v_layout->addLayout(h_layout);
    v_layout->addLayout(hlow_layout);
    setLayout(v_layout);
}
