drop database if exists Rentas;
create database Rentas;
use Rentas;

delimiter $$ 
drop procedure if exists Nombre$$ 
create procedure Nombre(
	in nom VARCHAR (100)
)

begin 
	select 
		f.imagen
	from 
		fotos as f  inner join Casa as c on f.idFotos = c.idFotos
    where 
		c.idCatalogo= '2' and
        c.nombre= nom;
    
end $$ 
delimiter ; 


delimiter $$ 
drop procedure if exists FotoPropi$$ 

create procedure FotoPropi(
	in nomP VARCHAR (100)
)

begin 
	select 
		f.imagen
	from 
		fotos as f  inner join Propietario as p on f.idFotos = p.idFotos
    where 
        p.usuario= nomP;
    
end $$ 
delimiter ;


delimiter $$ 
drop procedure if exists FotoCliente$$ 

create procedure FotoCliente(
	in nomC VARCHAR (100)
)

begin 
	select 
		f.imagen
	from 
		fotos as f  inner join Cliente as c on f.idFotos = c.idFotos
    where 
        c.usuario= nomC;
    
end $$ 
delimiter ;


create table Login(
  usuario NVARCHAR(50) not null,
  contrasena NVARCHAR(45) not null,
  PRIMARY KEY (usuario));
  
create table Direccion(
	idDireccion int auto_increment,
	estado NVARCHAR(45) NOT NULL,
	municipio NVARCHAR(45) NOT NULL,
	calle NVARCHAR(45) NOT NULL,
    colonia NVARCHAR(45) NOT NULL,
	no_interior INT NULL,
	no_exterior INT NOT NULL,
	cp INT NOT NULL,
 
PRIMARY KEY (idDireccion));

create table Fotos(
	idFotos INT  auto_increment,
	ruta NVARCHAR(100) NOT NULL,
	imagen LONGBLOB NULL,
  PRIMARY KEY (idFotos));
  
create table Cliente(
	idCliente int auto_increment,
	nombre NVARCHAR(45) NOT NULL,
	a_paterno NVARCHAR(45) NOT NULL,
	a_materno NVARCHAR(45) NOT NULL,
	telefono NVARCHAR(45) NOT NULL,
	correo NVARCHAR(45) NOT NULL,
    usuario NVARCHAR(50) not null,
    idDireccion INT NULL,
    idFotos INT NULL,
	FOREIGN KEY(idFotos) references Fotos(idFotos) ON DELETE NO ACTION ON UPDATE NO ACTION,
	FOREIGN KEY(idDireccion) references Direccion(idDireccion) ON DELETE NO ACTION ON UPDATE NO ACTION,
	FOREIGN KEY(usuario) references Login(usuario) ON DELETE NO ACTION ON UPDATE NO ACTION,
    
	PRIMARY KEY (idCliente));
        
create table Propietario( 
	idPropietario int auto_increment,
	nombre NVARCHAR(45) NOT NULL,
	a_paterno NVARCHAR(45) NOT NULL,
	a_materno NVARCHAR(45) NOT NULL,
	telefono NVARCHAR(45) NOT NULL,
	correo NVARCHAR(45) NOT NULL,
    usuario NVARCHAR(50) not null,
    idDireccion INT NULL,
    idFotos INT NULL,
	FOREIGN KEY(idFotos) references Fotos(idFotos) ON DELETE NO ACTION ON UPDATE NO ACTION,
	FOREIGN KEY(idDireccion) references Direccion(idDireccion) ON DELETE NO ACTION ON UPDATE NO ACTION,
	FOREIGN KEY(usuario) references Login(usuario) ON DELETE NO ACTION ON UPDATE NO ACTION,
	PRIMARY KEY (idPropietario));
    
create table Admin(
	idAdmin int  auto_increment,
    usuario NVARCHAR(50) not null,
	FOREIGN KEY(usuario) references Login(usuario) ON DELETE NO ACTION ON UPDATE NO ACTION,
    
PRIMARY KEY (idAdmin));

