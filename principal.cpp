#include "principal.h"
#include "ui_principal.h"

#include <QImage>
#include <QPainter>
#include <QMouseEvent>
#include <QPaintEvent>
#include <QDebug>
#include <QInputDialog>
#include <QColorDialog>
#include <QFileDialog>
#include <QMessageBox>
#include <QDialog>
#include <QAbstractButton>
#include <QIcon>
#include <QPixmap>

#define DEFAULT_ANCHO 3

Principal::Principal(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::Principal)
{
    ui->setupUi(this);
    // Instanciando la imagen (creando)
    mImagen = new QImage(this->size(), QImage::Format_ARGB32_Premultiplied);
    //Rellenar la imagen de color blanco
    mImagen->fill(Qt::white);
    // Instanciar el Painter a partir de mImagen (imagen)
    mPainter = new QPainter(mImagen);
    mPainter->setRenderHint(QPainter::Antialiasing);
    //Inicializar otras variables
    mPuedeDibujar = false;
    mColor = Qt::black;
    mAncho = DEFAULT_ANCHO;
    mNumLineas = 0;
    mCase = 2;
}

Principal::~Principal()
{
    delete ui;
    delete mPainter;
    delete mImagen;
}

void Principal::paintEvent(QPaintEvent *event)
{
    //Variable local
    //Crear el painter de la ventana principal)
    QPainter painter(this);
    // Dibujar la imagen
    painter.drawImage(0,0, *mImagen);
    //Aceptar el evento
    event->accept();
}

void Principal::mousePressEvent(QMouseEvent *event)
{
    mPuedeDibujar = true;
    //qDebug () << event->pos();
    //pos = posicion cursor
    mInicial = event->pos();
    //Aceptar el evento
    event->accept();
}

void Principal::mouseMoveEvent(QMouseEvent *event)
{
    //Validar si se puede dibujar
    /* if (!mPuedeDibujar){
        event->accept();
        return;
    }
    //Capturar el punto donde se suelta el mouse
    mFinal = event->pos();
    // Crear un pincel y establecer atributos
    QPen pincel;
    pincel.setColor(mColor);
    pincel.setWidth(mAncho);
    //Dibujar una linea
    mPainter->setPen(pincel);
    mPainter->drawLine(mInicial, mFinal);
    //Mostrar el numero de lineas en la barra de estado
    ui->statusbar->showMessage("Numero de lineas: " + QString::number(++mNumLineas));
    // Actualizar la interfaz
    update();
    //Actualizar el punto inicial
    mInicial = mFinal;
*/
    if(mCase==2){
        mPuedeDibujar = false;
        //Capturar el punto donde se suelta el mouse
        mFinal = event->pos();
        //Crear un pincel y establecer atributos
        QPen pincel;
        pincel.setColor(mColor);
        pincel.setWidth(mAncho);
        //Dibujar libremente
        mPainter->setPen(pincel);
        mPainter->drawLine(mInicial,mFinal);
        //Contar las lineas
        ui->statusbar->showMessage("Número de lineas: "+QString::number(++mNumLineas));
        update();
        mInicial=mFinal;
    }
}

void Principal::mouseReleaseEvent(QMouseEvent *event)
{
    //mPuedeDibujar = false;
    //qDebug () << event->pos();
    //pos = posicion cursor
    //Aceptar el evento
    //event->accept();

    if(mCase==1){
        mPuedeDibujar = false;
        // Capturar el punto donde se suelta el mouse
        mFinal = event->pos();
        //Crear un pincel y establecer atributos
        QPen pincel;
        pincel.setColor(mColor);
        pincel.setWidth(mAncho);
        //Dibujar lineas
        mPainter->setPen(pincel);
        mPainter->drawLine(mInicial,mFinal);
        //Contar las lineas
        ui->statusbar->showMessage("Número de lineas: "+QString::number(++mNumLineas));
        //Actualizar
        update();
        // Aceptar el evento
        event->accept();
    }
    if(mCase==3){
        mPuedeDibujar = false;
        // Capturar el punto donde se suelta el mouse
        mFinal = event->pos();
        //Crear un pincel y establecer atributos
        QPen pincel;
        pincel.setColor(mColor);
        pincel.setWidth(mAncho);
        //Dibujar rectangulos
        QRect rectangulo (mInicial, mFinal);
        mPainter->setPen(pincel);
        mPainter->drawRect(rectangulo);
        //Contar las lineas
        ui->statusbar->showMessage("Número de lineas: "+QString::number(++mNumLineas));
        //Actualizar
        update();
        // Aceptar el evento
        event->accept();
    }
    if(mCase==4){
        mPuedeDibujar = false;
        // Capturar el punto donde se suelta el mouse
        mFinal = event->pos();
        //Crear un pincel y establecer atributos
        QPen pincel;
        pincel.setColor(mColor);
        pincel.setWidth(mAncho);
        //Dibujar circunferencias
        QRectF circulos (mInicial, mFinal);
        mPainter->setPen(pincel);
        mPainter->drawEllipse(circulos);
        //Contar las lineas
        ui->statusbar->showMessage("Número de lineas: "+QString::number(++mNumLineas));
        //Actualizar
        update();
        // Aceptar el evento
        event->accept();
    }
}

