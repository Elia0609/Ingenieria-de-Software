#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFile>
#include <QFileDialog>
#include <QMessageBox>
#include <QPixmap>
#include<QString>
#include <QSqlQuery>
#include<QMessageBox>
#include <QFile>
#include <QFileDialog>
#include <QPixmap>
#include <QCompleter>
#include <QFileSystemModel>
#include <QImage>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow){

    ui->setupUi(this);
    db = QSqlDatabase::addDatabase("QMYSQL");
    db.setPort(3306);
    db.setHostName("localhost");
    db.setUserName("root");
    db.setPassword("");
    db.setDatabaseName("rentas");
    if (!db.open()){ qDebug() << "Error al conectar."; }
    else{ qDebug() << "Conectada."; }
    f= -1;
    CasaEspera();
    CatalogoPrincipal();
    CatalogoPropi();
    CatalogoAdmin();

}

void MainWindow::Detalles(){
    ui->stackedWidget->setCurrentIndex(5);
}
void MainWindow::Detalles2(){
    ui->stackedWidget->setCurrentIndex(18);
}
MainWindow::~MainWindow(){
    delete ui;
}

//Catalogos
void clearLayout(QLayout *layout) {
    QLayoutItem *item;
    while((item = layout->takeAt(0))) {
        if (item->layout()) {
            clearLayout(item->layout());
            delete item->layout();
        }
        if (item->widget()) {
           delete item->widget();
        }
        delete item;
    }
}

//--------------------------------------Interfaces principales----------------------------------

void MainWindow::on_IniciarSesion_clicked(){
    QSqlQuery sql;
    username = ui -> lineEdit_Usuario -> text();
    password = ui -> lineEdit_password -> text();


    iniciar="select usuario, contrasena from login where usuario='" +username+"' and contrasena = '"+ password +"'";

    if(ui->lineEdit_Usuario->text()=="Admin" && ui->lineEdit_password->text()=="admin" ){
        CasaEspera();
        ui->stackedWidget->setCurrentIndex(15);
    }
    else{
        if(ui->ButtonCliente_2->isChecked()){
            if(ui->ButtonCliente_2->text()=="Cliente"){
                if(sql.exec(iniciar)){
                    CatalogoPrincipal();
                    QMessageBox::about(this,QObject::tr( "Bienvenido"), QObject::tr("Bienvenido Cliente"));
                    ui->stackedWidget->setCurrentIndex(3);
                }
            }
        }
        if(ui->ButtonPropi_2->isChecked()){
            if(ui->ButtonPropi_2->text()=="Propietario"){
                if(sql.exec(iniciar)){
                    QMessageBox::about(this,QObject::tr( "Bienvenido"), QObject::tr("Bienvenido Propietario"));
                    ui->stackedWidget->setCurrentIndex(10);
                    CatalogoPropi();
                }
            }
        }

    }
    ui -> lineEdit_Usuario ->clear();
    ui -> lineEdit_password ->clear();
    ui->ButtonCliente_2->setAutoExclusive(false);
    ui->ButtonCliente_2->setChecked(false);
    ui->ButtonPropi_2->setAutoExclusive(false);
    ui->ButtonPropi_2->setChecked(false);
}
void MainWindow::on_registro_clicked(){
    ui->stackedWidget->setCurrentIndex(1);
}
void MainWindow::on_regresar_clicked(){
    ui->stackedWidget->setCurrentIndex(0);
}
void MainWindow::on_OContra_clicked(){
    ui->stackedWidget->setCurrentIndex(2);
}
void MainWindow::on_CrearCuenta_clicked(){
    if (db.open()){
        QString nombre, apellidom, apellidop, usuario, email, contra, tel , checkExist, existente;
        QSqlQuery sql, sql2, sql3;
        nombre=ui->NombreReg->text();
        apellidom=ui->ApellidoReg->text();
        apellidop=ui->ApellidoReg_2->text();
        usuario=ui->UsuarioReg->text();
        tel=ui->TelReg->text();
        email=ui->CorreoReg->text();
        contra=ui->ContraReg->text();
        existente = "select * from login where usuario = '"+ usuario +"'";

        if(nombre.isEmpty()||apellidom.isEmpty()||apellidop.isEmpty()||email.isEmpty()||contra.isEmpty()||usuario.isEmpty()|| tel.isEmpty()){

            QMessageBox::about(this, QObject::tr("Error"), QObject::tr("Campos Vacios"));
        }
        else{
            if(sql.exec(existente)){
                if(sql.next() == true){
                    QMessageBox::about(this ,QObject::tr( "Error"), QObject::tr("Nombre de usuario ya existe"));
                }
                else{
                    if(ui->ButtonCliente->isChecked()){
                        if(ui->ButtonCliente->text()=="Cliente"){
                            qDebug() << "insert into Login(usuario,  contrasena) values ('" + usuario+ "','" +contra+ "');";
                            qDebug() << "insert into cliente(nombre, a_paterno, a_materno, telefono, correo, usuario) values('" +nombre+ "','" +apellidop+ "','" +apellidom+ "','"  +tel+ "','" +email+ "','" +usuario+ "');";

                            if (sql2.exec("insert into Login(usuario,  contrasena) values ('" + usuario+ "','" +contra+ "')")){
                                if(sql3.exec("insert into cliente (usuario, nombre, a_paterno, a_materno, telefono, correo) values('" +usuario+ "','" +nombre+ "','" +apellidop+ "','" +apellidom+ "','"  +tel+ "','" +email+ "')")){
                                    QMessageBox::about(this,QObject::tr( "Registro"), QObject::tr("Registro exitoso"));
                                    ui->stackedWidget->setCurrentIndex(0);
                                }
                                else{
                                    QMessageBox::about(this,QObject::tr( "Error"), QObject::tr("La cuenta no se ha creado"));
                                }
                            }
                        }
                    }
                    if(ui->ButtonPropi->isChecked()){
                        if(ui->ButtonPropi->text()=="Propietario"){
                            qDebug() << "insert into Login(usuario,  contrasena) values ('" + usuario+ "','" +contra+ "');";
                            qDebug() << "insert into propietario(nombre, a_paterno, a_materno, telefono, correo, usuario) values('" +nombre+ "','" +apellidop+ "','" +apellidom+ "','"  +tel+ "','" +email+ "','" +usuario+ "');";

                            if (sql2.exec("insert into Login(usuario,  contrasena) values ('" + usuario+ "','" +contra+ "')")){
                                if(sql3.exec("insert into propietario (usuario, nombre, a_paterno, a_materno, telefono, correo) values('" +usuario+ "','" +nombre+ "','" +apellidop+ "','" +apellidom+ "','"  +tel+ "','" +email+ "')")){
                                   QMessageBox::about(this,QObject::tr( "Registro"), QObject::tr("Registro exitoso"));
                                   ui->stackedWidget->setCurrentIndex(0);
                                }
                                else{
                                    QMessageBox::about(this,QObject::tr( "Registro"), QObject::tr("Cuenta no creada"));
                                }
                            }
                        }
                    }
                    ui->NombreReg->clear();
                    ui->ApellidoReg->clear();
                    ui->ApellidoReg_2->clear();
                    ui->UsuarioReg->clear();
                    ui->TelReg->clear();
                    ui->CorreoReg->clear();
                    ui->ContraReg->clear();
                }
                ui->NombreReg->clear();
                ui->ApellidoReg->clear();
                ui->ApellidoReg_2->clear();
                ui->UsuarioReg->clear();
                ui->TelReg->clear();
                ui->CorreoReg->clear();
                ui->ContraReg->clear();
            }
        }
    }
}
void MainWindow::on_regresar_2_clicked(){
    ui->stackedWidget->setCurrentIndex(0);
}
void MainWindow::on_cambiarC_clicked(){

    QString username=ui->user-> text();
    QString password = ui -> CPass -> text();
    QString password2 = ui -> CPass2-> text();
    QSqlQuery query, query2;
    if (query.exec("update login set contrasena = '"+password2+"' where usuario = '"+username+"' and contrasena= '"+password+"';")){
         QMessageBox::about(this,QObject::tr( "Contraseña"), QObject::tr("La Contraseña ha sido cambiada"));
        ui->stackedWidget->setCurrentIndex(0);
    }
}