create table Catalogo(
  idCatalogo INT  auto_increment,
  fecha_publicacion DATE NOT NULL,
  idAdmin int not null ,
  FOREIGN KEY(idAdmin) references Admin(idAdmin) ON DELETE NO ACTION ON UPDATE NO ACTION,
  PRIMARY KEY (idCatalogo));

create table Casa(
	idCasa INT  auto_increment,
    nombre NVARCHAR(45) NOT NULL,
    no_habitaciones INT NOT NULL,
	no_banos INT NOT NULL,
    precio DOUBLE NOT NULL,
    descripcion NVARCHAR(100) NOT NULL,
    servicios NVARCHAR(100) NOT NULL,
	idCatalogo INT NULL,
	idDireccion INT not NULL,
    idPropietario int not null,
    idFotos INT NULL,
    valoracion INT NULL,
    idCliente INT NULL,
    FOREIGN KEY(idCatalogo) references Catalogo(idCatalogo) ON DELETE NO ACTION ON UPDATE NO ACTION,
    FOREIGN KEY(idDireccion) references Direccion(idDireccion) ON DELETE NO ACTION ON UPDATE NO ACTION,
    FOREIGN KEY(idFotos) references Fotos(idFotos) ON DELETE NO ACTION ON UPDATE NO ACTION,
    FOREIGN KEY(idPropietario) references Propietario(idPropietario) ON DELETE NO ACTION ON UPDATE NO ACTION,
    FOREIGN KEY(idCliente) references Cliente(idCliente) ON DELETE NO ACTION ON UPDATE NO ACTION,
    
    PRIMARY KEY (idCasa));
 
 create table Casa_NoAdmin(
	idCasa_NoAdmin INT  auto_increment,
	idCasa INT NOT NULL,
    FOREIGN KEY(idCasa) references Casa(idCasa) ON DELETE NO ACTION ON UPDATE NO ACTION,
PRIMARY KEY (idCasa_NoAdmin));

 create table Casa_Admin(
	idCasa_Admin INT auto_increment,
	idCasa INT NOT NULL,
    no_contrato INT NOT NULL,
    tasa_remunerada DOUBLE NOT NULL,
    FOREIGN KEY(idCasa) references Casa(idCasa) ON DELETE NO ACTION ON UPDATE NO ACTION,
PRIMARY KEY (idCasa_Admin));
    
create table Reservacion(
	idReservacion INT auto_increment,
	fecha_entrada DATE NOT NULL,
	fecha_salida DATE NOT NULL,
    idCasa INT NOT NULL,
	idCliente int not null,
    pago_anticipado DOUBLE NULL,
	FOREIGN KEY(idCasa) references Casa(idCasa) ON DELETE NO ACTION ON UPDATE NO ACTION,
    FOREIGN KEY(idCliente ) references Cliente(idCliente ) ON DELETE NO ACTION ON UPDATE NO ACTION,
  PRIMARY KEY (idReservacion));
  
create table Contrato(
	monto_recibido DOUBLE NULL,
	monto_restante DOUBLE NULL,
	mostrar_contrato BLOB NULL,
	idReservacion INT NOT NULL,
    FOREIGN KEY(idReservacion) references Reservacion(idReservacion) ON DELETE NO ACTION ON UPDATE NO ACTION,
  PRIMARY KEY (idReservacion)); 
  
create table Tarjeta(  
	idtarjeta INT auto_increment,
	no_tarjeta INT NULL,
	codigo INT  NULL,
	mes INT  NULL,
	agnio INT  NULL,
	tipo_tarjeta TINYINT NOT NULL,
    idCliente int null,
    idPropietario int null,
    FOREIGN KEY(idPropietario) references Propietario(idPropietario) ON DELETE NO ACTION ON UPDATE NO ACTION,
    FOREIGN KEY(idCliente ) references Cliente(idCliente ) ON DELETE NO ACTION ON UPDATE NO ACTION,
  PRIMARY KEY (idtarjeta));