void Principal::on_actionAncho_triggered()
{
    mAncho = QInputDialog::getInt(this, "Ancho del pincel",
                                  "Ingrese el ancho del pincel de dibujo",mAncho, 1, 100);
}

void Principal::on_actionSalir_triggered()
{
    QString nombreArchivo;
    if(!mImagen->save(nombreArchivo)){

        QMessageBox *box = new QMessageBox("Mi Paint", "¿Desea guardar los cambios?",
                                           QMessageBox::NoIcon, QMessageBox::Save,
                                           QMessageBox::Discard, QMessageBox::Cancel);

        box->button(QMessageBox::Save)->setText("Guardar");
        box->button(QMessageBox::Discard)->setText("No guardar");
        box->button(QMessageBox::Cancel)->setText("Cancelar");
        box->show();

        int ret = box->exec();
        switch (ret) {
        case QMessageBox::Save:
            QFileDialog::getSaveFileName(this,
                                         "Guardar imagen",
                                         QString(),
                                         "Imagenes (*.png)");
            if (!nombreArchivo.isEmpty()){
                if (mImagen->save(nombreArchivo))
                    QMessageBox::information(this,
                                             "Guardar imagen",
                                             "Archivo almacenado en: " + nombreArchivo);
                else
                    QMessageBox::warning(this,
                                         "Guardar imagen",
                                         "No se pudo almacenar la imagen.");
                break;
        case QMessageBox::Discard:
                    // Don't Save was clicked
                    this->close();
                    break;
            }
        }
    }
}

void Principal::on_actionNuevo_triggered()
{
    QString nombreArchivo;
    if(!mImagen->save(nombreArchivo)){

        QMessageBox *box = new QMessageBox("Mi Paint", "¿Desea guardar los cambios?",
                                           QMessageBox::NoIcon, QMessageBox::Save, QMessageBox::Discard, QMessageBox::Cancel);

        box->button(QMessageBox::Save)->setText("Guardar");
        box->button(QMessageBox::Discard)->setText("No guardar");
        box->button(QMessageBox::Cancel)->setText("Cancelar");
        box->show();

        int ret = box->exec();
        switch (ret) {
        case QMessageBox::Save:
            //Save was clicked
            QFileDialog::getSaveFileName(this,
                                         "Guardar imagen",
                                         QString(),
                                         "Imagenes (*.png)");
            if (!nombreArchivo.isEmpty()){
                if (mImagen->save(nombreArchivo))
                    QMessageBox::information(this,
                                             "Guardar imagen",
                                             "Archivo almacenado en: " + nombreArchivo);
                else
                    QMessageBox::warning(this,
                                         "Guardar imagen",
                                         "No se pudo almacenar la imagen.");
                break;
        case QMessageBox::Discard:
                    //Don't Save was clicked
                    mImagen->fill(Qt::white);
                    mNumLineas = 0;
                    update();
                    break;
            }
        }
    }
}

void Principal::on_actionColor_triggered()
{
    mColor = QColorDialog::getColor(mColor,this, "Color del pincel");
}

void Principal::on_actionGuardar_triggered()
{
    QString nombreArchivo = QFileDialog::getSaveFileName(this,
                                                         "Guardar imagen",
                                                         QString(),
                                                         "Imagenes (*.png)");
    if (!nombreArchivo.isEmpty()){
        if (mImagen->save(nombreArchivo))
            QMessageBox::information(this,
                                     "Guardar imagen",
                                     "Archivo almacenado en: " + nombreArchivo);
        else
            QMessageBox::warning(this,
                                 "Guardar imagen",
                                 "No se pudo almacenar la imagen.");
    }
}

void Principal::on_actionLineas_triggered()
{
    mCase=1;
}

void Principal::on_actionLibre_triggered()
{
    mCase=2;
}

void Principal::on_actionRect_ngulos_triggered()
{
    mCase=3;
}

void Principal::on_actionCircunferencia_triggered()
{
    mCase=4;
}