void MainWindow::CatalogoPrincipal(){
    QSqlQuery catalogo,sacarfoto;
       QString catalogosql, titulo, llamarFoto;
       QPixmap imagen;
       QIcon imagenreal;
       QByteArray img;

       catalogosql = "select nombre from Casa where casa.idCatalogo='2'; ";
       catalogo.exec(catalogosql);
       int counterf = 0;
       int counterc = 0;

       while (catalogo.next()){
           titulo = catalogo.value(0).toString();
           llamarFoto="call Nombre('"+titulo+"');";
           sacarfoto.exec(llamarFoto);
           sacarfoto.next();
           img=sacarfoto.value(0).toByteArray();
           imagen.loadFromData(img);

           QLabel *label = new QLabel;
           label->setPixmap(imagen.scaled(210, 125));
           label->setFixedSize(210, 125);
           ui->layout->addWidget(label, counterf, counterc,1,1);

           QPushButton *h = new QPushButton("Ver detalles");
           h->setAccessibleName(titulo);
           h->setFixedSize(90, 20);
           ui->layout->addWidget(h , counterf+1, counterc, 1, 1);
           QSignalMapper *mapper=new QSignalMapper(this);
           connect(h,SIGNAL(clicked(bool)),mapper,SLOT(map()));
           mapper->setMapping(h,titulo);
           connect(mapper,SIGNAL(mapped(QString)),this,SLOT(darEspecificacion(QString)));

           counterc++;

           if (counterc == 3){
               counterf= counterf + 2;
               counterc = 0;
           }
       }
}
void MainWindow::darEspecificacion(QString clave) {
    QString  estado, municipio, calle, no_interior, no_exterior, cp, id, casa, llamarFoto, buscar, info, nombre, no_habitaciones, no_banos, precio, descripcion, servicios, valoracion;

    buscar="select titulo from casa where titulo='"+clave+"'; ";
    QSqlQuery Informacion, query, sacarfoto, casas;

    Informacion.exec(buscar);
    Informacion.next();
    QByteArray img;
    QPixmap imagen;
    QPixmap cinco( "C:/Users/ailes/OneDrive/Escritorio/HRS/Recursos/5estrellas.png" );
    QPixmap cuatro( "C:/Users/ailes/OneDrive/Escritorio/HRS/Recursos/4estrellas.png" );
    QPixmap tres( "C:/Users/ailes/OneDrive/Escritorio/HRS/Recursos/3estrellas.png" );
    QPixmap dos( "C:/Users/ailes/OneDrive/Escritorio/HRS/Recursos/2estrellas.png" );
    QPixmap uno( "C:/Users/ailes/OneDrive/Escritorio/HRS/Recursos/1estrellas.png" );

    llamarFoto="call Nombre('"+clave+"');";
    sacarfoto.exec(llamarFoto);
    sacarfoto.next();
    img=sacarfoto.value(0).toByteArray();
    imagen.loadFromData(img);

    ui->imagPrincipal->setPixmap(imagen.scaled(251, 151));
    ui->imagPrincipal->setFixedSize(251, 151);


    ui->stackedWidget->setCurrentIndex(5);
    info="select nombre, no_habitaciones, no_banos, precio, descripcion, servicios, valoracion from casa where casa.nombre= '"+ clave +"'";
    if (query.exec(info)){
        while (query.next()){
            nombre = query.value(0).toString();
            no_habitaciones = query.value(1).toString();
            no_banos = query.value(2).toString();
            precio = query.value(3).toString();
            descripcion = query.value(4).toString();
            servicios = query.value(5).toString();
            valoracion = query.value(6).toString();
            qDebug()<<valoracion;

            if(valoracion=="5"){
                ui->valoracion->setPixmap(cinco.scaled(91,20));
            }
            if(valoracion=="4"){
                ui->valoracion->setPixmap(cuatro.scaled(91,20));
            }
            if(valoracion=="3"){
                ui->valoracion->setPixmap(tres.scaled(91,20));
            }
            if(valoracion=="2"){
                ui->valoracion->setPixmap(dos.scaled(91,20));
            }
            if(valoracion=="1"){
                ui->valoracion->setPixmap(uno.scaled(91,20));
            }

            ui->Nom->setText(nombre);
            ui->titulo->setText(nombre);
            ui->label_85->setText(nombre);
            ui->Habit->setText(no_habitaciones+" habitaciones");
            ui->banos->setText(no_banos+" baños");
            ui->precio->setText("$ "+precio+" MXN");
            ui->desc->setText(descripcion);
            ui->servicio->setText(servicios);
        }
    }

    id="select idDireccion from casa where Casa.nombre = '"+ clave +"'";
    QSqlQuery insert2;
    insert2.prepare(id);
    insert2.exec();
    while ( insert2.next()){
        id=insert2.value(0).toString();
    }

    casa="select estado, municipio, calle, no_interior, no_exterior, cp from direccion where direccion.idDireccion= '"+ id +"'";
    if (casas.exec(casa)){
        while (casas.next()){
            estado = casas.value(0).toString();
            municipio = casas.value(1).toString();
            calle = casas.value(2).toString();
            no_interior = casas.value(3).toString();
            no_exterior = casas.value(4).toString();
            cp = casas.value(5).toString();

            ui->estadoRes->setText(estado);
            ui->municipioRes->setText(municipio);
            ui->calleRes->setText(calle);
            ui->NIRes->setText(no_interior);
            ui->NERes->setText(no_exterior);
            ui->CPRes->setText(cp);

        }
    }

}
void MainWindow::CatalogoPropi(){
    QSqlQuery catalogo,sacarfoto;
    QString catalogosql, titulo, llamarFoto, idP;
    QPixmap imagen;
    QIcon imagenreal;
    QByteArray img;

    idP="select idPropietario from Propietario where Propietario.usuario= '" +username+ " '; ";
    QSqlQuery idPropi;
    idPropi.prepare(idP);
    idPropi.exec();
    while ( idPropi.next()){
        idP=idPropi.value(0).toString();
    }

    qDebug()<< idP;

    catalogosql = "select nombre from Casa where casa.idPropietario= '"+idP+"' and casa.idCatalogo='2';";
    catalogo.exec(catalogosql);
    int counterf = 0;
    int counterc = 0;
    qDebug()<< catalogosql;

    while (catalogo.next()){
        titulo = catalogo.value(0).toString();
        llamarFoto="call Nombre('"+titulo+"');";
        sacarfoto.exec(llamarFoto);
        sacarfoto.next();
        img=sacarfoto.value(0).toByteArray();
        imagen.loadFromData(img);

        QLabel *label = new QLabel;
        label->setPixmap(imagen.scaled(200, 125));
        label->setMaximumWidth(200);
        label->setMaximumHeight(125);
        ui->catalodo_propi->addWidget(label, counterf, counterc,1,1);

        QPushButton *h = new QPushButton("Ver detalles");
        h->setAccessibleName(titulo);
        h->setFixedSize(90, 20);
        ui->catalodo_propi->addWidget(h , counterf+1, counterc, 1, 1);
        QSignalMapper *mapper=new QSignalMapper(this);
        connect(h,SIGNAL(clicked(bool)),mapper,SLOT(map()));
        mapper->setMapping(h,titulo);
        connect(mapper,SIGNAL(mapped(QString)),this,SLOT(darEspecificacionP(QString)));

        counterc++;

        if (counterc == 3){
            counterf= counterf + 2;
            counterc = 0;
        }
    }
}
void MainWindow::darEspecificacionP(QString clave){
    QString  estado, municipio, calle, no_interior, no_exterior, cp, id, casa, llamarFoto, buscar, info, nombre, no_habitaciones, no_banos, precio, descripcion, servicios, valoracion;

    buscar="select nombre from casa where casa.nombre='"+clave+"'; ";
    QSqlQuery Informacion, query, sacarfoto, casas;

    Informacion.exec(buscar);
    Informacion.next();
    QByteArray img;
    QPixmap imagen;
    QPixmap cinco( "C:/Users/ailes/OneDrive/Escritorio/HRS/Recursos/5estrellas.png" );
    QPixmap cuatro( "C:/Users/ailes/OneDrive/Escritorio/HRS/Recursos/4estrellas.png" );
    QPixmap tres( "C:/Users/ailes/OneDrive/Escritorio/HRS/Recursos/3estrellas.png" );
    QPixmap dos( "C:/Users/ailes/OneDrive/Escritorio/HRS/Recursos/2estrellas.png" );
    QPixmap uno( "C:/Users/ailes/OneDrive/Escritorio/HRS/Recursos/1estrellas.png" );

    llamarFoto="call Nombre('"+clave+"');";
    sacarfoto.exec(llamarFoto);
    sacarfoto.next();
    img=sacarfoto.value(0).toByteArray();
    imagen.loadFromData(img);

    ui->imgCas->setPixmap(imagen.scaled(241, 221));
    ui->imgCas->setFixedSize(241, 221);


    ui->stackedWidget->setCurrentIndex(11);
    info="select nombre, no_habitaciones, no_banos, precio, descripcion, servicios, valoracion from casa where casa.nombre= '"+ clave +"'";
    if (query.exec(info)){
        while (query.next()){
            nombre = query.value(0).toString();
            no_habitaciones = query.value(1).toString();
            no_banos = query.value(2).toString();
            precio = query.value(3).toString();
            descripcion = query.value(4).toString();
            servicios = query.value(5).toString();
            valoracion = query.value(6).toString();

            if(valoracion=="5"){
                ui->label_83->setPixmap(cinco.scaled(91,20));
            }
            if(valoracion=="4"){
                ui->label_83->setPixmap(cuatro.scaled(91,20));
            }
            if(valoracion=="3"){
                ui->label_83->setPixmap(tres.scaled(91,20));
            }
            if(valoracion=="2"){
                ui->label_83->setPixmap(dos.scaled(91,20));
            }
            if(valoracion=="1"){
                ui->label_83->setPixmap(uno.scaled(91,20));
            }

            ui->nom_casa->setText(nombre);
            ui->habitaciones_2->setText(no_habitaciones+" habitaciones");
            ui->Nbanos->setText(no_banos+" baños");
            ui->precio_2->setText("$ "+precio+" MXN");
            ui->desc_2->setText(descripcion);
            ui->servicios->setText(servicios);
        }
    }

    id="select idDireccion from casa where Casa.nombre = '"+ clave +"'";
    QSqlQuery insert2;
    insert2.prepare(id);
    insert2.exec();
    while ( insert2.next()){
        id=insert2.value(0).toString();
    }

    casa="select estado, municipio, calle, no_interior, no_exterior, cp from direccion where direccion.idDireccion= '"+ id +"'";
    if (casas.exec(casa)){
        while (casas.next()){
            estado = casas.value(0).toString();
            municipio = casas.value(1).toString();
            calle = casas.value(2).toString();
            no_interior = casas.value(3).toString();
            no_exterior = casas.value(4).toString();
            cp = casas.value(5).toString();

            ui->estado_2->setText(estado);
            ui->colonia->setText(municipio);
            ui->calle->setText(calle);
            ui->no_int->setText(no_interior);
            ui->no_ext->setText(no_exterior);
            ui->cp->setText(cp);

        }
    }

}
void MainWindow::CatalogoAdmin(){
    QSqlQuery catalogo,sacarfoto;
    QString catalogosql, titulo, llamarFoto;
    QPixmap imagen;
    QIcon imagenreal;
    QByteArray img;

    catalogosql = "select nombre from Casa where casa.idCatalogo='2'; ";
    catalogo.exec(catalogosql);
    int counterf = 0;
    int counterc = 0;

    while (catalogo.next()){
        titulo = catalogo.value(0).toString();
        llamarFoto="call Nombre('"+titulo+"');";
        sacarfoto.exec(llamarFoto);
        sacarfoto.next();
        img=sacarfoto.value(0).toByteArray();
        imagen.loadFromData(img);

        QLabel *label = new QLabel;
        label->setPixmap(imagen.scaled(210, 125));
        label->setFixedSize(210, 125);
        ui->layout_3->addWidget(label, counterf, counterc,1,1);

        QPushButton *h = new QPushButton("Ver detalles");
        h->setAccessibleName(titulo);
        h->setFixedSize(90, 20);
        ui->layout_3->addWidget(h , counterf+1, counterc, 1, 1);
        QSignalMapper *mapper=new QSignalMapper(this);
        connect(h,SIGNAL(clicked(bool)),mapper,SLOT(map()));
        mapper->setMapping(h,titulo);
        connect(mapper,SIGNAL(mapped(QString)),this,SLOT(darEspecificacionA(QString)));

        counterc++;

        if (counterc == 3){
            counterf= counterf + 2;
            counterc = 0;
        }
    }
}
void MainWindow::darEspecificacionA(QString clave){

    QString  estado, municipio, calle, no_interior, no_exterior, cp, id, casa, llamarFoto, buscar, info, nombre, no_habitaciones, no_banos, precio, descripcion, servicios, valoracion;
    QSqlQuery Informacion, query, sacarfoto, casas;

    Informacion.exec(buscar);
    Informacion.next();
    QByteArray img;
    QPixmap imagen;
    QPixmap cinco( "C:/Users/ailes/OneDrive/Escritorio/HRS/Recursos/5estrellas.png" );
    QPixmap cuatro( "C:/Users/ailes/OneDrive/Escritorio/HRS/Recursos/4estrellas.png" );
    QPixmap tres( "C:/Users/ailes/OneDrive/Escritorio/HRS/Recursos/3estrellas.png" );
    QPixmap dos( "C:/Users/ailes/OneDrive/Escritorio/HRS/Recursos/2estrellas.png" );
    QPixmap uno( "C:/Users/ailes/OneDrive/Escritorio/HRS/Recursos/1estrellas.png" );

    llamarFoto="call Nombre('"+clave+"');";
    sacarfoto.exec(llamarFoto);
    sacarfoto.next();
    img=sacarfoto.value(0).toByteArray();
    imagen.loadFromData(img);

    ui->label_ImagenCasa_18->setPixmap(imagen.scaled(281, 241));
    ui->label_ImagenCasa_18->setFixedSize(281, 241);


    ui->stackedWidget->setCurrentIndex(17);
    info="select nombre, no_habitaciones, no_banos, precio, descripcion, servicios, valoracion from casa where casa.nombre= '"+ clave +"'";
    if (query.exec(info)){
        while (query.next()){
            nombre = query.value(0).toString();
            no_habitaciones = query.value(1).toString();
            no_banos = query.value(2).toString();
            precio = query.value(3).toString();
            descripcion = query.value(4).toString();
            servicios = query.value(5).toString();
            valoracion = query.value(6).toString();
            qDebug()<<valoracion;

            if(valoracion=="5"){
                ui->mascota_2->setPixmap(cinco.scaled(91,19));
            }
            if(valoracion=="4"){
                ui->mascota_2->setPixmap(cuatro.scaled(91,19));
            }
            if(valoracion=="3"){
                ui->mascota_2->setPixmap(tres.scaled(91,19));
            }
            if(valoracion=="2"){
                ui->mascota_2->setPixmap(dos.scaled(91,19));
            }
            if(valoracion=="1"){
                ui->mascota_2->setPixmap(uno.scaled(91,19));
            }

            ui->label_299->setText(nombre);
            ui->habitaciones_3->setText(no_habitaciones+" habitaciones");
            ui->banos_3->setText(no_banos+" baños");
            ui->precio_4->setText("$ "+precio+" MXN");
            ui->Desc->setText(descripcion);
            ui->servicio_2->setText(servicios);
        }
     }
    id="select idDireccion from casa where Casa.nombre = '"+ clave +"'";
    QSqlQuery insert2;
    insert2.prepare(id);
    insert2.exec();
    while ( insert2.next()){
        id=insert2.value(0).toString();
    }

    casa="select estado, municipio, calle, no_interior, no_exterior, cp from direccion where direccion.idDireccion= '"+ id +"'";
    if (casas.exec(casa)){
        while (casas.next()){
            estado = casas.value(0).toString();
            municipio = casas.value(1).toString();
            calle = casas.value(2).toString();
            no_interior = casas.value(3).toString();
            no_exterior = casas.value(4).toString();
            cp = casas.value(5).toString();

            ui->estado_4->setText(estado);
            ui->Mun->setText(municipio);
            ui->calle_3->setText(calle);
            ui->no_interior_2->setText(no_interior);
            ui->no_exterior_2->setText(no_exterior);
            ui->cp_3->setText(cp);

        }
    }
}
void MainWindow::CatalogoReservacion(){
    QSqlQuery catalogo,sacarfoto;
       QString catalogosql, titulo, llamarFoto, id;
       QPixmap imagen;
       QIcon imagenreal;
       QByteArray img;

       id="select idCliente from cliente where cliente.usuario= '"+username+"'";
       QSqlQuery insert2, cliente;
       insert2.prepare(id);
       insert2.exec();
       while ( insert2.next()){
           id=insert2.value(0).toString();
       }

       catalogosql = "select nombre from Casa where casa.idCatalogo='4' and casa.idCliente= '"+id+"'; ";
       catalogo.exec(catalogosql);
       int counterf = 0;
       int counterc = 0;
       qDebug()<<catalogosql;
       while (catalogo.next()){
           titulo = catalogo.value(0).toString();
           llamarFoto="call CatalogoC('"+titulo+"');";
           sacarfoto.exec(llamarFoto);
           sacarfoto.next();
           img=sacarfoto.value(0).toByteArray();
           imagen.loadFromData(img);

           QLabel *label = new QLabel;
           label->setPixmap(imagen.scaled(210, 125));
           label->setFixedSize(210, 125);
           ui->res_activas->addWidget(label, counterf, counterc,1,1);

           QPushButton *h = new QPushButton("Ver detalles");
           h->setAccessibleName(titulo);
           h->setFixedSize(90, 20);
           ui->res_activas->addWidget(h , counterf+1, counterc, 1, 1);
           QSignalMapper *mapper=new QSignalMapper(this);
           connect(h,SIGNAL(clicked(bool)),mapper,SLOT(map()));
           mapper->setMapping(h,titulo);
           connect(mapper,SIGNAL(mapped(QString)),this,SLOT(darEspecificacion4(QString)));

           counterc++;

           if (counterc == 3){
               counterf= counterf + 2;
               counterc = 0;
           }
       }
}
void MainWindow::darEspecificacion4(QString clave){
    QString  estado, municipio, calle, no_interior, no_exterior, cp, id, casa, llamarFoto, buscar, info, nombre, no_habitaciones, no_banos, precio, descripcion, servicios, valoracion;

    buscar="select nombre from casa where casa.nombre='"+clave+"'; ";
    QSqlQuery Informacion, query, sacarfoto, casas;

    Informacion.exec(buscar);
    Informacion.next();
    QByteArray img;
    QPixmap imagen;
    QPixmap cinco( "C:/Users/ailes/OneDrive/Escritorio/HRS/Recursos/5estrellas.png" );
    QPixmap cuatro( "C:/Users/ailes/OneDrive/Escritorio/HRS/Recursos/4estrellas.png" );
    QPixmap tres( "C:/Users/ailes/OneDrive/Escritorio/HRS/Recursos/3estrellas.png" );
    QPixmap dos( "C:/Users/ailes/OneDrive/Escritorio/HRS/Recursos/2estrellas.png" );
    QPixmap uno( "C:/Users/ailes/OneDrive/Escritorio/HRS/Recursos/1estrellas.png" );

    llamarFoto="call CatalogoC('"+clave+"');";
    qDebug()<<llamarFoto;
    sacarfoto.exec(llamarFoto);
    sacarfoto.next();
    img=sacarfoto.value(0).toByteArray();
    imagen.loadFromData(img);

    ui->img->setPixmap(imagen.scaled(201, 181));
    ui->img->setFixedSize(201, 181);


    ui->stackedWidget->setCurrentIndex(21);
    info="select nombre, no_habitaciones, no_banos, precio, descripcion, servicios, valoracion from casa where casa.nombre= '"+ clave +"'";
    if (query.exec(info)){
        while (query.next()){
            nombre = query.value(0).toString();
            no_habitaciones = query.value(1).toString();
            no_banos = query.value(2).toString();
            precio = query.value(3).toString();
            descripcion = query.value(4).toString();
            servicios = query.value(5).toString();
            valoracion = query.value(6).toString();
            qDebug()<<valoracion;

            if(valoracion=="5"){
                ui->valoracion_2->setPixmap(cinco.scaled(91,20));
            }
            if(valoracion=="4"){
                ui->valoracion_2->setPixmap(cuatro.scaled(91,20));
            }
            if(valoracion=="3"){
                ui->valoracion_2->setPixmap(tres.scaled(91,20));
            }
            if(valoracion=="2"){
                ui->valoracion_2->setPixmap(dos.scaled(91,20));
            }
            if(valoracion=="1"){
                ui->valoracion_2->setPixmap(uno.scaled(91,20));
            }

            ui->Nom_2->setText(nombre);
            ui->Habit_2->setText(no_habitaciones+" habitaciones");
            ui->banos_4->setText(no_banos+" baños");
            ui->precio_5->setText("$ "+precio+" MXN");
            ui->desc_3->setText(descripcion);
            ui->servicio_3->setText(servicios);
        }
    }

    id="select idDireccion from casa where Casa.nombre = '"+ clave +"'";
    QSqlQuery insert2;
    insert2.prepare(id);
    insert2.exec();
    while ( insert2.next()){
        id=insert2.value(0).toString();
    }

    casa="select estado, municipio, calle, no_interior, no_exterior, cp from direccion where direccion.idDireccion= '"+ id +"'";
    if (casas.exec(casa)){
        while (casas.next()){
            estado = casas.value(0).toString();
            municipio = casas.value(1).toString();
            calle = casas.value(2).toString();
            no_interior = casas.value(3).toString();
            no_exterior = casas.value(4).toString();
            cp = casas.value(5).toString();

            ui->estadoRes_2->setText(estado);
            ui->municipioRes_2->setText(municipio);
            ui->calleRes_2->setText(calle);
            ui->NIRes_2->setText(no_interior);
            ui->NERes_2->setText(no_exterior);
            ui->CPRes_2->setText(cp);

        }
    }

}
void MainWindow::Reservacion_actviva(){
    QSqlQuery catalogo,sacarfoto;
    QString catalogosql, titulo, llamarFoto, idP;
    QPixmap imagen;
    QIcon imagenreal;
    QByteArray img;

    idP="select idPropietario from Propietario where Propietario.usuario= '" +username+ " '; ";
    QSqlQuery idPropi;
    idPropi.prepare(idP);
    idPropi.exec();
    while ( idPropi.next()){
        idP=idPropi.value(0).toString();
    }

    qDebug()<< idP;

    catalogosql = "select nombre from Casa where casa.idPropietario= '"+idP+"' and casa.idCatalogo='4';";
    catalogo.exec(catalogosql);
    int counterf = 0;
    int counterc = 0;
    qDebug()<< catalogosql;

    while (catalogo.next()){
        titulo = catalogo.value(0).toString();
        llamarFoto="call CatalogoC('"+titulo+"');";
        sacarfoto.exec(llamarFoto);
        sacarfoto.next();
        img=sacarfoto.value(0).toByteArray();
        imagen.loadFromData(img);

        QLabel *label = new QLabel;
        label->setPixmap(imagen.scaled(200, 125));
        label->setMaximumWidth(200);
        label->setMaximumHeight(125);
        ui->layout_2->addWidget(label, counterf, counterc,1,1);

        counterc++;

        if (counterc == 3){
            counterf= counterf + 2;
            counterc = 0;
        }
    }
}

