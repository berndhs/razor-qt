/* BEGIN_COMMON_COPYRIGHT_HEADER
 *
 * Razor - a lightweight, Qt based, desktop toolset
 * http://razor-qt.org
 *
 * Copyright: 2010-2011 Razor team
 * Authors:
 *   Alexander Sokoloff <sokoloff.a@gmail.ru>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation;  only version 2 of
 * the License is valid for this program.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
 *
 * END_COMMON_COPYRIGHT_HEADER */


#ifndef RAZORPANELLAYOUT_H
#define RAZORPANELLAYOUT_H

#include <QtGui/QBoxLayout>
#include <QtCore/QList>
#include <QtGui/QWidget>

class MoveInfo;
class QMouseEvent;
class QEvent;

class RazorPanelLayout : public QBoxLayout
{
    Q_OBJECT
    friend class MoveProcessor;

public:
    RazorPanelLayout(Direction dir, QWidget* parent=0);
    virtual ~RazorPanelLayout();

    void startMoveWidget(QWidget* widget);

    QSize sizeHint() const;

signals:
    void widgetMoved(QWidget* movedWidget);

};

class MoveProcItem;
class MoveProcessor: public QWidget
{
    Q_OBJECT

public:
    explicit MoveProcessor(RazorPanelLayout* layout, QWidget* movedWidget);
    ~MoveProcessor();

signals:
    void widgetMoved(QWidget* movedWidget);

protected:
    bool event(QEvent* event );
    void apply();

private:
    void mouseMoveHoriz();
    void mouseMoveVert();

    QWidget* mWidget;
    QList<MoveProcItem*> mItems;
    RazorPanelLayout* mLayout;
    bool mHoriz;
    int mIndex;
    QPoint mOffset;
    QRect mWidgetPlace;

private slots:
    void finished();
};

#endif // RAZORPANELLAYOUT_H
