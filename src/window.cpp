// Copyright (C) 2016 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR BSD-3-Clause

//#include "ui_test_widget.h"
#include "widget.h"
#include "window.h"
#include "test_widget.h"
#include "scene.h"

#include <QGridLayout>
#include <QLabel>
#include <QTimer>
//#include <QtWidgets>


//! [0]
Window::Window()
{
    setWindowTitle(tr("蚁群算法演示v1.0"));

    setGeometry(0,0,430,500);

    //Widget *native = new Widget(&helper, this);


    Scene *sc = new Scene(&helper,this);

    QTimer *timer = new QTimer(this);
    connect(timer, &QTimer::timeout, sc, &Scene::render_update);
    timer->start(10);



    //QGraphicsScene *tmp = new QGraphicsScene();
    //QGraphicsProxyWidget *proxy = tmp->addWidget(sc,Qt::WindowType::Widget);

    QGridLayout *layout = new QGridLayout;
    layout->addWidget(sc->view,0,0);
    setLayout(layout);

    test_widget * tw = new test_widget(this);
    tw->setFixedSize(116,379);

    sc->setFriUi(tw);

    connect(tw,&test_widget::button_clicked,sc,&Scene::start_algo);
    connect(tw,&test_widget::button_2_clicked,sc,&Scene::change_mode);

    //sc->view->show();

    //sc->view->show();
    /*QLabel *nativeLabel = new QLabel(tr("Native"));
    nativeLabel->setAlignment(Qt::AlignHCenter);
    QGridLayout *layout = new QGridLayout;
    //layout->addWidget(native, 0, 2);

    //layout->addWidget(nativeLabel, 1, 2);
    //setLayout(layout);
    native->move(119,0);
    */
}

Window::~Window(){


}
//! [0]