//--------------------------------------Interfaces del Propietario ----------------------------------

//Datos del propietario
void MainWindow::Datos(){
   QString dato, nombre, a_paterno, a_materno, telefono, correo;
    QSqlQuery query;
    dato="select nombre, a_paterno, a_materno, telefono, correo, usuario from Propietario where usuario='"+username+"'";
    if (query.exec(dato)){
        while (query.next()){
            nombre = query.value(0).toString();
            a_paterno = query.value(1).toString();
            a_materno = query.value(2).toString();
            telefono = query.value(3).toString();
            correo = query.value(4).toString();

            ui->nom_2->setText(nombre);
            ui->apellido_2->setText(a_paterno);
            ui->apellido_3->setText(a_materno);
            ui->tel_2->setText(telefono);
            ui->correo_2->setText(correo);
        }
        ui->stackedWidget->setCurrentIndex(14);
    }
}


void MainWindow::DatosDireccion(){
    QString estado, municipio, calle, colonia, no_interior, no_exterior, cp, direccion, Propi, id, id2;
    QSqlQuery query;

    id= "select MAX(idDireccion) from direccion";

    QSqlQuery insert2;
    insert2.prepare(id);
    insert2.exec();
    while ( insert2.next()){
        id=insert2.value(0).toString();
    }

    id2="select idPropietario from Propietario where Propietario.usuario= '"+username+"'";
    qDebug()<<id2;
    QSqlQuery insert;
    insert.prepare(id2);
    insert.exec();
    while ( insert.next()){
        id2=insert.value(0).toString();
    }

    qDebug()<<id2;


    Propi= "select d.estado, d.municipio, d.calle, d.colonia, d.no_interior, d.no_exterior, d.cp, p.idDireccion from Direccion as d, Propietario as p where d.idDireccion=p.idDireccion and p.idPropietario='"+id2+ "';";
    qDebug()<<Propi;

    if (query.exec(Propi)){
          while (query.next()){
              estado=query.value(0).toString();
              municipio=query.value(1).toString();
              calle=query.value(2).toString();
              colonia=query.value(3).toString();
              no_interior=query.value(4).toString();
              no_exterior=query.value(5).toString();
              cp=query.value(6).toString();

              ui->estado_5->setText(estado);
              ui->municipio_2->setText(municipio);
              ui->calle_4->setText(calle);
              ui->colonia_4->setText(colonia);
              ui->no_interior_3->setText(no_interior);
              ui->no_exterior_3->setText(no_exterior);
              ui->cp_4->setText(cp);
          }
           ui->stackedWidget->setCurrentIndex(14);
     }
}
void MainWindow::DatosFoto(){
    QPixmap imagen;

    QIcon imagenreal;
    QByteArray img;
    QSqlQuery sql, sacarfoto;

    QString insertfoto, idF, propi, llamarFoto;
    llamarFoto="call FotoPropi('"+username+"');" ;


    sacarfoto.exec(llamarFoto);
    sacarfoto.next();
    img=sacarfoto.value(0).toByteArray();
    imagen.loadFromData(img);
    ui->label_28->setPixmap(imagen.scaled(171,141));

}


