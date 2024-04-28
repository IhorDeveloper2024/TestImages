#include "delegate.h"
#include "fileslist.h"
#include <QPainter>
#include <qglobal.h>
#include <QSize>
#include <QMargins>

struct Delegate::Impl {
    Impl()
        : iconSize(16, 16)
        , margins(0, 0, 0, 0)
        , spacingHorizontal(0)
        , spacingVertical(0) {
    }

    QRect timestampBox(QStyleOptionViewItem &option,
                                        const QModelIndex &index) const {
        option.font.setPointSizeF(0.85 * option.font.pointSize());
        return QFontMetrics(option.font).boundingRect(index.data(Qt::UserRole).toString()).adjusted(0, 0, 1, 1);
    }

    QRect messageBox(const QStyleOptionViewItem &option) const {
        return option.fontMetrics.boundingRect(option.text).adjusted(0, 0, 1, 1);
    }

    QSize iconSize;
    QMargins margins;
    int spacingHorizontal;
    int spacingVertical;
};


Delegate::Delegate(QObject *parent) :
    QStyledItemDelegate(parent),
    m_impl(new Impl()) {
    const auto margin = 8;
    const auto width = 32;
    setContentsMargins(margin, margin, margin, margin);
    setIconSize(width, width);
    setHorizontalSpacing(margin);
    setVerticalSpacing(margin / 2);
}

QSize Delegate::iconSize() const { return m_impl->iconSize; }

void Delegate::setIconSize(int width, int height) {
    m_impl->iconSize = QSize(width, height);
}

QMargins Delegate::contentsMargins() const { return m_impl->margins; }

void Delegate::setContentsMargins(int left, int top, int right, int bottom) {
    m_impl->margins = QMargins(left, top, right, bottom);
}

int Delegate::horizontalSpacing() const { return m_impl->spacingHorizontal; }

void Delegate::setHorizontalSpacing(int spacing) {
    m_impl->spacingHorizontal = spacing;
}

int Delegate::verticalSpacing() const { return m_impl->spacingVertical; }

void Delegate::setVerticalSpacing(int spacing) {
    m_impl->spacingVertical = spacing;
}

void Delegate::paint(QPainter *painter, const QStyleOptionViewItem &option,
    const QModelIndex &index) const {
    QStyleOptionViewItem opt(option);
    initStyleOption(&opt, index);

    const QPalette &palette(opt.palette);
    const QRect &rect(opt.rect);
    const QRect &contentRect(rect.adjusted(
        m_impl->margins.left(), m_impl->margins.top(),
        -m_impl->margins.right(), -m_impl->margins.bottom()));
    const bool lastIndex = (index.model()->rowCount() - 1) == index.row();
    const int bottomEdge = rect.bottom();
    QFont f(opt.font);

    f.setPointSize(0.85 * option.font.pointSize());

    painter->save();
    painter->setClipping(true);
    painter->setClipRect(rect);
    painter->setFont(opt.font);

    painter->fillRect(
        rect, opt.state & QStyle::State_Selected ?
        palette.highlight().color() : palette.light().color());

    painter->setPen(lastIndex ? palette.dark().color() : palette.mid().color());
    painter->drawLine(lastIndex ? rect.left() : m_impl->margins.left(), bottomEdge, rect.right(), bottomEdge);
    auto isDir = index.data(FilesList::IsDirRole).toBool();
    auto isValid = index.data(FilesList::IsValidRole).toBool();
    QString type = isDir ? "folder" : (isValid ? "icon" : "no_file");
    QIcon pixm(QString(":/imageData/project/icons/%1.png").arg(type));
    painter->drawPixmap(contentRect.left(), contentRect.top(), pixm.pixmap(m_impl->iconSize));

    QRect messageRect(contentRect);
    const auto delta = 15;
    messageRect.moveLeft(m_impl->iconSize.width() + delta);
    messageRect.moveCenter({messageRect.center().x(), contentRect.center().y()});

    painter->setFont(opt.font);
    painter->setPen(Qt::black);
    painter->drawText(messageRect, Qt::TextSingleLine, index.data(FilesList::NameRole).toString());
    painter->restore();
}

QSize Delegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const {
    QStyleOptionViewItem opt(option);
    initStyleOption(&opt, index);
    int textHeight = m_impl->timestampBox(opt, index).height()
                     + m_impl->spacingVertical + m_impl->messageBox(opt).height();
    int iconHeight = m_impl->iconSize.height();
    int h = textHeight > iconHeight ? textHeight : iconHeight;
    return QSize(opt.rect.width(), m_impl->margins.top() + h + m_impl->margins.bottom());
}
