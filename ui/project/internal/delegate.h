#pragma once

#include <QStyledItemDelegate>

class Delegate final : public QStyledItemDelegate {
public:
    explicit Delegate(QObject *parent = nullptr);
    ~Delegate() = default;

    QSize iconSize() const;
    void setIconSize(int width, int height);

    QMargins contentsMargins() const;
    void setContentsMargins(int left, int top, int right, int bottom);

    int horizontalSpacing() const;
    void setHorizontalSpacing(int spacing);

    int verticalSpacing() const;
    void setVerticalSpacing(int spacing);

    void paint(QPainter *painter, const QStyleOptionViewItem &option,
               const QModelIndex &index) const override;
    QSize sizeHint(const QStyleOptionViewItem &option,
                   const QModelIndex &index) const override;

private:
    struct Impl;
    QSharedPointer<Impl> m_impl;
};