void MainWindow::on_RCat_clicked(){
    ui->stackedWidget->setCurrentIndex(10);
}

//Catalogo Casa Registradas
void MainWindow::on_home_8_clicked(){
    clearLayout(ui->layout);
    CatalogoPrincipal();
    ui->stackedWidget->setCurrentIndex(10);
}
void MainWindow::on_add_8_clicked(){
    ui->stackedWidget->setCurrentIndex(12);
}
void MainWindow::on_reservaciones_8_clicked(){
    ui->stackedWidget->setCurrentIndex(13);
    Reservacion_actviva();
}
void MainWindow::on_ajustes_8_clicked(){

    Datos();
    DatosDireccion();
    DatosFoto();
    ui->stackedWidget->setCurrentIndex(14);
}
void MainWindow::on_salir_8_clicked(){
    clearLayout(ui->catalodo_propi);
    ui->estado_5->clear();
    ui->municipio_2->clear();
    ui->calle_4->clear();
    ui->colonia_4->clear();
    ui->no_interior_3->clear();
    ui->no_exterior_3->clear();
    ui->cp_4->clear();
    ui->stackedWidget->setCurrentIndex(0);
}

//Pueblicar, Eliminar, Registrar casa
void MainWindow::on_home_9_clicked(){
    clearLayout(ui->layout);
    ui->stackedWidget->setCurrentIndex(10);
}
void MainWindow::on_add_9_clicked(){
    ui->stackedWidget->setCurrentIndex(13);
}
void MainWindow::on_reservaciones_9_clicked(){
     Reservacion_actviva();
    ui->stackedWidget->setCurrentIndex(13);
}
void MainWindow::on_ajustes_9_clicked(){
    Datos();
    DatosDireccion();
    DatosFoto();
    ui->stackedWidget->setCurrentIndex(14);
}
void MainWindow::on_salir_9_clicked(){
    clearLayout(ui->catalodo_propi);
    ui->estado_5->clear();
    ui->municipio_2->clear();
    ui->calle_4->clear();
    ui->colonia_4->clear();
    ui->no_interior_3->clear();
    ui->no_exterior_3->clear();
    ui->cp_4->clear();
    ui->stackedWidget->setCurrentIndex(0);
}

//Agregar casa
void MainWindow::on_AddCasa_clicked(){
    QString desc, servicios, nombre, estado,municipio, calle, colonia, no_interior, no_exterior, cp, habitaciones, banos, precio, foto;
    QByteArray imagen;
    QSqlQuery sql;
    QString direccion, casa, id, idP, idF;
    desc= ui->plainTextEdit_3->toPlainText();
    servicios= ui->plainTextEdit_2->toPlainText();
    nombre= ui->nom_casa_2->text();
    estado= ui->estado_3->text();
    municipio= ui->municipio->text();
    calle= ui->calle_2->text();
    colonia= ui->colonia_2->text();
    no_interior= ui->no_interior->text();
    no_exterior= ui->no_exterior->text();
    cp=ui->cp_2->text();
    habitaciones=ui->no_habitaciones->text();
    banos= ui->banos_2->text();
    precio= ui->precio_3->text();

    direccion= "insert into direccion(estado, municipio, calle, colonia, no_interior, no_exterior, cp) "
               "values('"+estado+ "' , '" + municipio + "' , '" +calle+ "' , '"+colonia+ "' , '" +no_interior+ "' , '" +no_exterior+"' , '"+cp+"')";

    QSqlQuery insert;
    insert.prepare(direccion);
    insert.exec();

    id= "select MAX(idDireccion) from direccion";

    QSqlQuery insert2;
    insert2.prepare(id);
    insert2.exec();
    while ( insert2.next()){
        id=insert2.value(0).toString();
        qDebug()<<id;
    }

    idP="select idPropietario from Propietario where Propietario.usuario= '"+username+"'";
    QSqlQuery idPropi;
    idPropi.prepare(idP);
    idPropi.exec();
    while ( idPropi.next()){
        idP=idPropi.value(0).toString();
    }

    idF="select MAX(idFotos) from fotos";
    QSqlQuery insert3;
    insert3.prepare(idF);
    insert3.exec();
    while ( insert3.next()){
        idF=insert3.value(0).toString();
        qDebug()<<idF;
    }

    casa= "insert into casa(nombre, no_habitaciones, no_banos, precio, descripcion, servicios, idCatalogo, idDireccion, idPropietario, idFotos) "
          "values('" + nombre + "' , '" +habitaciones+ "' , '" +banos+ "' , '" +precio+ "' , '" +desc+"' , '" +servicios+ "' , '1' , '" +id+ " ', '"+idP+" ', '"+idF+"')";

    qDebug()<<casa;
    QSqlQuery insertcasa1;
    insertcasa1.prepare(casa);
    insertcasa1.exec();

    ui->plainTextEdit_3->clear();
    ui->plainTextEdit_2->clear();
    ui->nom_casa_2->clear();
    ui->estado_3->clear();
    ui->municipio->clear();
    ui->calle_2->clear();
    ui->colonia_2->clear();
    ui->no_interior->clear();
    ui->no_exterior->clear();
    ui->cp_2->clear();
    ui->no_habitaciones->clear();
    ui->banos_2->clear();
    ui->precio_3->clear();
}
void MainWindow::on_addFoto_clicked(){
    QString idP, casa, idF,insertfoto, idFoto, casa2;
    auto nombreArchivo=QFileDialog::getOpenFileName(this,"abrir imagen",QDir::rootPath(),"imagenes(*.png *.jpg *.jpeg);; cualquier archivo(*.*)");
    ruta=nombreArchivo;
    QFile archivo(ruta);
    if(!archivo.open(QIODevice::ReadOnly)){
        return;
    }

    QByteArray mostrar=archivo.readAll();
    QPixmap imag;
    imag.loadFromData(mostrar);
    ui->img_casa->setPixmap(imag.scaled(181,161));
    ruta1=ruta;
    imagen1=mostrar;

    insertfoto="insert into fotos(ruta, imagen)values('"+ruta1+"', :datos);" ;

    QSqlQuery insertfoto1;
    insertfoto1.prepare(insertfoto);
    insertfoto1.bindValue(":datos",imagen1);
    insertfoto1.exec();
    connect(ui->AddCasa, SIGNAL(clicked()), ui->img_casa, SLOT(clear()));
    qDebug()<<insertfoto;


}
void MainWindow::on_home_10_clicked(){
    ui->stackedWidget->setCurrentIndex(10);
}
void MainWindow::on_add_10_clicked(){
    ui->stackedWidget->setCurrentIndex(12);
}
void MainWindow::on_reservaciones_10_clicked(){
     Reservacion_actviva();
    ui->stackedWidget->setCurrentIndex(13);
}
void MainWindow::on_ajustes_10_clicked(){
    Datos();
    DatosDireccion();
    DatosFoto();
    ui->stackedWidget->setCurrentIndex(14);
}
void MainWindow::on_salir_10_clicked(){
    clearLayout(ui->catalodo_propi);
    ui->estado_5->clear();
    ui->municipio_2->clear();
    ui->calle_4->clear();
    ui->colonia_4->clear();
    ui->no_interior_3->clear();
    ui->no_exterior_3->clear();
    ui->cp_4->clear();
    ui->stackedWidget->setCurrentIndex(0);
}

