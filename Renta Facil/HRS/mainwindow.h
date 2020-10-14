#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPdfWriter>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include <QDebug>
#include <QtSql>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QMessageBox>
#include <QDebug>
#include <QTableWidgetItem>
#include<QtCore>
#include<QtGui>
#include <QPrinter>
#include <QPainter>


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    QString ruta1;
    QByteArray imagen1;
    QString ruta3;
    QByteArray imagen2;
    int f;

private slots:

    void on_registro_clicked();

    void on_OContra_clicked();

    void on_regresar_clicked();

    void on_CrearCuenta_clicked();

    void on_regresar_2_clicked();

    void on_cambiarC_clicked();

    void on_IniciarSesion_clicked();

    void Detalles();

    void on_salir_14_clicked();

    void on_salir_8_clicked();

    void on_AddCasa_clicked();
    
    void on_addFoto_clicked();

    void on_ajustes_13_clicked();

    void on_home_8_clicked();

    void on_add_8_clicked();

    void on_reservaciones_8_clicked();

    void on_ajustes_8_clicked();

    void on_home_10_clicked();

    void on_add_10_clicked();

    void on_reservaciones_10_clicked();

    void on_ajustes_10_clicked();

    void on_salir_10_clicked();

    void on_home_11_clicked();

    void on_add_11_clicked();

    void on_reservaciones_11_clicked();

    void on_ajustes_11_clicked();

    void on_salir_11_clicked();

    void on_home_13_clicked();

    void on_add_13_clicked();

    void on_reservaciones_13_clicked();

    void on_salir_13_clicked();

    void on_CambiarFoto_2_clicked();

    void on_aceptar_9_clicked();

    void on_aceptar_10_clicked();

    void on_aceptar_11_clicked();

    void on_home_14_clicked();

    void on_perfil_14_clicked();

    void on_reservaciones_14_clicked();

    void on_home_12_clicked();

    void on_perfil_12_clicked();

    void on_reservaciones_12_clicked();

    void on_salir_12_clicked();

    void on_home_4_clicked();

    void on_perfil_4_clicked();

    void on_reservaciones_4_clicked();

    void on_salir_4_clicked();

    void on_volver_clicked();

    void on_enviar_clicked();

    void Datos();

    void DatosC();

    void DatosDireccion();

    void DatosClienteDireccion();

    void DatosFoto();

    void DatosClienteFoto();

    void on_home_5_clicked();

    void on_perfil_5_clicked();

    void on_reservaciones_5_clicked();

    void on_salir_5_clicked();

    void on_tar_aceptar_clicked();

    void on_regresar_3_clicked();

    void on_home_6_clicked();

    void on_perfil_6_clicked();

    void on_reservaciones_6_clicked();

    void on_salir_6_clicked();

    void on_aceptar_2_clicked();

    void on_aceptar_clicked();

    void on_aceptar_3_clicked();

    void on_CambiarFoto_clicked();

    void on_aceptar_4_clicked();

    void on_regreso_clicked();

    void on_home_7_clicked();

    void on_perfil_7_clicked();

    void on_reservaciones_7_clicked();

    void on_salir_7_clicked();

    void on_ir_clicked();

    void on_ir_3_clicked();

    void CasaEspera();

    void on_anadir_clicked();

    void on_volver_3_clicked();

    void Detalles2();

    void on_solicitudesTableWidget_itemClicked(QTableWidgetItem *item);

    void on_reserva_clicked();

    void on_volver4_clicked();

    void on_volver_2_clicked();

    void on_regresar_7_clicked();

    void on_eliminar_clicked();

    void CatalogoPrincipal();

    void on_Salir_clicked();

    void CatalogoPropi();

     void CatalogoAdmin();

     void darEspecificacion(QString);

     void darEspecificacionP(QString);

     void darEspecificacionA(QString);

     void on_home_9_clicked();

     void on_add_9_clicked();

     void on_reservaciones_9_clicked();

     void on_ajustes_9_clicked();

     void on_salir_9_clicked();

     void on_RCat_clicked();

     void on_Rechazar_clicked();

     void Reservar();

     void Pago();

     void CatalogoReservacion();

     void darEspecificacion4(QString clave);

     void on_abrir_2_clicked();

     void on_generac_clicked();

     void on_home_15_clicked();

     void on_perfil_8_clicked();

     void on_reservaciones_15_clicked();

     void on_salir_15_clicked();

     void on_pushButton_clicked();

     void on_abrir_clicked();

     void on_descargar_clicked();

     void on_Regresar_clicked();

     void Reservacion_actviva();


     void on_cancelar_clicked();

private:
    Ui::MainWindow *ui;
    QSqlDatabase db;
    QString ruta;
    QString ruta2;
    QString iniciar, username, password, admin;
};
#endif // MAINWINDOW_H
