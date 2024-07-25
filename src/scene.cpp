#include "scene.h"
#include "helper.h"
#include <QGraphicsSceneEvent>
#include <string>
#include <iostream>
#include "algo.h"
#include "ui_test_widget.h"
#include <QResource>
#include "safe_delete.h"

extern algo antalgo;

Scene::Scene(Helper* helper,QWidget* pwindow)
    :helper(helper),
    pwindow(pwindow)
{
    view = new QGraphicsView(this);
    view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    view->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    //view->adjust
    //view->setResizeAnchor(QGraphicsView::NoAnchor);
    view->resize(1000,800);
    this->setSceneRect(0,0,5000,3100);
    //QGraphicsPathItem *m_pathItem = new QGraphicsPathItem();
    //helper->paint(m_pathItem,1);
    //this->addItem(m_pathItem);

    antp = new QPixmap(":/images/ant.png");
    printf("isnull:%d",antp->isNull());

    //friui->ui->lineEdit_4->setText("1");
    //friui->ui->lineEdit_3->setText("6");


}
Scene::~Scene() {

    safe_delete::SafeDeleteArray(view);

    safe_delete::SafeDeleteArray(p2p);
    safe_delete::SafeDeleteArray(p2p_a);

    safe_delete::SafeDeleteNDimensionalArray(ppPath,pointcount);
    safe_delete::SafeDeleteNDimensionalArray(created,pointcount);

    safe_delete::SafeDeleteArray(ant_dif);

    safe_delete::SafeDeleteArray(antp);
}

void Scene::clear(){

}

void Scene::change_mode(){
    cntx ^=1;
    if(cntx) return;
    mode ^= 1;
    if(mode == 0)
        friui->ui->pushButton_2->setText("自动");
    else
        friui->ui->pushButton_2->setText("手动");
    //printf("mode=%d\n",mode);
}

QPointF Scene::getSceneCursor(){
    // 获取鼠标在全局屏幕坐标系下的位置
    QPoint globalPos = QCursor::pos();
    //// 获取鼠标在视图坐标系下的位置
    QPoint viewPos = view->mapFromGlobal(globalPos);
    //// 获取鼠标在场景坐标系下的位置
    QPointF scenePos = view->mapToScene(viewPos);
    return scenePos;
}

void Scene::mousePressEvent(QGraphicsSceneMouseEvent *event){
    if(event->button() == Qt::LeftButton){
        clicked = true;
        moved = false;
    }
}