//Reservaciones activas
void MainWindow::on_home_11_clicked(){
    ui->stackedWidget->setCurrentIndex(10);
}
void MainWindow::on_add_11_clicked(){
   ui->stackedWidget->setCurrentIndex(12);
}
void MainWindow::on_reservaciones_11_clicked(){
     Reservacion_actviva();
    ui->stackedWidget->setCurrentIndex(13);
}
void MainWindow::on_ajustes_11_clicked(){
    Datos();
    DatosDireccion();
     DatosFoto();
    ui->stackedWidget->setCurrentIndex(14);
}
void MainWindow::on_salir_11_clicked(){
    clearLayout(ui->catalodo_propi);
    ui->estado_5->clear();
    ui->municipio_2->clear();
    ui->calle_4->clear();
    ui->colonia_4->clear();
    ui->no_interior_3->clear();
    ui->no_exterior_3->clear();
    ui->cp_4->clear();
    ui->stackedWidget->setCurrentIndex(0);
}

//Cambiar datos del perfil del propietario
void MainWindow::on_home_13_clicked(){
    ui->stackedWidget->setCurrentIndex(10);
}
void MainWindow::on_add_13_clicked(){
    ui->stackedWidget->setCurrentIndex(12);
}
void MainWindow::on_reservaciones_13_clicked(){
     Reservacion_actviva();
    ui->stackedWidget->setCurrentIndex(13);
}
void MainWindow::on_ajustes_13_clicked(){
    Datos();
    DatosDireccion();
    DatosFoto();
    ui->stackedWidget->setCurrentIndex(14);
}
void MainWindow::on_salir_13_clicked(){
    ui->estado_5->clear();
    ui->municipio_2->clear();
    ui->calle_4->clear();
    ui->colonia_4->clear();
    ui->no_interior_3->clear();
    ui->no_exterior_3->clear();
    ui->cp_4->clear();
    ui->stackedWidget->setCurrentIndex(0);
}
void MainWindow::on_CambiarFoto_2_clicked(){
    QPixmap imagen;
    QIcon imagenreal;
    QByteArray img;
    QSqlQuery sql, sacarfoto;
    QString insertfoto, idF, propi, llamarFoto;
    auto Archivo=QFileDialog::getOpenFileName(this,"abrir imagen",QDir::rootPath(),"imagenes(*.png *.jpg *.jpeg);; cualquier archivo(*.*)");
    ruta2= Archivo;
    QFile archivo(ruta2);
    if(!archivo.open(QIODevice::ReadOnly)){
        return;
    }

    QByteArray mostrar = archivo.readAll();
    QPixmap imag;
    imag.loadFromData(mostrar);
    ui->label_28->setPixmap(imag);
    ruta3=ruta2;
    imagen2=mostrar;

    insertfoto="insert into fotos(ruta, imagen)values('"+ruta3+"', :datos);" ;
    QSqlQuery insertfoto1;
      insertfoto1.prepare(insertfoto);
      insertfoto1.bindValue(":datos",imagen2);
      insertfoto1.exec();

      idF="select MAX(idFotos) from fotos";
      QSqlQuery insert3;
      insert3.prepare(idF);
      insert3.exec();
      while ( insert3.next()){
          idF=insert3.value(0).toString();
          qDebug()<<"id de la foto "+idF;
      }

      propi="update Propietario set idFotos= '"+ idF +"' where Propietario.usuario='"+username+"';";

      if(sql.exec(propi)){
          DatosFoto();
          QMessageBox::information(this, "Foto", "Foto Guardada");
      }
}
void MainWindow::on_aceptar_9_clicked(){
    QString estado, municipio, calle, colonia, no_interior, no_exterior, cp, direccion, id, casa, idP;
    QSqlQuery query;
    estado= ui->direccion_11->text();
    municipio= ui->direccion_12->text();
    calle= ui->direccion_6->text();
    colonia= ui->direccion_7->text();
    no_interior= ui->direccion_8->text();
    no_exterior= ui->direccion_9->text();
    cp= ui->direccion_10->text();

    direccion= "insert into direccion(estado, municipio, calle, colonia, no_interior, no_exterior, cp) "
    "values('"+estado+ "' , '" + municipio + "' , '" +calle+ "' , '"+colonia+ "' , '" +no_interior+ "' , '" +no_exterior+"' , '"+cp+"')";

    QSqlQuery insert;
    insert.prepare(direccion);
    insert.exec();

    id= "select MAX(idDireccion) from direccion";

    QSqlQuery insert2;
    insert2.prepare(id);
    insert2.exec();
    while ( insert2.next()){
        id=insert2.value(0).toString();
        qDebug()<<"id de la direccion"+id;
    }

   casa="update Propietario set idDireccion= '"+ id +"' where Propietario.usuario='"+username+"'";

   if (query.exec(casa)){
           ui->direccion_6->clear();
           ui->direccion_7->clear();
           ui->direccion_8->clear();
           ui->direccion_9->clear();
           ui->direccion_10->clear();
           ui->direccion_11->clear();
           ui->direccion_12->clear();
           Datos();
           DatosDireccion();
           QMessageBox::about(this,QObject::tr( "Propietario"), QObject::tr("Datos Actualizados"));
   }


}
void MainWindow::on_aceptar_10_clicked(){
    QString password=ui->contra2_2->text();
    QString password2 = ui -> contra_3 -> text();
    QSqlQuery query, query2;
    if (query.exec("update login set contrasena = '"+password2+"' where usuario = '"+username+"' and contrasena= '"+password+"';")){
           QMessageBox::information(this, "Cambiar contraseña", "Datos Actualizados");
    }
    ui->contra2_2->clear();
    ui -> contra_3 -> clear();
}


void MainWindow::on_aceptar_11_clicked(){

}


//-------------------------------------- Interfaces del Cliente ----------------------------------

void MainWindow::DatosC(){
    QString dato, nombre, a_paterno, a_materno, telefono, correo;
    QSqlQuery query;
    dato="select nombre, a_paterno, a_materno, telefono, correo, usuario from Cliente where usuario='"+username+"'";
    if (query.exec(dato)){
        while (query.next()){
            nombre = query.value(0).toString();
            a_paterno = query.value(1).toString();
            a_materno = query.value(2).toString();
            telefono = query.value(3).toString();
            correo = query.value(4).toString();

            ui->nom->setText(nombre);
            ui->apellido->setText(a_paterno);
            ui->apellido_4->setText(a_materno);
            ui->tel->setText(telefono);
            ui->correo->setText(correo);
        }
        ui->stackedWidget->setCurrentIndex(7);
    }
    else{
        QMessageBox::information(this, "error", "error");
    }
}

void MainWindow::DatosClienteDireccion(){
    QString estado, municipio, calle, colonia, no_interior, no_exterior, cp, direccion, Propi, id, id2;
    QSqlQuery query;

    id= "select MAX(idDireccion) from direccion";

    QSqlQuery insert2;
    insert2.prepare(id);
    insert2.exec();
    while ( insert2.next()){
        id=insert2.value(0).toString();
    }

    id2="select idCliente from Cliente where cliente.usuario= '"+username+"'";
    QSqlQuery insert;
    insert.prepare(id2);
    insert.exec();
    while ( insert.next()){
        id2=insert.value(0).toString();
    }

    Propi="select d.estado, d.municipio, d.calle, d.colonia, d.no_interior, d.no_exterior, d.cp, c.idDireccion from Direccion as d, Cliente as c where d.idDireccion=c.idDireccion and idCliente='"+id2+"'";
     if (query.exec(Propi)){
          while (query.next()){
              estado=query.value(0).toString();
              municipio=query.value(1).toString();
              calle=query.value(2).toString();
              colonia=query.value(3).toString();
              no_interior=query.value(4).toString();
              no_exterior=query.value(5).toString();
              cp=query.value(6).toString();

              ui->Estado->setText(estado);
              ui->Municipio->setText(municipio);
              ui->Calle->setText(calle);
              ui->Colonia->setText(colonia);
              ui->direc_5->setText(no_interior);
              ui->direc_6->setText(no_exterior);
              ui->direc_7->setText(cp);
          }
           ui->stackedWidget->setCurrentIndex(7);
     }


}




void MainWindow::DatosClienteFoto(){
    QPixmap imagen;
    QIcon imagenreal;
    QByteArray img;
    QSqlQuery sql, sacarfoto;

    QString insertfoto, idF, propi, llamarFoto;
    llamarFoto="call FotoCliente('"+username+"');" ;


    sacarfoto.exec(llamarFoto);
    sacarfoto.next();
    img=sacarfoto.value(0).toByteArray();
    imagen.loadFromData(img);
    ui->fotouser->setPixmap(imagen.scaled(191,181));
}
void MainWindow::Reservar(){

}

//Catalogo Principal
void MainWindow::on_home_14_clicked(){
    clearLayout(ui->layout);
    CatalogoPrincipal();
    ui->stackedWidget->setCurrentIndex(3);
}
void MainWindow::on_perfil_14_clicked(){
    DatosC();
    DatosClienteDireccion();
    DatosClienteFoto();
     ui->stackedWidget->setCurrentIndex(7);
}
void MainWindow::on_reservaciones_14_clicked(){
    clearLayout(ui->res_activas);
    CatalogoReservacion();
    ui->stackedWidget->setCurrentIndex(8);
}
void MainWindow::on_salir_14_clicked(){
    ui->Estado->clear();
    ui->Municipio->clear();
    ui->Calle->clear();
    ui->Colonia->clear();
    ui->direc_5->clear();
    ui->direc_6->clear();
    ui->direc_7->clear();

    ui->stackedWidget->setCurrentIndex(0);
}

//Resultado de la busqueda
void MainWindow::on_home_12_clicked(){
    clearLayout(ui->layout);
    CatalogoPrincipal();
     ui->stackedWidget->setCurrentIndex(3);
}
void MainWindow::on_perfil_12_clicked(){
    DatosC();
    DatosClienteDireccion();
    DatosClienteFoto();
    ui->stackedWidget->setCurrentIndex(7);
}
void MainWindow::on_reservaciones_12_clicked(){
    clearLayout(ui->res_activas);
    CatalogoReservacion();
    ui->stackedWidget->setCurrentIndex(8);
}
void MainWindow::on_salir_12_clicked(){
    ui->Estado->clear();
    ui->Municipio->clear();
    ui->Calle->clear();
    ui->Colonia->clear();
    ui->direc_5->clear();
    ui->direc_6->clear();
    ui->direc_7->clear();
    ui->stackedWidget->setCurrentIndex(0);
}

