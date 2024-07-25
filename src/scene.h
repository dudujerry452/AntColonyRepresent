#ifndef SCENE_H
#define SCENE_H

#include <QWidget>
#include <QApplication>

#include <QGraphicsScene>

#include <QGraphicsView>

#include <QGraphicsRectItem>
#include "test_widget.h"

#include "math.h"

class Helper;

class Scene : public QGraphicsScene
{
    Q_OBJECT
public:
    Scene(Helper* helper,QWidget* pwindow);
    ~Scene();
    void setFriUi(test_widget* tw){friui = tw;};
    QPointF getSceneCursor();

    void clear();


public:
    QGraphicsView* view;
    QWidget *pwindow;

    test_widget* friui;

private:
    Helper* helper;
    bool clicked = false;
    bool moved = false;
    QPointF lastcursorpos = QPointF(0,0);
    QGraphicsSimpleTextItem *simpleTextItem;
    QPointF nowPos = QPointF(0,0);
    float scale = 1.0F;
    int pointcount = 0;
    int pathcount = 0;

    bool algo_started = false;
    bool cntx = false;

    qreal ptpixel = 40;

private: //animate reltative
    int progress = 0;
    int *s;//当前所在
    int *r;//当前要去
    qreal opacity = 1.f;

    bool mode = false;
    int speed = 1;

    bool pPathFlag = false;

    QPointF* ant_dif = nullptr;
    QGraphicsPathItem* pPath;

    QGraphicsPathItem** *ppPath;
    bool** created;

    QPixmap* antp;

private:
    QGraphicsItem** p2p;
    QGraphicsItem** p2p_a;


protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;
    void wheelEvent(QGraphicsSceneWheelEvent *event) override;

public slots:
    void render_update();
    void start_algo();
    void change_mode();
};

#endif // SCENE_H