void Scene::mouseReleaseEvent(QGraphicsSceneMouseEvent *event){
    if(event->button() == Qt::LeftButton && moved == false && !algo_started){
        QGraphicsEllipseItem * qi= addEllipse(0,0,ptpixel,ptpixel);
        qi->setPos((int)event->scenePos().x()-ptpixel/2,(int)event->scenePos().y()-ptpixel/2);
        qi->setZValue(pointcount++);
        friui->ui->lineEdit->setText(QString::number(pointcount));
        friui->ui->lineEdit_2->setText(QString::number(pointcount));
    }

    moved = false;
    clicked = false;
}
void Scene::wheelEvent(QGraphicsSceneWheelEvent *event) {
    scale = 1.0f + (float)(event->delta())/10000.0f;
    view->scale(scale,scale);
}
void Scene::render_update(){

    int w = pwindow->width();
    int h = pwindow->height();

    view->resize(w,h);
    this->update();

    QPointF cur = getSceneCursor();
    QPointF dif = QPointF(0,0);
    if(clicked && cur != lastcursorpos){
        moved = true;
        dif = lastcursorpos- cur;
    }
    nowPos = nowPos + dif;
    view->setSceneRect(nowPos.x(),nowPos.y(),w,h);
    view->setTransformationAnchor(QGraphicsView::AnchorViewCenter);

    std::string s = std::to_string(lastcursorpos.x()) + " " + std::to_string(lastcursorpos.y());

    /*if(simpleTextItem){
        this->removeItem(simpleTextItem);
        delete simpleTextItem;
        //simpleTextItem = nullptr;
    }

    simpleTextItem = this->addSimpleText(s.c_str(), QFont("Arial", 20));*/
    /*std::cout<<"last="<<s<<",";
    s = std::to_string(cur.x()) + " " + std::to_string(cur.y());
    std::cout<<"cur="<<s<<",";
    s = std::to_string(dif.x()) + " " + std::to_string(dif.y());
    std::cout<<"dif="<<s<<std::endl;*/
    //printf("progress=%d,mode=%d\n",progress,mode);
    if(progress < 100){
        progress += speed;
        if(progress > 100) progress = 100;

        for(int i = 0;i < antalgo.m ; i++){
            int r = antalgo.tour[i].r(antalgo.tour[i].size()-1);
            p2p_a[i]->setPos(p2p[r]->scenePos() + progress*ant_dif[i]/100);
        }
        //pPath->setOpacity(1-(progress/100.f));
    }
    else  if(mode){
        printf("enter\n");
        int tmp = cntx;
        cntx = 1;
        start_algo();
        cntx = tmp;
    }

    if(pPath && pPathFlag)
        pPath->setOpacity(opacity);
    if(opacity > 0.1f) opacity -= 0.005f;

    lastcursorpos = cur;


    //绘制蚂蚁

}
void Scene::start_algo(){
    cntx ^=1;
    if(cntx) return;
    if(!algo_started){
        p2p = new QGraphicsItem*[pointcount+5];

        QList<QGraphicsItem*> lst = this->items();
        QListIterator ii(lst);
        ii.toBack();

        char* str = new char[200];
        std::string strr;
        //std::cout<<"11";
        while(ii.hasPrevious()){
            QGraphicsItem* it = ii.previous();
            int num = qRound((*it).zValue());
            printf("num=%d\n",num);
            if(num < 0 || num >= pointcount) continue;
            memset(str,-1,200);
            //std::cout<<num<<std::endl;
            qreal x = (*it).scenePos().x();
            qreal y = (*it).scenePos().y();
            double x_f = (QString::number(x,'f',2)).toDouble();
            double y_f = (QString::number(y,'f',2)).toDouble();

            //printf("%lf %lf\n",x_f,y_f);
            sprintf(str,"%d %lf %lf|", num, x_f, y_f);
            std::string stri(str);
            strr += stri;

            p2p[num] = it;

            friui->ui->pushButton->setText("进行下一步");
        }
        delete[] str;
        //std::cout<<strr;

        antalgo.delta = friui->ui->lineEdit_4->text().toInt();
        antalgo.beta = friui->ui->lineEdit_3->text().toInt();

        antalgo.N = pointcount;
        antalgo.io(strr.c_str());
        antalgo.init();
        antalgo.reset();

        p2p_a = new QGraphicsItem*[antalgo.m+5];

        for(int i = 0 ; i < antalgo.m; i ++){
            p2p_a[i] = this->addRect(0,0,20,20);
            p2p_a[i]->setPos(p2p[antalgo.r1[i]]->scenePos().x(),p2p[antalgo.r1[i]]->scenePos().y());
            p2p_a[i]->setZValue(pointcount+i);
            //printf("set %d ant,r1's z=%lf\n",i,p2p[antalgo.r1[i]]->zValue());
        }

        pPath = new QGraphicsPathItem();
        this->addItem(pPath);

        ppPath = new QGraphicsPathItem**[pointcount+5];
        created = new bool*[pointcount+5];//
        for(int i = 0; i < pointcount; i ++){
            ppPath[i] = new QGraphicsPathItem*[pointcount+5];
            created[i] = new bool[pointcount+5];
            for(int j = 0; j < pointcount;j ++) created[i][j] = 0;
        }

        for(int i = 0; i < pointcount; i ++){
            for(int j = i+1 ; j < pointcount; j ++){
                if(!created[i][j]){
                    ppPath[i][j] = new QGraphicsPathItem();
                    helper->paint(ppPath[i][j],p2p[i]->scenePos(),p2p[j]->scenePos(),ptpixel/2);
                    this->addItem(ppPath[i][j]);
                    ppPath[i][j]->setZValue(pointcount+antalgo.m+pathcount++);
                    created[i][j] = true;
                }
                if(created[i][j]){
                    qreal opac = 100.f*antalgo.pheromone[i][j];
                    if(opac > 1.f) opac = 1.f;
                    if(opac < 0.f) opac = 0.f;
                    ppPath[i][j]->setOpacity(opac);
                }
            }
        }

        //delete[] ant_dif;
        ant_dif = new QPointF[antalgo.m+5];

        antalgo.construct_solution_next();

        for(int i = 0 ; i < antalgo.m; i ++){
            if(antalgo.tour[i].size() == 0) continue;
            int s = antalgo.tour[i].s(antalgo.tour[i].size()-1);
            int r = antalgo.tour[i].r(antalgo.tour[i].size()-1);
            ant_dif[i] = p2p[s]->scenePos() - p2p[r]->scenePos();
            printf("m=%d,%d->%d\n",i,r,s);
        }

        progress = 0;
        algo_started = true;
        return;
    }

    //for(int i = 0 ; i < antalgo.m; i ++){
    //    printf("p2p_a[%d]=%d",)
    //}

    if(antalgo.construction_it >= antalgo.N){
        antalgo.update_pheromone();
        antalgo.reset();
        helper->paint(pPath,antalgo.best_so_far,p2p,ptpixel/2);
        opacity = 1.f;
        pPathFlag = true;
        for(int i = 0; i < pointcount; i ++){
            for(int j = i+1 ; j < pointcount; j ++){
                if(!created[i][j]){
                    ppPath[i][j] = new QGraphicsPathItem();
                    helper->paint(ppPath[i][j],p2p[i]->scenePos(),p2p[j]->scenePos(),ptpixel/2);
                    this->addItem(ppPath[i][j]);
                    ppPath[i][j]->setZValue(pointcount+antalgo.m+pathcount++);
                    created[i][j] = true;
                }
                if(created[i][j]){
                    qreal opac = 100.f*antalgo.pheromone[i][j];
                    if(opac > 1.f) opac = 1.f;
                    ppPath[i][j]->setOpacity(opac);
                }
            }
        }
    }


    antalgo.construct_solution_next();


    for(int i = 0 ; i < antalgo.m; i ++){
        if(antalgo.tour[i].size() == 0) continue;
        int s = antalgo.tour[i].s(antalgo.tour[i].size()-1);
        int r = antalgo.tour[i].r(antalgo.tour[i].size()-1);
        ant_dif[i] = p2p[s]->scenePos() - p2p[r]->scenePos();
        printf("m=%d,%d->%d\n",i,r,s);
    }

    if(progress < 100){
        for(int i = 0 ; i <antalgo.m ;  i ++){
            p2p_a[i]->setPos(p2p[antalgo.r[i]]->scenePos().x(),p2p[antalgo.r[i]]->scenePos().y());
            //addEllipse(p2p[antalgo.r[i]]->scenePos().x(),p2p[antalgo.r[i]]->scenePos().y(),10,10);
        }
    }
    for(int i = 0 ; i < antalgo.m; i ++){
        if(antalgo.tour[i].size() == 0) continue;
        printf("ant=%d,s=%d,r=%d\n",i,antalgo.tour[i].s(antalgo.tour[i].size()-1),antalgo.tour[i].r(antalgo.tour[i].size()-1));
    }
    //antalgo.update_pheromone();
    //if(antalgo.last > antalgo.best_so_far.L) antalgo.last = antalgo.best_so_far.L;
    //printf("iteration:%d,best_so_far=%d\n",antalgo.iteration++,antalgo.last);
    progress = 0;

    speed = friui->ui->lineEdit_5->text().toInt();
}