//Reservar
void MainWindow::on_home_4_clicked(){
    clearLayout(ui->layout);
    CatalogoPrincipal();
     ui->stackedWidget->setCurrentIndex(3);
}
void MainWindow::on_perfil_4_clicked(){
    DatosC();
    DatosClienteDireccion();
    DatosClienteFoto();
    ui->stackedWidget->setCurrentIndex(7);
}
void MainWindow::on_reservaciones_4_clicked(){
    ui->stackedWidget->setCurrentIndex(8);
}
void MainWindow::on_salir_4_clicked(){
    ui->stackedWidget->setCurrentIndex(0);
}
void MainWindow::on_volver_clicked(){
    ui->stackedWidget->setCurrentIndex(3);
}
void MainWindow::Pago(){
    QString reservacion, id, precio, casa, FE, FS, casa1, casa2, idF, fecha;
    QSqlQuery sql;
    casa=ui->Nom->text();

    precio="select idCasa from casa where casa.nombre= '"+casa+"'";
    QSqlQuery insert2;
    insert2.prepare(precio);
    insert2.exec();
    while ( insert2.next()){
        precio=insert2.value(0).toString();
    }
    FE=ui->FechaEntrada->text();
    FS=ui->FechaSalida->text();

    qDebug()<<FE;
    qDebug()<<FS;

    id="select idCliente from Cliente where Cliente.usuario='"+username+"';";
    QSqlQuery insert3;
    insert3.prepare(id);
    insert3.exec();
    while ( insert3.next()){
        id=insert3.value(0).toString();
    }
    reservacion="insert into reservacion(fecha_entrada, fecha_salida, idCasa, idCliente) values('" +FE+ "','" +FS+"','"+precio+ "','"+id+"')";
    qDebug()<<reservacion;

    QSqlQuery insertreservacion;
    insertreservacion.prepare(reservacion);
    insertreservacion.exec();
}


void MainWindow::on_enviar_clicked(){
    Pago();
    QString pago, casa2, casa, id, id1, id2, monto, nom;
    pago =ui->PagoTE->currentText();
    nom=ui->Nom->text();


    if(pago=="Tarjeta"){
        Pago();
        id1="select MAX(idReservacion) from reservacion";
        QSqlQuery insert4;
        insert4.prepare(id1);
        insert4.exec();
        while ( insert4.next()){
            id1=insert4.value(0).toString();

        }

       id="select idCasa from casa where casa.nombre= '"+nom+"'";
       QSqlQuery insert3;
       insert3.prepare(id);
       insert3.exec();
       while ( insert3.next()){
           id=insert3.value(0).toString();
       }

       id2="select precio from casa where casa.idCasa= '"+id+"'";
       QSqlQuery insert;
       insert.prepare(id2);
       insert.exec();
       while ( insert.next()){
           id2=insert.value(0).toString();
       }



       casa2="select c.precio, sum(datediff(r.fecha_salida, r.fecha_entrada) * "+id2+")  as  total "
             " from Casa as c inner join reservacion as r "
             " where r.idCasa= '"+id+"' and c.idCasa='"+id+"'  and r.idReservacion='"+id1+"';";

       qDebug()<< casa2;
       QSqlQuery insert9;
       insert9.prepare(casa2);
       insert9.exec();
       while (insert9.next()){
           casa2=insert9.value(0).toString();
           qDebug()<< casa2;
       }

       monto= casa2;
       ui->monto->setText("$ "+monto+" MNX");
       ui->stackedWidget->setCurrentIndex(6);

    }
    else{
        Pago();
        id1="select MAX(idReservacion) from reservacion";
        QSqlQuery insert4;
        insert4.prepare(id1);
        insert4.exec();
        while ( insert4.next()){
            id1=insert4.value(0).toString();

        }

       id="select idCasa from casa where casa.nombre= '"+nom+"'";
       QSqlQuery insert3;
       insert3.prepare(id);
       insert3.exec();
       while ( insert3.next()){
           id=insert3.value(0).toString();
       }

       id2="select precio from casa where casa.idCasa= '"+id+"'";
       QSqlQuery insert;
       insert.prepare(id2);
       insert.exec();
       while ( insert.next()){
           id2=insert.value(0).toString();
       }



       casa2="select c.precio, sum(datediff(r.fecha_salida, r.fecha_entrada) * "+id2+")  as  total "
             " from Casa as c inner join reservacion as r "
             " where r.idCasa= '"+id+"' and c.idCasa='"+id+"'  and r.idReservacion='"+id1+"';";

       qDebug()<< casa2;
       QSqlQuery insert9;
       insert9.prepare(casa2);
       insert9.exec();
       while (insert9.next()){
           casa2=insert9.value(0).toString();
           qDebug()<< casa2;
       }

       monto= casa2;
       ui->monto_2->setText("$ "+monto+" MNX");
       ui->stackedWidget->setCurrentIndex(6);

       ui->stackedWidget->setCurrentIndex(20);
    }

}
void MainWindow::on_abrir_2_clicked(){
    QString format = ".pdf";
    QString filename = QFileDialog::getSaveFileName(this,
            "Elija el nombre", QDir::homePath(),"*.pdf");

    filename.remove(format);
    filename+=format;
    ui->ruta_2->setText(filename);
}
void MainWindow::on_generac_clicked(){
    QString nom;
    nom= ui->ruta_2->text();

    QString html ="<div align=right> 17 de Junio del 2020 </div>"
                  "<h1 align=center>CONTRATO DE ARRENDAMIENTO</h1> "
                  "<p align=justify>"
                  "<h2>I. ANTECEDENTES</h2>"
                  "<div align=left>"
                  "<p style='font-size:120%;'> I.1. Las Partes.- Las Partes son personas físicas (de ser el caso), de nacionalidad mexicana, las cuales desean establecer una relación de Arrendamiento respecto a cierto inmueble ubicado en la Ciudad de México, México, descrito en mayor detalle en el párrafo II.1.6 de éste Contrato.</p>"
                  "<p style='font-size:120%;'> I.2 Voluntad de las Partes.- El Arrendador es propietario del inmueble, el cual desea rentar acorde a las leyes de los Estados Unidos Mexicanos, y en los términos y las condiciones estipulados en el presente Contrato, mientras que el Arrendatario desea rentar ese inmueble de la misma manera acorde a las leyes de los Estados Unidos Mexicanos, y sujeto a los términos y condiciones estipulados en éste Contrato. </p>"
                  "<h2>II. DEFINICIONES </h2>"
                  "<p style='font-size:120%;'> II.1. Términos en Negritas.- Las Partes acuerdan que los términos que aparecen en el texto del Contrato en negritas –y todas sus variaciones gramaticales y de género - tienen el significado asignado a continuación y, de ser el caso, el ampliado en el párrafo correspondiente, que así los entienden y que las definiciones son partes íntegras al contenido jurídico del Contrato, siendo parte de los términos, condiciones y obligaciones del mismo: </p>"
                  "<dd style='font-size:120%;'> II.1.2.  “Arrendador”.- se refiere a la persona física o moral quien es el/la único/única y legítimo/legítima propietario/propietaria del inmueble, según consta en escritura pública. </dd>"
                  "<br><dd style='font-size:120%;'> II.1.3. “Arrendamiento”.- se refiere al acuerdo en el que un arrendador  conviene con un arrendatario en percibir una suma única de dinero – o una serie de pagos o cuotas – por cederle el derecho a usar un inmueble durante un período de tiempo determinado, bajo ciertos términos y condiciones y acorde a las estipulaciones de la ley.  </dd>"
                  "<br><dd style='font-size:120%;'>II.1.4. “Arrendatario”.- se refiere a la persona física que desea rentar por cierto plazo, a la renta estipulada, y bajo las condiciones y términos de éste contrato, al inmueble.</dd>"
                  "<br><dd style='font-size:120%;'>II.1.5. “Condominio”.- se refiere a las unidades – “condominios” - dentro de un mismo edificio, sobre las cuales existe un derecho singular de propiedad que permite un aprovechamiento individual, y las cuales cuentan con una salida propia a la vía pública o a un elemento común del edificio. A los dueños de los condominios, se les refiere como “condóminos.”</dd>"
                  "<br><dd style='font-size:120%;'>II.1.6. “Contrato”.- se refiere a éste Contrato Privado de Arrendamiento y todos sus Anexos (si los hubiere).</dd>"
                  "<br><dd style='font-size:120%;'>II.1.7. “Fiador”.- se refiere a la persona física, la cual por un lado, es ajena a la obligación principal de éste Contrato, pero la cual por otro lado, voluntariamente garantiza y se hace responsable de su cumplimiento, comprometiéndose a cumplir lo que el Arrendatario no cumpla por sí mismo.</dd>"
                  "<br><dd style='font-size:120%;'>II.1.8. “Fuerza Mayor”.- se refiere a un evento imprevisto difícil de resistir y de preverse o advertirse, y fuera del control de las Partes, tal como un terremoto, tsunami, acto de gobierno, etc., y que por lo mismo, en general, libera a todas las Partes y al Fiador de sus obligaciones bajo el Contrato.</dd>"
                  "<br><dd style='font-size:120%;'>II.1.9. “Inmueble”.- se refiere a esa cierta propiedad ubicada en _________________ en la Ciudad de México, en la Colonia ___________, Delegación ____________, Código Postal ___________.  (Anexo I a éste Contrato), consta como propietario único y legítimo, el Arrendador.</dd>"
                  "<br><dd style='font-size:120%;'>II.1.10. “Partes”.- se refiere en conjunto a todos los signatarios de éste Contrato. En forma singular se refiere al Arrendador o al Arrendatario, acorde al caso. </dd>"
                  "<br><dd style='font-size:120%;'>II.1.11. “Régimen de Condominio”.- se refiere al conjunto de normas que gobiernan/reglamentan la convivencia de los condóminos y/o sus inquilinos en un edificio de condominios.</dd>"
                  "<br><dd style='font-size:120%;'>II.1.12.  “Reparaciones Estructurales”.- se refiere a aquellas reparaciones que deberá efectuar el Arrendador - o que efectúe el Arrendatario a cuenta del Arrendador - por daños causados a la estructura del inmueble por causas de fuerza mayor, y que no sean causa de desperfectos causados por el mal uso del inmueble por parte del Arrendatario. Cualquier monto que erogue el Arrendatario en Reparaciones Estructurales, lo podrá desquitar contra la renta, siempre y cuando presente los recibos correspondientes.</dd>";

    QTextDocument document;
    document.setHtml(html);

    QPrinter printer(QPrinter::PrinterResolution);
    printer.setOutputFormat(QPrinter::PdfFormat);
    printer.setPaperSize(QPrinter::A4);
    printer.setOutputFileName(nom);
    printer.setPageMargins(QMarginsF(15, 15, 15, 15));

    document.print(&printer);

    ui->comboBox_2->clear();
    ui->pago_tar->clear();
    ui->pago_tar_clave->clear();
    ui->Fecha_Tar->clear();
    ui->Fecha_Tar_2->clear();
    ui->titulo->clear();
    ui->ruta_2->clear();

    QMessageBox::about(this,"contrato","Contrato Generado");

}
void MainWindow::on_Regresar_clicked(){
    ui->stackedWidget->setCurrentIndex(5);
}


//Contrato
void MainWindow::on_home_5_clicked(){

    clearLayout(ui->layout);
    CatalogoPrincipal();
     ui->stackedWidget->setCurrentIndex(3);
}
void MainWindow::on_perfil_5_clicked(){
    DatosC();
    DatosClienteDireccion();
    DatosClienteFoto();
    ui->stackedWidget->setCurrentIndex(7);
}
void MainWindow::on_reservaciones_5_clicked(){
    clearLayout(ui->res_activas);
    CatalogoReservacion();
    ui->stackedWidget->setCurrentIndex(8);
}
void MainWindow::on_salir_5_clicked(){
    ui->Estado->clear();
    ui->Municipio->clear();
    ui->Calle->clear();
    ui->Colonia->clear();
    ui->direc_5->clear();
    ui->direc_6->clear();
    ui->direc_7->clear();
    ui->stackedWidget->setCurrentIndex(0);
}
void MainWindow::on_tar_aceptar_clicked(){
    QString banco, nt, ns, mes, agnio, cuenta, id, id1, id2, casa, titulo,Cantidad, tot;

    banco=ui->comboBox_2->currentText();
    nt=ui->pago_tar->text();
    ns=ui->pago_tar_clave->text();
    mes=ui->Fecha_Tar->text();
    agnio=ui->Fecha_Tar_2->text();
    titulo=ui->titulo->text();
    id="select idCliente from cliente where cliente.usuario= '"+username+"'";
    QSqlQuery insert2, cliente;
    insert2.prepare(id);
    insert2.exec();
    while ( insert2.next()){
        id=insert2.value(0).toString();
    }
    cuenta="insert into tarjeta(no_tarjeta, codigo, mes, agnio, tipo_tarjeta, idCliente) values('"+nt+"','"+ns+"','"+mes+"','"+agnio+"','"+banco+"','"+id+"')";
    cliente.prepare(cuenta);
    cliente.exec();
    cliente.next();

    id1="select idCliente from Cliente where cliente.usuario= '"+username+"' ";
    QSqlQuery insert4;
    insert4.prepare(id1);
    insert4.exec();
    while ( insert4.next()){
        id1=insert4.value(0).toString();
    }

    id2="select idCasa from Casa where Casa.nombre= '"+titulo+"' ";
    QSqlQuery insert;
    insert.prepare(id2);
    insert.exec();
    while ( insert.next()){
        id2=insert.value(0).toString();
    }

    qDebug()<<id2;

    casa="update Casa set idCatalogo = '4', idCliente='" +id1+ "' where casa.idCasa= '"+id2+"'";
    QSqlQuery query, query2;
    if (query.exec(casa)){
           QMessageBox::about(this, "Pago", "Pago exitoso");
    }
}
void MainWindow::on_regresar_3_clicked(){
 ui->stackedWidget->setCurrentIndex(5);
}
void MainWindow::on_pushButton_clicked(){
    CatalogoPrincipal();
    ui->stackedWidget->setCurrentIndex(3);
}
void MainWindow::on_abrir_clicked(){
    QString format = ".pdf";
    QString filename = QFileDialog::getSaveFileName(this,
            "Elija el nombre", QDir::homePath(),"*.pdf");

    filename.remove(format);
    filename+=format;
    ui->ruta->setText(filename);
}

void MainWindow::on_descargar_clicked(){
    QImage cinco( ":/Recursos/codigo.png" );
    QString nom, casa, precio;
    casa=ui->label_85->text();
    precio=ui->monto_2->text();
    nom= ui->ruta->text();

    QPainter paint;

    QString html ="<div align=right> 17 de Junio del 2020 </div>"
                  "<h1 align=center>FICHA DE PAGO</h1>"
                  "<p align=justify>"
                  "<div align=left>"
                  "<h1>Detalles de la orden</h1>"
                  "<h2>Nombre de la casa: "+casa+"</h2>"
                  "<h2>Precio:"+precio+" </h2>"
                  "<h2>Sucursales donde puede pagar</h2>"
                  "<ul>"
                  "<li style='font-size:120%;'>OXXO</li>"
                  "<p style='font-size:120%;'>Referencia[9400-0007-3779-87]</p>"
                  "<li style='font-size:120%;'>7-Eleven</li>"
                  "<p style='font-size:120%;'>Referencia[9300-0007-3779-87]</p>"
                  "<li style='font-size:120%;'>Soriana</li>"
                  "<p style='font-size:120%;'>Referencia[9200-0007-3779-87]</p>"
                  "<li style='font-size:120%;'>Telecomm</li>"
                  "<p style='font-size:120%;'>Referencia[9900-0007-3779-87]</p>"
                  "</ul>"
                  "</div>";

    QTextDocument document;
    document.setHtml(html);

    QPrinter printer(QPrinter::PrinterResolution);
    printer.setOutputFormat(QPrinter::PdfFormat);
    printer.setPaperSize(QPrinter::A4);
    printer.setOutputFileName(nom);
    printer.setPageMargins(QMarginsF(15, 15, 15, 15));

    QMessageBox::about(this,"Pago","Ficha de Pago Guardada");
    document.print(&printer);


    QString id1="select idCliente from Cliente where cliente.usuario= '"+username+"' ";
    QSqlQuery insert4;
    insert4.prepare(id1);
    insert4.exec();
    while ( insert4.next()){
        id1=insert4.value(0).toString();
    }

     QString id2="select idCasa from Casa where Casa.nombre= '"+casa+"' ";
    QSqlQuery insert;
    insert.prepare(id2);
    insert.exec();
    while ( insert.next()){
        id2=insert.value(0).toString();
    }


    casa="update Casa set idCatalogo = '4', idCliente='" +id1+ "' where casa.idCasa= '"+id2+"'";
    QSqlQuery query, query2;
    if (query.exec(casa)){
        ui->ruta->clear();
    }

}


//Perfil
void MainWindow::on_home_6_clicked(){
    clearLayout(ui->layout);
    CatalogoPrincipal();
     ui->stackedWidget->setCurrentIndex(3);
}
void MainWindow::on_perfil_6_clicked(){
    DatosC();
    DatosClienteDireccion();
    DatosClienteFoto();
    ui->stackedWidget->setCurrentIndex(7);
}
void MainWindow::on_reservaciones_6_clicked(){
    clearLayout(ui->res_activas);
    CatalogoReservacion();
    ui->stackedWidget->setCurrentIndex(8);
}
void MainWindow::on_salir_6_clicked(){
    ui->Estado->clear();
    ui->Municipio->clear();
    ui->Calle->clear();
    ui->Colonia->clear();
    ui->direc_5->clear();
    ui->direc_6->clear();
    ui->direc_7->clear();
    ui->stackedWidget->setCurrentIndex(0);
}
void MainWindow::on_aceptar_2_clicked(){
    QString password=ui->contra2->text();
    QString password2 = ui -> contra-> text();
    QSqlQuery query, query2;
    if (query.exec("update login set contrasena = '"+password2+"' where usuario = '"+username+"' and contrasena= '"+password+"';")){
           QMessageBox::information(this, "Cambiar contraseña", "Datos Actualizados");
    }
    ui->contra2->clear();
    ui -> contra->clear();
}
void MainWindow::on_aceptar_clicked(){
    QString estado, municipio, calle, colonia, no_interior, no_exterior, cp, direccion, id, casa, idP;
    QSqlQuery query;
    estado= ui->direccion_13->text();
    municipio= ui->direccion_14->text();
    calle= ui->direccion->text();
    colonia= ui->direccion_2->text();
    no_interior= ui->direccion_3->text();
    no_exterior= ui->direccion_4->text();
    cp= ui->direccion_5->text();

    direccion= "insert into direccion(estado, municipio, calle, colonia, no_interior, no_exterior, cp) "
    "values('"+estado+ "' , '" + municipio + "' , '" +calle+ "' , '"+colonia+ "' , '" +no_interior+ "' , '" +no_exterior+"' , '"+cp+"')";

    QSqlQuery insert;
    insert.prepare(direccion);
    insert.exec();

    id= "select MAX(idDireccion) from direccion";

    QSqlQuery insert2;
    insert2.prepare(id);
    insert2.exec();
    while ( insert2.next()){
        id=insert2.value(0).toString();
        qDebug()<<"id de la direccion"+id;
    }

   casa="update Cliente set idDireccion= '"+ id +"' where Cliente.usuario='"+username+"'";

   if (query.exec(casa)){
           ui->direccion_13->clear();
           ui->direccion_14->clear();
           ui->direccion->clear();
           ui->direccion_2->clear();
           ui->direccion_3->clear();
           ui->direccion_4->clear();
           ui->direccion_5->clear();
           DatosC();
           DatosClienteDireccion();
           QMessageBox::information(this, "Cliente", " Datos actualizados");
   }

}
void MainWindow::on_aceptar_3_clicked(){

}

void MainWindow::on_CambiarFoto_clicked(){
    QPixmap imagen;
    QIcon imagenreal;
    QByteArray img;
    QSqlQuery sql, sacarfoto;
    QString insertfoto, idF, propi, llamarFoto;
    auto Archivo=QFileDialog::getOpenFileName(this,"abrir imagen",QDir::rootPath(),"imagenes(*.png *.jpg *.jpeg);; cualquier archivo(*.*)");
    ruta2= Archivo;
    QFile archivo(ruta2);
    if(!archivo.open(QIODevice::ReadOnly)){
        return;
    }

    QByteArray mostrar = archivo.readAll();
    QPixmap imag;
    imag.loadFromData(mostrar);
    ui->fotouser->setPixmap(imag.scaled(191,181));
    ruta3=ruta2;
    imagen2=mostrar;

    insertfoto="insert into fotos(ruta, imagen)values('"+ruta3+"', :datos);" ;
      QSqlQuery insertfoto1;
      insertfoto1.prepare(insertfoto);
      insertfoto1.bindValue(":datos",imagen2);
      insertfoto1.exec();

      idF="select MAX(idFotos) from fotos";
      QSqlQuery insert3;
      insert3.prepare(idF);
      insert3.exec();
      while ( insert3.next()){
          idF=insert3.value(0).toString();
      }

      propi="update Cliente set idFotos= '"+ idF +"' where Cliente.usuario='"+username+"';";

      if(sql.exec(propi)){
           DatosClienteFoto();
          QMessageBox::information(this, "Foto", "Foto Guardada");
      }
}

//Reservaciones
void MainWindow::on_home_7_clicked(){
    clearLayout(ui->layout);
    CatalogoPrincipal();
     ui->stackedWidget->setCurrentIndex(3);
}
void MainWindow::on_perfil_7_clicked(){
    DatosC();
    DatosClienteDireccion();
    DatosClienteFoto();
    ui->stackedWidget->setCurrentIndex(7);
}
void MainWindow::on_reservaciones_7_clicked(){
    clearLayout(ui->res_activas);
    CatalogoReservacion();
    ui->stackedWidget->setCurrentIndex(8);
}
void MainWindow::on_salir_7_clicked(){
    ui->Estado->clear();
    ui->Municipio->clear();
    ui->Calle->clear();
    ui->Colonia->clear();
    ui->direc_5->clear();
    ui->direc_6->clear();
    ui->direc_7->clear();
    ui->stackedWidget->setCurrentIndex(0);
}

//Cancelar reservacion
void MainWindow::on_aceptar_4_clicked(){

}
void MainWindow::on_regreso_clicked(){

}

//Detalles de la reservacion
void MainWindow::on_home_15_clicked(){
    clearLayout(ui->layout);
    CatalogoPrincipal();
    ui->stackedWidget->setCurrentIndex(3);
}
void MainWindow::on_perfil_8_clicked(){
    DatosC();
    DatosClienteDireccion();
    DatosClienteFoto();
    ui->stackedWidget->setCurrentIndex(7);

}
void MainWindow::on_reservaciones_15_clicked(){
    clearLayout(ui->res_activas);
    CatalogoReservacion();
    ui->stackedWidget->setCurrentIndex(8);
}
void MainWindow::on_salir_15_clicked(){
    ui->Estado->clear();
    ui->Municipio->clear();
    ui->Calle->clear();
    ui->Colonia->clear();
    ui->direc_5->clear();
    ui->direc_6->clear();
    ui->direc_7->clear();
   ui->stackedWidget->setCurrentIndex(0);
}
void MainWindow::on_cancelar_clicked(){
    QString catalogo, valoracion, nombre, id;
    QTableWidgetItem * nom;
    QSqlQuery query;
    nombre=ui->Nom_2->text();

    id="select idCasa from casa where Casa.nombre = '"+nombre+"';";
    QSqlQuery insert2;
    insert2.prepare(id);
    insert2.exec();
    while ( insert2.next()){
        id=insert2.value(0).toString();

    }

    QMessageBox::StandardButton reply;
       reply = QMessageBox :: question(this, "Reservacion", "¿Esta seguro de cancelar su reservacion?",QMessageBox::Yes|QMessageBox::No );
       if (reply == QMessageBox::Yes)
       {
           catalogo="update casa set idCatalogo ='2' where Casa.idCasa= '"+id+"';";

           if (query.exec(catalogo)){
                   clearLayout(ui->layout);
                   CatalogoPrincipal();
                   QMessageBox :: information(this, "Reservacion", "Dentro de una semana su dinero le sera de vuelto a mas tardar" );
           }

       }
       else
       {
             QMessageBox :: information(this, "Reservacion", "La reservacion sigue activa" );
       }


}

//-------------------------------------- Interfaces del Administrador ----------------------------------

void MainWindow::CasaEspera(){
    QSqlQuery query;
    QString sql;
    int numeroColumnas = ui->solicitudesTableWidget->rowCount();

    QList<int> numeros;
    for(int i=1; i<=numeroColumnas; i++){
        numeros.append(i);
        ui->solicitudesTableWidget->removeRow(numeros.value(i));
    }

    sql = "select nombre, no_habitaciones, no_banos, precio, descripcion, servicios from Casa where casa.idCatalogo='1';";
    query.exec(sql);

    while(query.next()){
        QString titulo = query.value(0).toString();
        QString no_habitaciones = query.value(1).toString();
        QString no_banos = query.value(2).toString();
        QString precio = query.value(3).toString();
        QString desc = query.value(4).toString();
        QString servicios = query.value(5).toString();

        ui->solicitudesTableWidget->insertRow(ui->solicitudesTableWidget->rowCount());

        int fila=ui->solicitudesTableWidget->rowCount()-1;
        ui->solicitudesTableWidget->setItem(fila, 0, new QTableWidgetItem(titulo));
        ui->solicitudesTableWidget->setItem(fila, 1, new QTableWidgetItem(no_habitaciones));
        ui->solicitudesTableWidget->setItem(fila, 2, new QTableWidgetItem(no_banos));
        ui->solicitudesTableWidget->setItem(fila, 3, new QTableWidgetItem(precio));
        ui->solicitudesTableWidget->setItem(fila, 4, new QTableWidgetItem(desc));
        ui->solicitudesTableWidget->setItem(fila, 5, new QTableWidgetItem(servicios));
    }
}

//Opciones Admin
void MainWindow::on_ir_clicked(){
    clearLayout(ui->layout_3);
    CatalogoAdmin();
    ui->stackedWidget->setCurrentIndex(16);
}
void MainWindow::on_ir_3_clicked(){
    CasaEspera();
    ui->stackedWidget->setCurrentIndex(18);
}
void MainWindow::on_Salir_clicked(){
   ui->stackedWidget->setCurrentIndex(0);
}

//Catalogo Administrados
void MainWindow::on_volver_2_clicked(){
    ui->stackedWidget->setCurrentIndex(15);
}

//Vista detallada de las casas
void MainWindow::on_regresar_7_clicked(){
    clearLayout(ui->layout_3);
    CatalogoAdmin();
    ui->stackedWidget->setCurrentIndex(16);
}
void MainWindow::on_eliminar_clicked(){
    QString catalogo, valoracion, nombre, id;
    QTableWidgetItem * nom;
    QSqlQuery query;
    nombre=ui->label_299->text();


    id="select idCasa from casa where Casa.nombre = '"+nombre+"';";
    QSqlQuery insert2;
    insert2.prepare(id);
    insert2.exec();
    while ( insert2.next()){
        id=insert2.value(0).toString();

    }

     QMessageBox::StandardButton reply;
    reply = QMessageBox :: question(this, "Reservacion", "¿Esta seguro de cancelar su reservacion?",QMessageBox::Yes|QMessageBox::No );
    if (reply == QMessageBox::Yes){
        catalogo="update casa set idCatalogo ='3' where Casa.idCasa= '"+id+"';";
        if (query.exec(catalogo)){
                QMessageBox::about(this, "Casa", "Casa Eliminada del catalogo");

        }
    }
    else
    {
    QMessageBox :: information(this, "Casa", "La casa sigue en el catalogo" );
    }


}

//Tabla de Casas a ser registradas
void MainWindow::on_volver_3_clicked(){
    ui->stackedWidget->setCurrentIndex(15);
}
void MainWindow::on_solicitudesTableWidget_itemClicked(QTableWidgetItem *item){
     QString catalogo;
    f= item->row();
    QTableWidgetItem * nom, *no_habitaciones, *no_banos, *precio, *descripcion, *servicios;
    nom= ui->solicitudesTableWidget->item(f, 0);
    no_habitaciones= ui->solicitudesTableWidget->item(f, 1);
    no_banos= ui->solicitudesTableWidget->item(f, 2);
    precio= ui->solicitudesTableWidget->item(f, 3);
    descripcion= ui->solicitudesTableWidget->item(f, 4);
    servicios= ui->solicitudesTableWidget->item(f, 5);
    ui->ANom->setText(nom->text());
    ui->ANH->setText(no_habitaciones->text());
    ui->ANB->setText(no_banos->text());
    ui->APrec->setText(precio->text());
    ui->ADesc->setText(descripcion->text());
    ui->ASev->setText(servicios->text());

}
void MainWindow::on_reserva_clicked(){
    ui->stackedWidget->setCurrentIndex(19);
}

//Valorar casas
void MainWindow::on_volver4_clicked(){
   ui->stackedWidget->setCurrentIndex(18);
   CasaEspera();
}
void MainWindow::on_anadir_clicked(){
    QString catalogo, valoracion, nombre, id;
    QTableWidgetItem * nom;
    QSqlQuery query;
    nombre=ui->ANom->text();
    nom= ui->solicitudesTableWidget->item(f, 0);
    valoracion=ui->comboBox->currentText();

    id="select idCasa from casa where Casa.nombre = '"+nombre+"';";
    QSqlQuery insert2;
    insert2.prepare(id);
    insert2.exec();
    while ( insert2.next()){
        id=insert2.value(0).toString();
        qDebug()<<"id  "+id;
    }

    catalogo="update casa set idCatalogo ='2', valoracion = '"+valoracion+"' where Casa.idCasa= '"+id+"';";
    if (query.exec(catalogo)){
            QMessageBox::about(this, "Valoracion", "Casa Valorada");

    }

}
void MainWindow::on_Rechazar_clicked(){
    QString catalogo, valoracion, nombre, id;
    QTableWidgetItem * nom;
    QSqlQuery query;
    nombre=ui->ANom->text();
    nom= ui->solicitudesTableWidget->item(f, 0);

    id="select idCasa from casa where Casa.nombre = '"+nombre+"';";
    QSqlQuery insert2;
    insert2.prepare(id);
    insert2.exec();
    while ( insert2.next()){
        id=insert2.value(0).toString();

    }

    catalogo="update casa set idCatalogo ='3' where Casa.idCasa= '"+id+"';";
    if (query.exec(catalogo)){
            QMessageBox::about(this, "Casa", "Casa Rechazada");

    }
}


