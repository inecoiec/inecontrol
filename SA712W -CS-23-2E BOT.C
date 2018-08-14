/******************************************************
   Base - SA712W-CS-23-2E
   INECO-IEC, 22 de Abril de 2018

   Encendido por Boton+ Compatibilidad

   IMPORTANTE
   Ir a seccion definicion de parámetrossasaas

   RCM3700 !!!!!!
 	===========
	Cliente: No segg
	Version 4
	
	
   ===========


   Description 3w3
	===========

   SNMP

   Aires Acondicionados Diferentes

   Programa para secuenciar aires acondicionados
   Contiene todas las rutinas:

   Encendido:
     Controles remotos por Timer. Sin SPI
     10  Contactores

   Esquemas:
		Secuenciado
      Encendido Fijo
      11/08/08 Sensibilidad temperatura

      Link de prueba mails
		Link para demos

      Validación o no de mails
      Configuracion tiempos envios de mail de status.

   Configuraciones adicionales
   	Mail From
      DNS Server


   Funcionalidades:
   El secuenciador sigue en su cuenta ante:
   Falla de linea
   Falla por alta temperatura.



   Etapa 1: Inicializar puertos
            Puerto B PB0, PB2-PB5 Outputs
            Puerto E PE0, PE1, PE4 Input
variables que empiezan con
w =tienen una variable que igualar con la de alguna form
v =variable local para comparación o almacén de dato
int =variable local entera
bit =variable con dato 0  o 1
f= variable declarada para formularios

03/01/06 Se agrega Modulo de mensajes.
xx/xx/xx Se agrega modulo de control
22/04/06 Se agregan modulos para lectura de SHT71
30/08/06 Se saca configuracion puerto PA
12/05/08 Se modifica puerto de Lectura falla de linea de E a F
18/08/08 Se agrega detección de Low Battery y corte por baja tension batería
29/01/09 Se modifican puertos para nuevo impreso    (D)
31/01/09 Se codifica nombre de archivo
06/08/09 Se agrega sample para lectura de control remoto

26/08/2010 Se modifica IR-Learn para no usar SPI
26/08/2010 Se almacena en files DLM codificacion de control remoto
05/09/2010 Se agrega config IR por el usuario
11/10/2010 Se inicia trabajo sobre SNMP
**********************************************************

V-20 SMTP port
Correccion de Bugs

V-20-1 09/02/2016 se saca condición envio desde EnaRut para apagarEquipos()
		 Se pasa a apagar sin condición

		 18/02/2016 No cambiaba bitCompen en verifytempalta en version contactor


V-20-2 09/04/2016 Se corrigen textos mails.
V-20-2 smtp x nombre 09/04/2016 Se agrega smtp por nombre
V-21-2 Encendido por boton reset y No necesita Link
V-22-2 Agregado modo compatibilidad.
V-23-2 Control remoto tipo TV
22-04-2017 Agregado de pantalla de Configuracion
22-04-2017 Encendio de los dos aires por boton presionado mas de 5 segundos


********************************************************/

#class auto
#memmap xmem

#define SNMP_ENABLED
//#define VMON_ENABLED

#ifdef SNMP_ENABLED
	#define USE_SNMP		1		// This is necessary for all SNMP applications
	#define SNMP_TRAPS			// This must be defined to support trap sending
	#define SNMP_INTERFACE	IF_ANY	// Support all incoming interfaces
	#define SNMP_ENTERPRISE			33222 //INECOIEC by IANA
#endif

#define TCPCONFIG 1
#define HTTP_TIMEOUT 5
#define USE_RABBITWEB 1
#define HTTP_MAXSERVERS 2
#define USE_SMTP_AUTH
#MAMA

#define REDIRECTHOST    _PRIMARY_STATIC_IP
#define REDIRECTTO      "http://" REDIRECTHOST "/frmcontrol.zhtml"

#define PortBCfg 0xff   //configura port B como Salidas
#define On        0 // ver en circuito definitivo el estado
#define Off       1 // ver en circuito definitivo el estado

// DLM ---
#define UDP_SOCKETS 4	// allow enough for downloader and DHCP
#define MAX_UDP_SOCKET_BUFFERS 4


//------------------------------------------------------
// Forma de encender equipos  INFRARROJO
//                            CONTACTOR
//                            BOTON

//#define INFRARROJO
#define CONTACTOR
//#define BOTON

//-----------------------------------------------
// Constantes para puertos
// o Define salida
// i Define entrada
//--------------------------------------

//-- Puerto B
#ifdef INFRARROJO
	#define boIrAA1     0 // Activacion Led Infrarojo AA1
	#define boIrAA2     2 // Activacion Led Infrarojo AA2
	#define buffir  800          // longitud del buffer para grabar codigos CR
#endif

#ifdef BOTON // hay que verlo
	#define boButtAA1   0 // Activacion Boton boButtAA1
	#define boButtAA2   2 // Activacion Boton boButtAA2
   #define boButt      3 // Activacion de boton contol remoto
#endif

#ifdef CONTACTOR
	#define boContAA1   0 // contactor AA1
	#define boContAA2   2 // contactor AA2
#endif
//------------------------------------------------------


#define boIrAA1     0 // Activacion Led Infrarojo AA1
#define boIrAA2     2 // Activacion Led Infrarojo AA2

#define boButtAA1   0 // Activacion Boton boButtAA1
#define boButtAA2   2 // Activacion Boton boButtAA2

#define boContAA1   0 // contactor AA1
#define boContAA2   2 // contactor AA2
//------------------------------------------------------

#define boButt      3 // Activacion de boton contol remoto



#define boAcFail    4 // Indicador ACFail
#define boLedAA2    5 // Indicador encendido AA 2 pasa a PB5
#define boAireFijo  7 // Indicador aire fijo

//-----------------------------------------------
//-- Puerto C
#define coAlExterna   0 // Relé alarma externa

//-----------------------------------------------
//-- Puerto F
#define fiLowBattery  6 // detección baja tension bateria
#define foIrClock     0 // pin de salida de clock
#define foCorteBaja   1 // Transistor de corte por baja tensión batería
#define foLedAA1    4 // Indicador encendido AA 1
#define foHighTemp  5 // Indicador HighTemp
#define fiAcFail    7 // deteccion falla de linea alternativa

//-----------------------------------------------
//-- Puerto E
#define eiDemo   1  // Llave de demo
#define eiSilReset  4  // Botón reset soft
#define eiAcFail 7  // Falla de linea en puerto
#define eoBuzzer 5  // activación buzzer

//----------------------------------------------
//-- De los tiempos

#define SegXHora    3600UL  // UL significa que la constante es Unsigned Long
#define SegXMin       60UL  // UL significa que la constante es Unsigned Long

//*******--para medicion de temperatura
#define SHT_TMOUT 2000



typedef union       // define una sola posición de memoria para i y para
{ unsigned int i;   // f, y se lo llamará value.
  float f;
} value;

enum {TEMP,HUMI};

int k;

#define	DATA   	4    // tenia Data = 0 pero en el 3000 no está disponible // se cambio. tenia el 4
#define	SCK   	0    // tenia Data = 7 pero en el SA712 no está disponible

#define noACK 0
#define ACK   1
                            //adr  command  r/w
#define STATUS_REG_W 0x06   //000   0011    0
#define STATUS_REG_R 0x07   //000   0011    1
#define MEASURE_TEMP 0x03   //000   0001    1
#define MEASURE_HUMI 0x05   //000   0010    1
#define RESET        0x1e   //000   1111    0

//****************************************
// Seccion definicion de Parámetros
//***************************************
//const char wModelo[15]="SA-712W-IR";
const char wModelo[15]="SA-712W-CS";
#web wModelo

const char wFechaFab[15]="22/04/2017";
#web wFechaFab

const char wVersionFW[17]="23-2E";
#web wVersionFW

const char wVersionHW[15]="SA-IRD";
#web wVersionHW

char wMacAddress[20];
#web wMacAddress

char TituloPags[26];
#web TituloPags


typedef struct {
       int cVerifyTempAlta;
       int cVerifyTempCompen;
       int cFallaLinea;
       int cMensStatus;
       int cRepiteRepoTemp;
       int cEsperaTemp;
       int cEsperaComp;
       int cSendMail;
       int cTestAlarmas2Eq;
       int cTestAlarmasFijoHT;
       int cTestAlarmasCompen;
       int bitAcFail;  // 100
       int bitTmpAlta; // 500
       int bitEsperaTemp;
       int bitCompen;  // 600
       int bitEsperaCompen;
       int bitMinConmut;
       int wFijo;      // 800
       int EqOn;
       int CodEmail;
       int EnaRutAcFail;
       int EnaRutTmp;
       int wMalTemp; // cantidad de veces que se usó doble sensado ????
       int pfoCorteBaja;
       int pfiLowBattery;
} StatVar;

StatVar EstVariables;

#web EstVariables;
//-Para grabación de parámetros en Flash

typedef  struct {
     int bitSistON; // indica si el sistema estuvo encendido
     char wdirip[16];
     char wmask[16];
     char wrouter[16];
     char wdnssrvr[16];
     char wsmtpsrvr[41];
     unsigned int wsmtpport;
     char wsmtpmailfrom[41];
     char wsmtpcuentaval[31];
     char wsmtppass[26];
     int inthabimens;
     char wAsunto[71];
     char wDestMensCrit[71];
	  char wDestMensNotif[71];
     char wPassRegistrada[11];      // Password ingresada por usuario
     long vintTMinACFail;
     unsigned long kTimPriConm;//
     unsigned long kTimConmut1; //
     unsigned long kTimEsperaTemp;
     unsigned long kTimEsperaComp;
     unsigned long kTimRepStat;
     unsigned long kTimRepTemp;
     unsigned long kTMinACFail;
     unsigned int vintTimRepStat;   // tiempo para repetir mensajes de status
     unsigned int vintTimRepTemp;   // Tiempo para repetir mensaje temperatura
     unsigned int vintTimPriConm;  // tiempo de conmutacion en horas
     unsigned int vintTimConmut1;  // tiempo de conmutacion en horas
     int intSelAAInit;
     int vintTempNormal;
     int vintTempDeltaComp;
     int vintTempAlta;
     int vintTHEsperaTemp;
     int vintTHEsperaTComp;
     int vintTempSense; // sensibilidad para temp: Cantidad de veces que le tiene que dar igual

     int vintIrReg1Ai;	// Indice IR Registro A
     int vintIrReg1Bi;   // Indice IR Registro B
     int vintIrReg1Ci;   // Indice IR Registro C
     int vintIrReg1Di;   // Indice IR Registro D

	  int vintIrReg1Aj;	// Valor j de Registro A(i)
     int vintIrReg1Bj;   // Valor j de Registro B(i)
     int vintIrReg1Cj;   // Valor j de Registro C(i)
     int vintIrReg1Dj;   // Valor j de Registro D(i)

     int vintCompMode1;  // modo compatibilidad (activa corrección)

     int vintIrReg2Ai;	// Indice IR Registro A
     int vintIrReg2Bi;   // Indice IR Registro B
     int vintIrReg2Ci;   // Indice IR Registro C
     int vintIrReg2Di;   // Indice IR Registro D

	  int vintIrReg2Aj;	// Valor j de Registro A(i)
     int vintIrReg2Bj;   // Valor j de Registro B(i)
     int vintIrReg2Cj;   // Valor j de Registro C(i)
     int vintIrReg2Dj;   // Valor j de Registro D(i)

     int vintCompMode2;  // modo compatibilidad (activa corrección)

     int vintTipoCr1;	// tipo control remoto 1
     int vintReenc1;    // rencendido Aire 1

     int vintTipoCr2;	// tipo control remoto 2
     int vintReenc2;    // rencendido Aire 2


     int vintDivCr1;		//divisor para portadora infrarrojo
     int vintDivCr2;		//divisor para portadora infrarrojo
     int inthabisnmp;
     char snmpIPtrap1[16];
	  char snmpIPtrap2[16];
	  char snmpIPtrap3[16];
     char snmpReadComm[16];
	  char snmpWriteComm[16];
     char snmpTrapComm[16];


} Configuracion;


const static Configuracion info_defaults={
                                          0,
														"192.168.1.222",
                                          "255.255.255.0",
                                          "192.168.1.1",
                                          "192.168.1.1",
                                          "No Configurado",
                                          25,
                                          "No Configurado",
                                          "",
                                          "",
                                          1,
                                          "DATACENTER ",
                                          "",
                                          "",
                                          "admin",
                                          2UL,// 2 horas: se es menos conmuta el aire si vuelven los 220
                                          3600UL,
                                          3600UL,
                                          3600UL,
                                          3600UL,
                                          3600UL,
                                          60UL,
                                          3600UL,
                                          12, //repeticion mensaje status (horas)
                                          5, //repeticion mensaje Temperatura (minutos)
                                          2, // tiempo de primera conmutacion
                                          24, // tiempo de conmutacion
                                          1,  // aire acondicionado seleccionado
                                          27, // temperatura normal
                                          23,//temperatura compensacion
                                          31, // temperatura alta
                                          1,// (Default 1) tiempo de espera para medicion
                                          2, // tiempo de duracion de compensacion
                                          5, // cantidad de veces que tiene que medir temperatura
                                          0,   // Indice i Registro A
                                          0,   // Indice i Registro B
                                          99,  // Indice i Registro C
                                          100, // Indice i Registro D
                                          132,   // valor j Registro A
                                          132,   // valor j Registro A
                                          158,   // valor j Registro A
                                          129,   // valor j Registro A
                                          1,   // habilitar modo compatibilidad                                    137,// divisor portadora IR AA 1
                                          0,   // Indice i Registro A
                                          0,   // Indice i Registro B
                                          99,  // Indice i Registro C
                                          100, // Indice i Registro D
                                          132,   // valor j Registro A
                                          132,   // valor j Registro A
                                          158,   // valor j Registro A
                                          129,   // valor j Registro A
                                          1,   // habilitar modo compatibilidad                                    137,// divisor portadora IR AA 1
														0,	// tipo cr 1
                                          0, // reencendido 1
                                          0, // tipo cr 2
                                          0, // reencendido 2
                                          137,
                                          137,
                                          0, // habilitacion de snmp
														"192.168.1.150",
                                          "No Configurado",
                                          "No Configurado",
                                          "public",
                                          "private",
                                          "trap"};

Configuracion config; // declara la variable config del tipo configuracion

#ifdef INFRARROJO
	typedef  struct {
   	  unsigned long int time_bits[buffir];
	} info_cremotos;

	info_cremotos codesIR;
	int AireACalibrar;
	int CrDivTest;
 // Para lectura del IR
	 unsigned long int j, max,delay;
	 unsigned int i;
	 char bState;
// unsigned long int time_bits[buff];
	 int br;

    unsigned long int wIr_reg1A;
	 unsigned long int wIr_reg1B;
    unsigned long int wIr_reg1C;
    unsigned long int wIr_reg1D;

    unsigned long int wIr_reg2A;
	 unsigned long int wIr_reg2B;
    unsigned long int wIr_reg2C;
    unsigned long int wIr_reg2D;


#endif


#web config


#define SMTP_VERBOSE
#define SMTP_SERVER config.wsmtpsrvr
#define SMTP_PORT config.wsmtpport


#define INPUT_COMPRESSION_BUFFERS 4
#use "zimport.lib"

#use "dcrtcp.lib"
#use "http.lib"
#use "smtp.lib"

#ifdef SNMP_ENABLED
	longword trapdest_ip;
	snmp_oid r_oid; // variable para armar el buffer
#endif

// DLM ----
#define DLM_RW
#use SHD_DLM.LIB


//Imagen principal---------
#ximport "/Encabe2.jpg"         encabe2

//Páginas controlador -----------------------------------------------
#zimport "/formppal.zhtml" 	  index_zhtml   // si
#zimport "/func.js"				  func_java
#ximport "/estilos.css"			  estilos_css
#ximport "/variables.xml"       variables_xml


#zimport "/frmred.zhtml"        frmred_zhtml  //
#zimport "/frmcontrol.zhtml"    frmcontrol_zhtml
#zimport "/ftemp.zhtml"         frmtemp_zhtml      // si
#zimport "/ftiempos.zhtml"      frmtiempos_zhtml   // si
#zimport "/frmmens.zhtml"       frmmens_zhtml
#zimport "/frmlogin.zhtml"      frmlogin_zhtml
#zimport "/fparams.zhtml"       fparams_zhtml

#zimport "/frmreset.zhtml"      frmreset_zhtml

#ifdef INFRARROJO
	#zimport "/fcomirojo.zhtml"     fcomirojo_zhtml
	#zimport "/ayudas.zhtml"     	  ayudas_zhtml
#endif

#ifdef SNMP_ENABLED
	#zimport "/frmsnmp.zhtml"		  frmsnmp_zhtml
#endif



//-----------------------------------------------
int Save_Config(void)
//-----------------------------------------------
{
    int handle;
    handle=ffile_open("Config.data");
    if (handle<0)
        handle=ffile_create("Config.data");
    if (handle>=0)
    {
        ffile_write(handle,(char *)&config, sizeof config);
        ffile_close(handle);
        printf("Grabado en la flash\n");
        return 1;

    }
    return 0;

}

//-----------------------------------------------
int Load_Config(void)
//-----------------------------------------------
{
	 int handle;
         handle=ffile_open("Config.data");
    if (handle>=0)
    {
        ffile_read(handle,(char *)&config, sizeof config);
        ffile_close(handle);
        return 1;

    }
    return 0;
}


#ifdef SNMP_ENABLED
//**** variables para snmp
 	snmp_parms _p;
	snmp_parms * p;
	word tt;
	word trapindices[10];
	word monindex;

   int  intLectTemp;
   int  intLectHum;
//***********************
#endif

// Variables de estados vieja, hay que  mejorar los nombres
int wLoop;         // si es 1 se hace el loop si es 0 no
int wFijo;
int Wait;

int lineamail;
int NroMail;
char NroAsunto[80];



typedef struct {
        char DescEstado[70];
        } stestados;
stestados RecEstGral[10];

typedef struct {
        char DescEst220[85];
        } stest220;
stest220  RecEst220[10];

typedef struct {
         char DescEstTemp[85];
         } stestTemp;
stestTemp RecEstTemp[10];

typedef struct {
         char DescEstSec[95];
         } stestSec;
stestSec  RecEstSec[15];

int wDeltaTemp;

// Variables para reportes e integracion ELIPGO--------
//char wMensInforme[450]; // Reporte de eventos
char wTipoReporte[20]; // reporte de temperatura

char wEstLinea[5];
char wEstTemp[5];
char wEstEq1[6];
char wEstEq2[6];
char wEstSec[10];
char wEstModo[10];
char wEstDelayMed[10];

#web wDeltaTemp

#web wEstLinea
#web wEstTemp
#web wEstEq1
#web wEstEq2
#web wEstSec
#web wEstModo
#web wEstDelayMed

#ifdef INFRARROJO
	char wBtCalAA1[10];
	char wBtCalAA2[10];
	char wBtRegCalAA1[10];
	char wBtRegCalAA2[10];



	#web wBtCalAA1
	#web wBtCalAA2
	#web wBtRegCalAA1
	#web wBtRegCalAA2
#endif


int wCodeEvento,widTemp,wid220,widSec,widXX,widGral;



//-declaracion de funciones para que las vean las cgi
void CargaDatParam(void);// carga datos de parámetros
void CargaDatTemp(void); // carga datos de temperaturas
void CargaDatSense(void);// carga los datos de Sense en frmparams
void CargaDatRed(void); // carga los nuevos datos de la red
void CargaDatMens(void); // carga nuevos mensajes


#ifdef SNMP_ENABLED
	void EnvioTraps(int nrotrap);
	void CargaDatSnmp(void); // carga datos SNMP
#endif

void CargaDatTiempos(void); // carga nuevos datos de tiempos.
void CargaNewPass(void); // carga nueva password de login
void VerifyPass(void); // verifica password ingresada
void CargaDatDivCr(void);

void CargaDatModiCodIR(void); // carga datos modificacion codigos IR


//void sendmail(char *mensaje, char *destinos);
void CrearReporte(int TipoReporte, int CodeEvento, int idTemp, int id220, int idSec, int idXX, int idGral);
//void DatosMensInforme(void);
//----
void ActEstadosEq(char *pwEstEq1,char *pwEstEq2);
void ActEstadosTemp(char *pwEstTemp);
void ActEstadosLinea(char *pwEstLinea);
void ActEstadosCtrl(char *pwEstSec, char *pwEstModo, char *pwEstDelayMed);  // estados controlador
//----
int mail_generator(char * buf, int len, longword offset,
                                int flags, void * dhnd_data);


void SendMensPrueba(void);
void SemdMensLowBatt(void);
int CalcDeltaTemp(int pTipo);



//int HabiSendMail;
char DirSep[71];


void RetSecuenciado(void);
void CargaTiempos(void);
void SelAireInit(void);


void InitDemo(void); // inicializa variables para demo
void InitWork(void); // inicializa variables para trabajo


//-------------------------------------------------------------
CoData cReloj,cLecTemp,
       cFallaLinea,cVerifyTempCompen,cVerifyTempAlta,
       cDosEqON,
       cTestAlarmas2Eq,cTestAlarmasFijoHT,
       cEsperaTemp,
       cRecentConmut,
       cEsperaComp,
       cInitAire,cConmutAire,
		 cBuzzer,
       cbtReset,cbtIniciar,cbtDosEquiposON,cbtInitSecCompen,
       cSilResetStat,
       cButtExec,
       cRepiteRepoTemp,
       cMensStatus,
       cReporteTemp, // reporte de temperatura.
       cSendMail,
       cFinCompen,
       #ifdef INFRARROJO
	       cCaliDivCr,
       #endif
       cLowBattery;


// variables de magnitudes----------------------------
char magTHPriConm[10];
char magTHConmut1[10];
char magTHEsperaTemp[10];
char magTimMensStat[10];
char magTimMensTemp[10];

#web magTHPriConm
#web magTHConmut1
#web magTHEsperaTemp
#web magTimMensStat
#web magTimMensTemp

//--------------------------------------------------------


// habilita / deshabilita los links
#web config.bitSistON
// Variables para campos de confired--------


#web config.wdirip
#web config.wmask
#web config.wrouter
#web config.wdnssrvr

#web_update config.wdirip,config.wmask,config.wrouter,config.wdnssrvr CargaDatRed


#web config.vintTimRepStat    // tiempo para repetir mensajes de status
#web config.vintTimRepTemp   // Tiempo  para repetir mensaje critico
#web config.inthabimens select("Si"=0, "No"=1)
#web_update config.vintTimRepStat,config.vintTimRepTemp,config.inthabimens CargaDatMens

//
#ifdef SNMP_ENABLED
	#web config.snmpIPtrap1
	#web config.snmpIPtrap2
	#web config.snmpIPtrap3

	#web config.inthabisnmp select("Si"=0, "No"=1)

	#web config.snmpReadComm
	#web config.snmpWriteComm
	#web config.snmpTrapComm
	#web_update config.inthabisnmp,config.snmpIPtrap1,config.snmpIPtrap2,config.snmpIPtrap3,config.snmpReadComm,config.snmpWriteComm,config.snmpTrapComm CargaDatSnmp
#endif



#web config.wsmtpsrvr
#web config.wsmtpport
#web config.wsmtpmailfrom
#web config.wsmtpcuentaval
#web config.wsmtppass
#web_update config.wsmtpsrvr,config.wsmtpport,config.wsmtpmailfrom,config.wsmtpcuentaval,config.wsmtppass CargaDatMens

#web config.wDestMensCrit
#web config.wDestMensNotif
#web config.wAsunto
#web_update config.wDestMensCrit,config.wDestMensNotif,config.wAsunto CargaDatMens

#web config.vintTempSense
#web_update config.vintTempSense,wDeltaTemp CargaDatSense

#ifdef INFRARROJO

   #web wIr_reg1A
	#web wIr_reg1B
   #web wIr_reg1C
   #web wIr_reg1D

   #web wIr_reg2A
	#web wIr_reg2B
   #web wIr_reg2C
   #web wIr_reg2D


	#web config.vintDivCr1
	#web config.vintDivCr2
	#web_update config.vintDivCr1,config.vintDivCr2 CargaDatDivCr

   #web config.vintIrReg1Ai
   #web config.vintIrReg1Bi
   #web config.vintIrReg1Ci
   #web config.vintIrReg1Di
	#web_update config.vintIrReg1Ai,config.vintIrReg1Bi,config.vintIrReg1Ci,config.vintIrReg1Di CargaDatModiCodIR

   #web config.vintIrReg1Aj
   #web config.vintIrReg1Bj
   #web config.vintIrReg1Cj
   #web config.vintIrReg1Dj
	#web_update config.vintIrReg1Ai,config.vintIrReg1Bi,config.vintIrReg1Ci,config.vintIrReg1Di CargaDatModiCodIR


	#web config.vintTipoCr1 select("Seleccionar"=0, "On-OFF"=1, "Tipo TV"=2)
 	#web config.vintReenc1  select("Seleccionar"=0, "Si"=1,"No"=2)
   #web config.vintCompMode1 select("Si"=0, "No"=1)

   #web_update config.vintTipoCr1,config.vintReenc1,config.vintCompMode1 CargaDatModiCodIR

   #web config.vintIrReg2Ai
   #web config.vintIrReg2Bi
   #web config.vintIrReg2Ci
   #web config.vintIrReg2Di
	#web_update config.vintIrReg2Ai,config.vintIrReg2Bi,config.vintIrReg2Ci,config.vintIrReg2Di CargaDatModiCodIR

   #web config.vintIrReg2Aj
   #web config.vintIrReg2Bj
   #web config.vintIrReg2Cj
   #web config.vintIrReg2Dj
	#web_update config.vintIrReg2Ai,config.vintIrReg2Bi,config.vintIrReg2Ci,config.vintIrReg2Di CargaDatModiCodIR



   #web config.vintTipoCr2 select("Seleccionar"=0, "On-OFF"=1, "Tipo TV"=2)
 	#web config.vintReenc2  select("Seleccionar"=0, "Si"=1,"No"=2)
   #web config.vintCompMode2 select("Si"=0, "No"=1)

   #web_update config.vintTipoCr2,config.vintReenc2,config.vintCompMode2 CargaDatModiCodIR


#endif


unsigned long  intTimSMensStat;
unsigned long  intTimSMensTemp;


char *direnvio;

//---------------------------------------------



// Variables para Tiempos---------------------
shared unsigned long  SetTimer(unsigned long tc);
unsigned	long  vintTSPriConm;  // tiempo de conmutacion en segundos
unsigned	long  vintTSConmut1;  // tiempo de conmutacion en segundos

struct tm		rtc;					// time struct
char wSelAireInit[4];



#web config.vintTimConmut1
#web config.vintTimPriConm
#web config.intSelAAInit select("AA1"=1, "AA2"=2)

#web_update config.vintTimConmut1,config.vintTimPriConm,config.intSelAAInit CargaDatTiempos

// variables para cálculos de tiempos con kWorkDemo
unsigned long  vintTRestante; // segundos que faltan para la conmutación
unsigned long  vintNow; //hora actual en segundos
unsigned long	vintHoraConmut; 	// hora a la que se deben conmutar los aires

unsigned long  vintTResetPress; // tiempo de presionado de boton reset

unsigned long vintTMinACFail; // Si vintTRestante < TMinACFail := se conmuta aire
                          // lo deberiamos pasar a un porcentaje, es deir que no
                          // sea una constante

unsigned long vintTSEsperaTemp; //tiempo de espera a que se reestablezca la temperatura
                           // por falla de un aire o por corte de energía en segundos
unsigned long vintTSEsperaTComp; //tiempo de espera a que se reestablezca la temperatura
                           // en estado de compensacion


unsigned long kWorkDemo; // constante para multiplicar al tiempo de conmutacion
                         // de la página en caso de work o de demo


// multiplicadores para pasar a segundos parámetros
//unsigned long kTimPriConm;//
//unsigned long kTimConmut1; //
//unsigned long kTimEsperaTemp;
//unsigned long kTimEsperaComp;
//unsigned long kTimRepStat;
//unsigned long kTimRepTemp;

//unsigned long  SegXMin;
//unsigned long  SegXHora;

//conversion de segundos a HMS para la form

unsigned long  vSegundos;
unsigned long  vMinutos;
unsigned long  vHoras;
#web  vSegundos
#web  vMinutos
#web  vHoras


/*{ tm_rd(&rtc);						// get time in struct tm
    vTConmut1= mktime(&rtc)+tiempo que dice la página;
    yield;
  }
*/
//------------------------------------------------------
// Variables para Cambio password ---------

char wOldPass[11];
char wNewPass[11];
char wRetNewPass[11];
char OldPassConfirm[4];
char NewPassConfirm[12];

#web wOldPass
#web wNewPass
#web wRetNewPass
#web OldPassConfirm
#web NewPassConfirm

#web_update wOldPass,wNewPass,wRetNewPass CargaNewPass


// Variables para login ---------
int bitPassOK;
char wPassInput[11];      // Password ingresada por usuario
#web bitPassOK;
#web wPassInput
#web_update wPassInput VerifyPass

//----------------------------------------------

// Variables para cambios de estados. se pusieron acá para que las vean las cgi--------
//
// Variables de estados------------------------
int bitAcFail; //
//---------------------------------------------
int EqOn; // Indica el equipo que está encendido EqOn=11 encendidos los dos
int UltimoEqOn ; // indica el último equipo que estuvo encendido antes de falla de linea
//---------------------------------------------
int bitTmpAlta;
int bitCompen; // 1 si mide temp, 0 si no mide temp
int bitCtrlActivo; // 1 si está activo el controlador
int bitDosEqON;
int bitMinConmut;
int bitSilBuzzer;
//---------------------------------------------
int bitEnaRutTmp;
int bitEnaRutAcFail;
//---------------------------------------------

//---------------------------------------------
char wAireSel[4];
#web wAireSel
//----------------------------------------------------
char wMailStatus[50];
#web wMailStatus




SSPEC_MIMETABLE_START
   SSPEC_MIME_FUNC(".zhtml", "text/html", zhtml_handler),
   SSPEC_MIME_FUNC(".xml", "text/xml", zhtml_handler),
   SSPEC_MIME(".html", "text/html"),
   SSPEC_MIME(".jpg", "image/jpg"),
   SSPEC_MIME(".gif", "image/gif"),
	SSPEC_MIME(".css","text/css"),
	SSPEC_MIME(".js", "text/javascript"),
   SSPEC_MIME(".cgi", ""),
SSPEC_MIMETABLE_END

// Rutinas CGI
/*
(pTipo==1){ // < normal
(pTipo==2){ // > Compen
(pTipo==3){// < Compen
(pTipo==4){// > Alta
*/


#ifdef INFRARROJO
	//----------------------------------------------------------------------------
	int Registra_IR(char *archnom_g)
	//----------------------------------------------------------------------------
	{
   	int handle;
	   max = 66666UL;
   	i=bState=0;
	   for (i=0;i<buffir;i++){
   	    codesIR.time_bits[i]=0xFFFF;
	   }

   	printf("Preparado para grabar %s \n",archnom_g);

	   while (BitRdPortI(PCDR,3)==1) {
//          	   time_bits[0]=BitRdPortI(PCDR,3);
   	}
   	i=0;
	   while (i<buffir){
   	  	j=0;
         while(max && (BitRdPortI(PCDR,3)==bState))
			{
			j++;
			max--;
			}
	      bState ^=0x01;
   	   codesIR.time_bits[i]=j;
      	i++;
	      if (max==0UL){
   	     	break;
      	}
	   }
   	for (i=0;i<buffir;i++){
      	printf("i= %u time = %ld \n",i,codesIR.time_bits[i]);
	   }
   	printf("\n");

   	handle=ffile_open(archnom_g);
	   if (handle<0)
      	handle=ffile_create(archnom_g);
	   if (handle>=0)
   	{
      	ffile_write(handle,(char *)&codesIR, sizeof codesIR);
         ffile_close(handle);
         printf("Grabado en la flash\n");
         return 1;
	   }

   	return 0;
	}

 	//----------------------------------------------------------------------------
	int Emite_IR(char *archnom_e, int NumAire)
	//----------------------------------------------------------------------------
	{
	// leer el archivo y cargar el contenido en time_bits

		int handle;
    	handle=ffile_open(archnom_e);
    	if (handle>=0)
    	{
        	ffile_read(handle,(char *)&codesIR, sizeof codesIR);
        	ffile_close(handle);
    	}
      ffile_close(handle);

      bState=0;   // IMPORTANTISIMO !!!! Tiene que estar en 0

      switch (NumAire){
         case 1:
      		if (config.vintCompMode1==0){
	      		if (config.vintIrReg1Aj !=0){
   	    			codesIR.time_bits[config.vintIrReg1Ai]=config.vintIrReg1Aj;
     				}
        	   	if (config.vintIrReg1Bj !=0){
       				codesIR.time_bits[config.vintIrReg1Bi]=config.vintIrReg1Bj;
      			}
      			if (config.vintIrReg1Cj !=0){
       				codesIR.time_bits[config.vintIrReg1Ci]=config.vintIrReg1Cj;
      			}
      			if (config.vintIrReg1Dj !=0){
       				codesIR.time_bits[config.vintIrReg1Di]=config.vintIrReg1Dj;
      			}
      		 }
		       wIr_reg1A=codesIR.time_bits[config.vintIrReg1Ai];
			    wIr_reg1B=codesIR.time_bits[config.vintIrReg1Bi];
   			 wIr_reg1C=codesIR.time_bits[config.vintIrReg1Ci];
      		 wIr_reg1D=codesIR.time_bits[config.vintIrReg1Di];



			    BitWrPortI(PBDR,&PBDRShadow,1,boIrAA1);
			    BitWrPortI(PBDR,&PBDRShadow,0,boIrAA2);
		       WrPortI(TAT7R,&TAT7RShadow,config.vintDivCr1);
             break;
         case 2:

      		if (config.vintCompMode2==0){
	      		if (config.vintIrReg2Aj !=0){
   	    			codesIR.time_bits[config.vintIrReg2Ai]=config.vintIrReg2Aj;
     				}
        	   	if (config.vintIrReg2Bj !=0){
       				codesIR.time_bits[config.vintIrReg2Bi]=config.vintIrReg2Bj;
      			}
      			if (config.vintIrReg2Cj !=0){
       				codesIR.time_bits[config.vintIrReg2Ci]=config.vintIrReg2Cj;
      			}
      			if (config.vintIrReg2Dj !=0){
       				codesIR.time_bits[config.vintIrReg2Di]=config.vintIrReg2Dj;
      			}
      		 }
		       wIr_reg2A=codesIR.time_bits[config.vintIrReg2Ai];
			    wIr_reg2B=codesIR.time_bits[config.vintIrReg2Bi];
   			 wIr_reg2C=codesIR.time_bits[config.vintIrReg2Ci];
      		 wIr_reg2D=codesIR.time_bits[config.vintIrReg2Di];

			    BitWrPortI(PBDR,&PBDRShadow,0,boIrAA1);
			    BitWrPortI(PBDR,&PBDRShadow,1,boIrAA2);
  		       WrPortI(TAT7R,&TAT7RShadow,config.vintDivCr2);
             break;
      }


	   i=0;
   	while (codesIR.time_bits[i] != 65535UL){
  			//printf("bState= %d  i= %u time = %ld \n",bState,i,time_bits[i]);
         for (j=0;j<codesIR.time_bits[i];j++){

             BitWrPortI(PCDR,&PCDRShadow,bState,2);
         }
         bState ^=0x01;
         i++;
   	}
      BitWrPortI(PCDR,&PCDRShadow,1,2);
	   printf("Emitido codigo= %s \n",archnom_e);
   	printf("\n");
	}



	//----------------------------------------------------------------------------
	int graba_cr1_ON(HttpState* state)
	//----------------------------------------------------------------------------
	{
 // encender led AA1 + Alta temperatura
    	BitWrPortI(PFDR,&PFDRShadow,On,foLedAA1);
    	BitWrPortI(PFDR,&PFDRShadow,On,foHighTemp);   //->

    	Registra_IR("cr1on");

   //    1 pito buzzer
    	BitWrPortI(PFDR,&PFDRShadow,Off,foLedAA1);
    	BitWrPortI(PFDR,&PFDRShadow,Off,foHighTemp);
    	cgi_redirectto(state,"/fcomirojo.zhtml");
    	return 0;
	}

	//----------------------------------------------------------------------------
	int graba_cr1_Reenc(HttpState* state)
	//----------------------------------------------------------------------------
	{
 // encender led AA1 + Alta temperatura
    	BitWrPortI(PFDR,&PFDRShadow,On,foLedAA1);
	   BitWrPortI(PBDR,&PBDRShadow,On,boAireFijo); //

    	Registra_IR("cr1reenc");

   //    1 pito buzzer
    	BitWrPortI(PFDR,&PFDRShadow,Off,foLedAA1);
		BitWrPortI(PBDR,&PBDRShadow,Off,boAireFijo);

      cgi_redirectto(state,"/fcomirojo.zhtml");
    	return 0;
	}



	//----------------------------------------------------------------------------
	int graba_cr1_OFF(HttpState* state)
	//----------------------------------------------------------------------------
	{
	 // encender led AA1 + Falla de linea
   	BitWrPortI(PFDR,&PFDRShadow,On,foLedAA1);
     	BitWrPortI(PBDR,&PBDRShadow,On,boAcFail); //->

     	Registra_IR("cr1off");

     	BitWrPortI(PFDR,&PFDRShadow,Off,foLedAA1);
     	BitWrPortI(PBDR,&PBDRShadow,Off,boAcFail);
     	cgi_redirectto(state,"/fcomirojo.zhtml");
     	return 0;
	}

	//----------------------------------------------------------------------------
	int graba_cr2_ON(HttpState* state)
	//----------------------------------------------------------------------------
	{
 	// encender led AA2 + Alta temperatura
    	BitWrPortI(PBDR,&PBDRShadow,On,boLedAA2);
    	BitWrPortI(PFDR,&PFDRShadow,On,foHighTemp);  //->

    	Registra_IR("cr2on");
	// 1 pito buzzer

    	BitWrPortI(PBDR,&PBDRShadow,Off,boLedAA2);
    	BitWrPortI(PFDR,&PFDRShadow,Off,foHighTemp);
    	cgi_redirectto(state,"/fcomirojo.zhtml");
    	return 0;
	}


	//----------------------------------------------------------------------------
	int graba_cr2_Reenc(HttpState* state)
	//----------------------------------------------------------------------------
	{
 // encender led AA1 + Alta temperatura
    	BitWrPortI(PBDR,&PBDRShadow,On,boLedAA2);
	   BitWrPortI(PBDR,&PBDRShadow,On,boAireFijo); //

    	Registra_IR("cr2reenc");

   //    1 pito buzzer

    	BitWrPortI(PBDR,&PBDRShadow,Off,boLedAA2);
	   BitWrPortI(PBDR,&PBDRShadow,Off,boAireFijo); //

      cgi_redirectto(state,"/fcomirojo.zhtml");
    	return 0;
	}



	//----------------------------------------------------------------------------
	int graba_cr2_OFF(HttpState* state)
	//----------------------------------------------------------------------------
	{
 	// encender led AA2 + Falla de linea
    	BitWrPortI(PBDR,&PBDRShadow,On,boLedAA2);
    	BitWrPortI(PBDR,&PBDRShadow,On,boAcFail);   //->

    	Registra_IR("cr2off");

    	BitWrPortI(PBDR,&PBDRShadow,Off,boLedAA2);
    	BitWrPortI(PBDR,&PBDRShadow,Off,boAcFail);
    	cgi_redirectto(state,"/fcomirojo.zhtml");
    	return 0;
 // dos pitobuzzer
	}

	//----------------------------------------------------------------------------
	int test_cr1_ON(HttpState* state)
	//----------------------------------------------------------------------------
	{
	//    BitWrPortI(PBDR,&PBDRShadow,1,boIrAA1);
	//    BitWrPortI(PBDR,&PBDRShadow,0,boIrAA2);

   	Emite_IR("cr1on",1);

    	cgi_redirectto(state,"fcomirojo.zhtml");
    	return 0;
	}

	//----------------------------------------------------------------------------
	int test_cr1_Reenc(HttpState* state)
	//----------------------------------------------------------------------------
	{
	//    BitWrPortI(PBDR,&PBDRShadow,1,boIrAA1);
	//    BitWrPortI(PBDR,&PBDRShadow,0,boIrAA2);

   	Emite_IR("cr1reenc",1);

    	cgi_redirectto(state,"fcomirojo.zhtml");
    	return 0;
	}


   //----------------------------------------------------------------------------
	int test_cr1_OFF(HttpState* state)
	//----------------------------------------------------------------------------
	{
	//    BitWrPortI(PBDR,&PBDRShadow,1,boIrAA1);
	//    BitWrPortI(PBDR,&PBDRShadow,0,boIrAA2);

   	Emite_IR("cr1off",1);

    	//AA1OFFExec;
    	cgi_redirectto(state,"/fcomirojo.zhtml");
    	return 0;
	}

	//----------------------------------------------------------------------------
	int test_cr2_ON(HttpState* state)
	//----------------------------------------------------------------------------
	{
	//    BitWrPortI(PBDR,&PBDRShadow,0,boIrAA1);
	//    BitWrPortI(PBDR,&PBDRShadow,1,boIrAA2);
    	Emite_IR("cr2on",2);

	 	//AA2ONExec;
    	cgi_redirectto(state,"/fcomirojo.zhtml");
    	return 0;
	}

	//----------------------------------------------------------------------------
	int test_cr2_Reenc(HttpState* state)
	//----------------------------------------------------------------------------
	{
	//    BitWrPortI(PBDR,&PBDRShadow,1,boIrAA1);
	//    BitWrPortI(PBDR,&PBDRShadow,0,boIrAA2);

   	Emite_IR("cr2reenc",2);

    	cgi_redirectto(state,"fcomirojo.zhtml");
    	return 0;
	}


	//----------------------------------------------------------------------------
	int test_cr2_OFF(HttpState* state)
	//----------------------------------------------------------------------------
	{
	//    BitWrPortI(PBDR,&PBDRShadow,0,boIrAA1);
	//    BitWrPortI(PBDR,&PBDRShadow,1,boIrAA2);

    	Emite_IR("cr2off",2);
    	//AA2OFFExec;
    	cgi_redirectto(state,"/fcomirojo.zhtml");
    	return 0;
	}

	//----------------------------------------------------------------------------
	int inicali_cr1(HttpState* state)
	//----------------------------------------------------------------------------
	{
    	AireACalibrar=1;



	 	strcpy(wBtCalAA1,"disabled");
	 	strcpy(wBtCalAA2,"disabled");
    	strcpy(wBtRegCalAA1,"enabled");
	 	strcpy(wBtRegCalAA2,"disabled");

	 	CoBegin(&cCaliDivCr);
    	cgi_redirectto(state,"/fcomirojo.zhtml");
    	return 0;
	}

	//----------------------------------------------------------------------------
	int inicali_cr2(HttpState* state)
	//----------------------------------------------------------------------------
	{

    	AireACalibrar=2;



    	strcpy(wBtCalAA1,"disabled");
	 	strcpy(wBtCalAA2,"disabled");
    	strcpy(wBtRegCalAA1,"disabled");
	 	strcpy(wBtRegCalAA2,"enabled");

 	 	CoBegin(&cCaliDivCr);
    	cgi_redirectto(state,"/fcomirojo.zhtml");
    	return 0;
	}

	//----------------------------------------------------------------------------
	int regcali_cr1(HttpState* state)
	//----------------------------------------------------------------------------
	{
    	CoReset (&cCaliDivCr);



	 	strcpy(wBtCalAA1,"enabled");
	 	strcpy(wBtCalAA2,"enabled");
    	strcpy(wBtRegCalAA1,"disabled");
	 	strcpy(wBtRegCalAA2,"disabled");

    	Save_Config();
    	cgi_redirectto(state,"/fcomirojo.zhtml");
    	return 0;
	}

	//----------------------------------------------------------------------------
	int regcali_cr2(HttpState* state)
	//----------------------------------------------------------------------------
	{
    	CoReset (&cCaliDivCr);

	 	strcpy(wBtCalAA1,"enabled");
	 	strcpy(wBtCalAA2,"enabled");
    	strcpy(wBtRegCalAA1,"disabled");
	 	strcpy(wBtRegCalAA2,"disabled");

    	Save_Config();
    	cgi_redirectto(state,"/fcomirojo.zhtml");
    	return 0;
	}

#endif


//----------------------------------------------------------------------------
int bt_rstltemp(HttpState* state)
//----------------------------------------------------------------------------
{
//     wDeltaTemp=0;
     cgi_redirectto(state,"/iec/fparams.zhtml");
     return 0;


}

//----------------------------------------------------------------------------
int bt_simulcom(HttpState* state) //simula compensación
//----------------------------------------------------------------------------
{

	  wDeltaTemp=CalcDeltaTemp(2);
     cgi_redirectto(state,"/iec/fparams.zhtml");
     return 0;

}

//----------------------------------------------------------------------------
int bt_bajacomp(HttpState* state) //simula menor a compensacion
//----------------------------------------------------------------------------
{

	  wDeltaTemp=CalcDeltaTemp(3);
     cgi_redirectto(state,"/iec/fparams.zhtml");
     return 0;

}

//----------------------------------------------------------------------------
int bt_simultn(HttpState* state)
//----------------------------------------------------------------------------
{

	  wDeltaTemp=CalcDeltaTemp(1);
     cgi_redirectto(state,"/iec/fparams.zhtml");
     return 0;

}

//----------------------------------------------------------------------------
int bt_simulht(HttpState* state)
//----------------------------------------------------------------------------
{
	  wDeltaTemp=CalcDeltaTemp(4);
     cgi_redirectto(state,"/iec/fparams.zhtml");
     return 0;

}
//----------------------------------------------------------------------------
int bt_hreset(HttpState* state)
//----------------------------------------------------------------------------
{
// resetea la placa
     k=DLM_Reboot(UDPDNLD_MODE_RESTART);

}
//----------------------------------------------------------------------------
int bt_demo(HttpState* state)
//----------------------------------------------------------------------------
{
    config.vintTimPriConm = 120;
    config.vintTimConmut1 = 120;
    config.vintTHEsperaTemp = 30;
    config.vintTHEsperaTComp = 30;
    config.vintTimRepStat = 20;
    config.vintTimRepTemp = 20;
    config.vintTMinACFail = 40;

    config.kTimPriConm = 1UL;
    config.kTimConmut1 = 1UL;
    config.kTimEsperaTemp = 1UL;
    config.kTimEsperaComp = 1UL;
    config.kTimRepStat = 1UL;
    config.kTimRepTemp = 1UL;
    config.kTMinACFail = 1UL;

    strcpy(magTHPriConm," Segundos");
    strcpy(magTHConmut1," Segundos");
    strcpy(magTHEsperaTemp," Segundos");
    strcpy(magTimMensStat,"Segundos");
    strcpy(magTimMensTemp," Segundos");

    cgi_redirectto(state,"/frmcontrol.zhtml");
    return 0;

}

//----------------------------------------------------------------------------
int bt_work (HttpState* state)
//----------------------------------------------------------------------------
{

     config.vintTimPriConm = 2;
     config.vintTimConmut1 = 24;
     config.vintTHEsperaTemp = 1;
     config.vintTHEsperaTComp = 1;
     config.vintTimRepStat = 12;
     config.vintTimRepTemp = 5;
     config.vintTMinACFail = 2;

     config.kTimPriConm = 3600UL;
     config.kTimConmut1 =  3600UL;
     config.kTimEsperaTemp =  3600UL;
     config.kTimEsperaComp =  3600UL;
     config.kTimRepStat =  3600UL;
     config.kTimRepTemp =  60UL;
     config.kTMinACFail = 3600UL;

     strcpy(magTHPriConm," Horas");
     strcpy(magTHConmut1," Horas");
     strcpy(magTHEsperaTemp," Horas");
     strcpy(magTimMensStat,"Horas");
     strcpy(magTimMensTemp," Minutos");

     cgi_redirectto(state,"/frmcontrol.zhtml");
     return 0;

}

//----------------------------------------------------------------------------
int bt_default (HttpState* state)
//----------------------------------------------------------------------------
{
    memcpy(&config,&info_defaults,sizeof(Configuracion));
    Save_Config();
    Load_Config();
    strcpy(magTHPriConm," Horas");
    strcpy(magTHConmut1," Horas");
    strcpy(magTHEsperaTemp," Horas");
    strcpy(magTimMensStat,"Horas");
    strcpy(magTimMensTemp," Minutos");


    cgi_redirectto(state,"/frmcontrol.zhtml");
    return 0;

}



//----------------------------------------------------------------------------
int cgi_default(HttpState* state) // carga los valores de default del sistema
//----------------------------------------------------------------------------
{
     return 0;
}


//----------------------------------------------------------------------------
int bt_silactbuzzer(HttpState* state)
//----------------------------------------------------------------------------
{
     if (bitSilBuzzer==1){
        bitSilBuzzer=0;
        BitWrPortI(PEDR,&PEDRShadow,0,eoBuzzer); // apaga el buzzer
     }else{
        bitSilBuzzer=1;
     }
     cgi_redirectto(state,"/frmcontrol.zhtml");
     return 0;

}

//----------------------------------------------------------------------------
int bt_iniciar(HttpState* state)
//----------------------------------------------------------------------------
{
     CoBegin(&cbtIniciar);
     cgi_redirectto(state,"/frmcontrol.zhtml");
     return 0;

}


//----------------------------------------------------------------------------
int bt_dosequiposon(HttpState* state)
//----------------------------------------------------------------------------
{
     CoBegin(&cbtDosEquiposON);
     cgi_redirectto(state,"/frmcontrol.zhtml");
     return 0;
}

//----------------------------------------------------------------------------
int bt_initseccompen(HttpState* state)
//----------------------------------------------------------------------------
{
     CoBegin(&cbtInitSecCompen);
     cgi_redirectto(state,"/frmcontrol.zhtml");
     return 0;
}


//----------------------------------------------------------------------------
int bt_pruebamail(HttpState* state)
//----------------------------------------------------------------------------
{
   SendMensPrueba();
   cgi_redirectto(state,"/");
   return 0;
}


//-------------------------------------------------------
int bt_reset(HttpState* state)
//-------------------------------------------------------
{
     CoBegin(&cbtReset);
     cgi_redirectto(state,"/");
     return 0;

}

//----------------------------------------------------------------------------
int link_login(HttpState* state)
//----------------------------------------------------------------------------
{
    cgi_redirectto(state,"/frmlogin.zhtml");
    return 0;
}



//----------------------------------------------------------------------------
int link_logout(HttpState* state)
//----------------------------------------------------------------------------
{
     bitPassOK=0;
     strcpy(wPassInput,"");
     cgi_redirectto(state,"/");
     return 0;
}



SSPEC_RESOURCETABLE_START
   SSPEC_RESOURCE_ZMEMFILE("/index.zhtml", index_zhtml),
   SSPEC_RESOURCE_ZMEMFILE("/func.js", func_java),
	SSPEC_RESOURCE_XMEMFILE("/estilos.css", estilos_css),
	SSPEC_RESOURCE_XMEMFILE("/variables.xml", variables_xml),
   SSPEC_RESOURCE_ZMEMFILE("/frmred.zhtml", frmred_zhtml),
   SSPEC_RESOURCE_ZMEMFILE("/frmcontrol.zhtml", frmcontrol_zhtml),
   SSPEC_RESOURCE_ZMEMFILE("/frmlogin.zhtml", frmlogin_zhtml),
   SSPEC_RESOURCE_ZMEMFILE("/ftemp.zhtml", frmtemp_zhtml),
   SSPEC_RESOURCE_ZMEMFILE("/ftiempos.zhtml", frmtiempos_zhtml),
   SSPEC_RESOURCE_ZMEMFILE("/frmmens.zhtml",frmmens_zhtml),
	SSPEC_RESOURCE_ZMEMFILE("/frmreset.zhtml",frmreset_zhtml),
   SSPEC_RESOURCE_ZMEMFILE("/iec/fparams.zhtml",fparams_zhtml),
#ifdef INFRARROJO
	SSPEC_RESOURCE_ZMEMFILE("/ayudas.zhtml", ayudas_zhtml),
#endif

   SSPEC_RESOURCE_XMEMFILE("/Encabe2.jpg",encabe2),
//----------------------------------------------------------------
   SSPEC_RESOURCE_FUNCTION("/btiniciar.cgi", bt_iniciar),
	SSPEC_RESOURCE_FUNCTION("/btreset.cgi", bt_reset),
	SSPEC_RESOURCE_FUNCTION("/btpruebamail.cgi", bt_pruebamail),
	SSPEC_RESOURCE_FUNCTION("/btdosequiposon.cgi", bt_dosequiposon),
	SSPEC_RESOURCE_FUNCTION("/btiniseccompen.cgi", bt_initseccompen),
//-------------------------------------------------------------
	SSPEC_RESOURCE_FUNCTION("/linklogout.cgi",link_logout),
   SSPEC_RESOURCE_FUNCTION("/linklogin.cgi",link_login),
//-------------------------------------------------------------
   SSPEC_RESOURCE_FUNCTION("/iec/btrstltemp.cgi", bt_rstltemp),
   SSPEC_RESOURCE_FUNCTION("/iec/btdefault.cgi", bt_default),
   SSPEC_RESOURCE_FUNCTION("/iec/btsimulcom.cgi", bt_simulcom),
   SSPEC_RESOURCE_FUNCTION("/iec/btbajacomp.cgi", bt_bajacomp),
   SSPEC_RESOURCE_FUNCTION("/iec/btsimulht.cgi", bt_simulht),
	SSPEC_RESOURCE_FUNCTION("/iec/btsimultn.cgi", bt_simultn),
	SSPEC_RESOURCE_FUNCTION("/iec/bthreset.cgi", bt_hreset),
	SSPEC_RESOURCE_FUNCTION("/iec/btdemo.cgi", bt_demo),
	SSPEC_RESOURCE_FUNCTION("/iec/btwork.cgi", bt_work),
   SSPEC_RESOURCE_FUNCTION("/iec/btsilbuzz.cgi", bt_silactbuzzer),
//-------------------------------------------------------------
#ifdef INFRARROJO
   SSPEC_RESOURCE_ZMEMFILE("/fcomirojo.zhtml",fcomirojo_zhtml),
	SSPEC_RESOURCE_FUNCTION("/grcr1on.cgi", graba_cr1_ON),
	SSPEC_RESOURCE_FUNCTION("/grcr1off.cgi", graba_cr1_OFF),
	SSPEC_RESOURCE_FUNCTION("/grcr1reenc.cgi", graba_cr1_Reenc),

	SSPEC_RESOURCE_FUNCTION("/grcr2on.cgi", graba_cr2_ON),
	SSPEC_RESOURCE_FUNCTION("/grcr2off.cgi", graba_cr2_OFF),
	SSPEC_RESOURCE_FUNCTION("/grcr2reenc.cgi", graba_cr2_Reenc),

	SSPEC_RESOURCE_FUNCTION("/testcr1on.cgi", test_cr1_ON),
	SSPEC_RESOURCE_FUNCTION("/testcr1reenc.cgi", test_cr1_Reenc),
	SSPEC_RESOURCE_FUNCTION("/testcr1off.cgi", test_cr1_OFF),
	SSPEC_RESOURCE_FUNCTION("/testcr2on.cgi", test_cr2_ON),
	SSPEC_RESOURCE_FUNCTION("/testcr2reenc.cgi", test_cr2_Reenc),
	SSPEC_RESOURCE_FUNCTION("/testcr2off.cgi", test_cr2_OFF),
//-------------------------------------------------------------
	SSPEC_RESOURCE_FUNCTION("/inicalicr1.cgi", inicali_cr1),
	SSPEC_RESOURCE_FUNCTION("/inicalicr2.cgi", inicali_cr2),

	SSPEC_RESOURCE_FUNCTION("/regcalicr1.cgi", regcali_cr1),
	SSPEC_RESOURCE_FUNCTION("/regcalicr2.cgi", regcali_cr2),
#endif


#ifdef SNMP_ENABLED
   SSPEC_RESOURCE_ZMEMFILE("/frmsnmp.zhtml",frmsnmp_zhtml),
#endif


SSPEC_RESOURCETABLE_END

// The following line defines an "admin" group, which will be used to protect
// certain variables.  This must be defined before we register the variables
// below.
#web_groups inecontrol,admin



// Variables de temperatura y humedad para las comparaciones---
shared float vintLectTemp; //valor de temperatura medida que se muestra
#web vintLectTemp // despues ver lo del los grupos de acceso
shared float vintTempPura; // lectura real de temperatura
#web vintTempPura;
shared float vintTempAnt; // lectura anterior
#web vintTempAnt

shared float vintLectHum;
#web vintLectHum
int wMalTemp;

int wContSense;

#web config.vintTempNormal (($config.vintTempNormal < 40))// umbral de temperatura Normal
#web config.vintTempAlta// umbral de temperatura alta
#web config.vintTempDeltaComp // temperatura de compensacion

#web config.vintTHEsperaTemp
#web config.vintTHEsperaTComp

#web_update  config.vintTempNormal,config.vintTempAlta,config.vintTempDeltaComp,config.vintTHEsperaTemp,config.vintTHEsperaTComp  CargaDatTemp
//-----------------------------------------------------

char bitEsperaTemp; // se pone en 1 si tiene que esperar para medir temperatura
char bitRecentConmut; // se pone en 1 cuando conmutan los aires para dar tiempo de enfriar si hay alta temp
int bitEsperaCompen; // se pone en 1 si tiene que esperar para medir temperatura
						// por compensacion



//------------------------------------------------------------------------------
int mail_generator(char * buf, int len, longword offset,int flags, void * dhnd_data)
//------------------------------------------------------------------------------
{

	auto int * my_data;

	if (flags == SMTPDH_OUT) {
		if (lineamail > 10)
			return 0;	// EOF after 500 bytes

		my_data = (int *)dhnd_data;	// Access our opaque data

		// Note that a '.' at the beginning of a line is deleted by the mail server.
		// Also, email normally requires lines to be terminated with \r\n not just \n.

      switch (lineamail){
         case 1:
//		sprintf(buf, ".. Line %d: The quick brown fox jumps over the lazy dog.  %lu\r\n", *my_data, offset);
 			    sprintf(buf,"Estado del Sistema: %s \r\n",RecEstGral[widGral].DescEstado);
             break;
         case 2:
			    sprintf(buf,"Temp.= %5.1f Gr (%s) - Hum.= %5.1f%% \r\n",vintLectTemp,wEstTemp,vintLectHum);
             break;
         case 3:
             sprintf(buf," %s   %s  \r\n",RecEstTemp[widTemp].DescEstTemp,RecEst220[wid220].DescEst220);
             break;
         case 4:
 			    sprintf(buf,"\r\n Equipo 1: %s, Equipo 2: %s, Secuenciador: %s \r\n",wEstEq1,wEstEq2,wEstSec);
//             sprintf(buf," %s \r\n", RecEst220[wid220].DescEst220);
             break;
         case 5:
 			    sprintf(buf," %s \r\n",RecEstSec[widSec].DescEstSec);
             break;
         case 6:
             sprintf(buf," \r\n Parámetros : Temp Alta > %d Gr | Temp Normal < %d Gr \r\n\r\n",config.vintTempAlta,config.vintTempNormal);
             break;
         case 7:
 			    sprintf(buf," \r\n");
             break;
         case 8:
             sprintf(buf," %s -( Cod:  %d ).- \r\n",wTipoReporte,wCodeEvento);
             break;
         case 9:
 			    sprintf(buf," \r\n");
             break;
         case 10:
 			    sprintf(buf,"(Informe Generado por IneControl en forma automatica) \r\n");
             break;




      }

      (lineamail)++;

		(*my_data)++;	// Increment line number (data is not opaque to us!).

		return(strlen(buf));
	}
	return -1;	// Indicate don't understand or care about this flag
}


//----------------------------------------------------------------------
void CargaDatParam(void) // carga los nuevos datos de la red
//----------------------------------------------------------------------
{
  Save_Config();
}


//----------------------------------------------------------------------
void CargaDatTemp(void) // carga los nuevos datos de la red
//----------------------------------------------------------------------
{
  config.vintTempNormal=config.vintTempAlta-2;
  config.vintTempDeltaComp=config.vintTempAlta-1;
  Save_Config();
}

//----------------------------------------------------------------------
void CargaDatSense(void) // carga valor de Cant de mediciones de temperatura
//----------------------------------------------------------------------
{
  Save_Config();
}

//----------------------------------------------------------------------
void CargaDatModiCodIR (void) // carga modificacion registros IR
//----------------------------------------------------------------------
{
  Save_Config();
}


//----------------------------------------------------------------------
void CargaDatDivCr(void)
//----------------------------------------------------------------------
{
  Save_Config();
}


//----------------------------------------------------------------------
void CargaDatRed(void) // carga los nuevos datos de la red
//----------------------------------------------------------------------
{
   if (bitPassOK==1){
     ifconfig(IF_ETH0, IFS_IPADDR, aton(config.wdirip),
              IFS_NETMASK, aton(config.wmask),
              IFS_ROUTER_SET, aton(config.wrouter),
              IFS_NAMESERVER_SET, aton(config.wdnssrvr),
              IFS_UP,
              IFS_END);
     Save_Config();
   }
}

//---------------------------------------------------------------------
void CargaDatMens(void) // carga nuevos mensajes
//---------------------------------------------------------------------
{
// CoReset(&cMensStatus);
 intTimSMensStat=config.vintTimRepStat*config.kTimRepStat;
 intTimSMensTemp=config.vintTimRepTemp*config.kTimRepTemp;

 if (bitPassOK==1){
    Save_Config();
 }
}



#ifdef SNMP_ENABLED
	//---------------------------------------------------------------------
	void CargaDatSnmp(void) // carga datos SNMP
	//---------------------------------------------------------------------
	{
   	if (config.inthabisnmp==1){
      	snmp_stop();
	 	}else{
   		snmp_start();
	   }

		 if (bitPassOK==1){
   		 Save_Config();
		 }
	}
#endif

//---------------------------------------------------------------------
void CargaNewPass(void) // carga nueva password de login
//---------------------------------------------------------------------
{
	if (strcmp(config.wPassRegistrada,wOldPass)==0)
   {
      strcpy(OldPassConfirm,"OK");
      if (strcmp(wNewPass,wRetNewPass)==0)
      {
          strcpy(NewPassConfirm,"");
          strcpy(OldPassConfirm,"");
          strcpy(config.wPassRegistrada,wNewPass);
          Save_Config();
          strcpy(wOldPass,"");
          strcpy(wNewPass,"");
          strcpy(wRetNewPass,"");
      }
      else
      {
        strcpy(NewPassConfirm,"No Coincide");
      }
   }
   else
   {
     strcpy(OldPassConfirm,"Mal");
     strcpy(wNewPass,"");
     strcpy(wOldPass,"");

   }

}

//---------------------------------------------------------------------
void VerifyPass(void) // verifica password ingresada
//---------------------------------------------------------------------
{
	if (strcmp(config.wPassRegistrada,wPassInput)==0){
      bitPassOK=1;
   }else{
      bitPassOK=0;
   }

}

//---------------------------------------------------------------------
int CalcDeltaTemp(int pTipo) // calculo de delta temperatura para pruebas
//---------------------------------------------------------------------
{
      int delta;

      if (pTipo==1){ // < normal
           delta = config.vintTempNormal-vintTempPura-2;
      }

      if (pTipo==2){ // > Compen
           delta = config.vintTempDeltaComp-vintTempPura+1;
      }

      if (pTipo==3){// < Compen
           delta = config.vintTempDeltaComp-vintTempPura-1;
      }


      if (pTipo==4){// > Alta
           delta = config.vintTempAlta-vintTempPura+2;

      }
      return delta;

}


//----------------------------------------------------------------------
void convHMS(unsigned long seg) // convierte segundos a horas, minutos y segundos
//----------------------------------------------------------------------
{
   vSegundos=seg;
   vHoras=(long)vSegundos/3600UL;
   vSegundos=(long)vSegundos%3600UL;
   vMinutos=(long)vSegundos/60UL;
   vSegundos=(long)vSegundos%60UL;

}

//----------------------------------------------------------------------
void VueltaSecuenc(void) //Determina continuación de secuenciado según el tiempo que falte
//----------------------------------------------------------------------
{
     if (bitMinConmut==0){
           vintHoraConmut=SetTimer(vintTRestante);
     }else{
           vintHoraConmut=SetTimer(vintTSConmut1);
           if (UltimoEqOn==1){
              UltimoEqOn=2;
           }else{
              UltimoEqOn=1;
           }
     }
}




//---------------------------------------------------------------------
void defaestados() // para los estados que se muestran en frmcontrol
//---------------------------------------------------------------------
{
   EqOn=0;  // ojo, selaire NO TIENE QUE VER siempre con EqOn
   UltimoEqOn=0;
   strcpy(wAireSel,"AA1");
   strcpy(wSelAireInit,"AA1");
}


//---------------------------------------------------------------------
void ConfiPorts()
//---------------------------------------------------------------------
{
   WrPortI(PBDDR,&PBDDRShadow,0xff);   // configura port B como salida
   WrPortI(PBDR,&PBDRShadow,0xf0);     // escribe f0 en port B para apagar todos los leds

   BitWrPortI(PCDR,&PCDRShadow,0,2); // por éste bit sale la data del ir
   //-----------------------------------------------
  //-Configura puerto C

	BitWrPortI(PCDR,&PCDRShadow,0,coAlExterna); // desactiva alarma externa
//-Configura puerto F

   BitWrPortI(PFDDR,&PFDDRShadow,0,fiLowBattery); // deteccion LowBattery
   BitWrPortI(PFDDR,&PFDDRShadow,1,foCorteBaja); // Corte por baja tension batería
   BitWrPortI(PFDDR,&PFDDRShadow,0,fiAcFail); // deteccion falla de linea por puerto F
   BitWrPortI(PFDDR,&PFDDRShadow,1,foHighTemp); // Indicador HT
   BitWrPortI(PFDDR,&PFDDRShadow,1,foLedAA1); // Indicador AA1
   BitWrPortI(PFDR,&PFDRShadow,Off,foHighTemp); // apaga los leds
   BitWrPortI(PFDR,&PFDRShadow,Off,foLedAA1); // apaga los leds

	BitWrPortI(PFDDR,&PFDDRShadow,1,foIrClock); // clock de salida para IR

	WrPortI(SDCR,&SDCRShadow,0xAC);             // que se yo, vio ?



//-Configura puerto B
   BitWrPortI(PBDDR,&PBDDRShadow,1,boLedAA2);
   BitWrPortI(PBDDR,&PBDDRShadow,1,boAireFijo);
   BitWrPortI(PBDR,&PBDRShadow,Off,boLedAA2); // apaga los leds
   BitWrPortI(PBDR,&PBDRShadow,Off,boAireFijo); // apaga los leds

//-Configura puerto E

   BitWrPortI(PEDDR,&PEDDRShadow,0,eiDemo);
   BitWrPortI(PEDDR,&PEDDRShadow,0,eiSilReset);
   BitWrPortI(PEDDR,&PEDDRShadow,0,eiAcFail);
   BitWrPortI(PEDDR,&PEDDRShadow,1,eoBuzzer);

//-- Configura puertos para lectura y escritura de SHT71
   BitWrPortI(PEDDR,&PEDDRShadow,1,SCK);       // se agregó para utilizar el port E como clock

   BitWrPortI (PDDR,&PDDRShadow,0,DATA );		// DATA =0
   BitWrPortI(PDDCR, &PDDCRShadow,1,DATA);		// DATA = open drain

}

#ifdef INFRARROJO
	//----------------------------------------------------------------------------
	void ReEncenderAA(void)
	//----------------------------------------------------------------------------
	{
	    if (EqOn==1)
   	 {
			 if (config.vintTipoCr1==2) { //tipo TV
				Emite_IR("cr1reenc",1);
          }else{
	   	 	Emite_IR("cr1on",1);
          }
	    }
		 if (EqOn==2)
	    {
			 if (config.vintTipoCr2==2) { //tipo TV
				Emite_IR("cr2reenc",2);
          }else{
	   	 	Emite_IR("cr2on",2);
          }
	    }
       return;
 	}
#endif


//----------------------------------------------------------------------------
scofunc AA1ONExec(void)
//----------------------------------------------------------------------------
{

    #ifdef INFRARROJO
		 Emite_IR("cr1on",1);
    #endif

    #ifdef CONTACTOR
       BitWrPortI(PBDR,&PBDRShadow,1,boContAA1);
    #endif

    #ifdef BOTON
       BitWrPortI(PBDR,&PBDRShadow,1,boContAA1);
    #endif


    BitWrPortI(PFDR,&PFDRShadow,On,foLedAA1);

    return 0;
}
//----------------------------------------------------------------------------
scofunc AA1OFFExec(void)
//----------------------------------------------------------------------------
{
    #ifdef INFRARROJO
	    Emite_IR("cr1off",1);
    #endif

    #ifdef CONTACTOR
       BitWrPortI(PBDR,&PBDRShadow,0,boContAA1);
    #endif

    #ifdef BOTON
       //BitWrPortI(PBDR,&PBDRShadow,1,boContAA1);
    #endif

    BitWrPortI(PFDR,&PFDRShadow,Off,foLedAA1);
    return 0;
}
//----------------------------------------------------------------------------
scofunc AA2ONExec(void)
//----------------------------------------------------------------------------
{
//    BitWrPortI(PBDR,&PBDRShadow,0,boIrAA1);
//    BitWrPortI(PBDR,&PBDRShadow,1,boIrAA2);

    #ifdef INFRARROJO
	    Emite_IR("cr2on",2);
    #endif

    #ifdef CONTACTOR
       BitWrPortI(PBDR,&PBDRShadow,1,boContAA2);
    #endif

    #ifdef BOTON
       //BitWrPortI(PBDR,&PBDRShadow,1,boContAA1);
    #endif

    BitWrPortI(PBDR,&PBDRShadow,On,boLedAA2);

    return 0;
}

//----------------------------------------------------------------------------
scofunc AA2OFFExec(void)
//----------------------------------------------------------------------------
{

    #ifdef INFRARROJO
	    Emite_IR("cr2off",2);
    #endif

    #ifdef CONTACTOR
       BitWrPortI(PBDR,&PBDRShadow,0,boContAA2);
    #endif

    #ifdef BOTON
       //BitWrPortI(PBDR,&PBDRShadow,1,boContAA1);
    #endif

    BitWrPortI(PBDR,&PBDRShadow,Off,boLedAA2);
    return 0;
}


//----------------------------------------------------------------------------
scofunc PitoBuzz(int pbuzz,int prep)
//----------------------------------------------------------------------------
{
    int i,rep;
    rep=0;
    while( rep < prep){
       BitWrPortI(PEDR,&PEDRShadow,1,eoBuzzer);
        for (i=1;i<pbuzz;i++){
        }
        BitWrPortI(PEDR,&PEDRShadow,0,eoBuzzer);
        for (i=1;i<pbuzz;i++){
        }
        rep++;
    }
}

//---------------------------------------------------------------------
scofunc ApagarEquipos(char *pSender)
//---------------------------------------------------------------------
{

  UltimoEqOn=EqOn; //


// Para apagar con SPI
//
//  Apaga equipo 1
  UltimoEqOn=EqOn; //
   if (config.bitSistON == 1) {
//	   if (strcmp(pSender,"EnaRut")!=0){
 			if (UltimoEqOn == 1) {
	//  Apaga equipo 1 (manda el código solo si está encendido )
   		 	wfd AA1OFFExec();
    			waitfor(DelaySec(5));
     		}

 	  		if (UltimoEqOn == 2) {
// 		 Apaga equipo 1 (manda el código solo si está encendido )
     			wfd AA2OFFExec();
     			waitfor(DelaySec(5));
   		}

         if (UltimoEqOn == 12) {
	//  Apaga equipos 1y2 (manda el código solo si está encendido )
   		 	wfd AA1OFFExec();
    			waitfor(DelaySec(5));

				wfd AA2OFFExec();
    			waitfor(DelaySec(5));

     		}


//   	}
	}
    if (wFijo==801){
        if (EqOn==1)
          ActEstadosEq("Off","EqMal");
        if (EqOn==2)
          ActEstadosEq("EqMal","Off");
        if (EqOn==12)
          ActEstadosEq("EqMal","EqMal");

    }else{
        ActEstadosEq("Off","Off");

    }
    EqOn=0;
    /* hay que ver que se pone acá---------------
    if (config.inthabimens==0) {
        SendMensNotif(config.wMensIniConmut);
    }*/

}

//-----------------------------------------------------------------------------
scofunc ApagarEquipoOff(int EquipoON)
//-----------------------------------------------------------------------------
{
  int EquiToOff;     // este es el equipo que finalmente se debe apagar

  if (EquipoON==1){
     EquiToOff=2;

  }
  if (EquipoON==2){
     EquiToOff=1;

  }

   if (EquiToOff==1){
       wfd AA1OFFExec();
       BitWrPortI(PFDR,&PFDRShadow,Off,foLedAA1);
       BitWrPortI(PBDR,&PBDRShadow,On,boLedAA2);
       ActEstadosEq("Off","On");
       EqOn=2;
   }

   if (EquiToOff==2){
       wfd AA2OFFExec();
       BitWrPortI(PFDR,&PFDRShadow,On,foLedAA1);
       BitWrPortI(PBDR,&PBDRShadow,Off,boLedAA2);
       ActEstadosEq("On","Off");
       EqOn=1;
   }

}

//---------------------------------------------------------------------
scofunc EsperaComp(unsigned long TEspera)
//---------------------------------------------------------------------
{
     // cambia cartelito estado
     // cambia cartelito equipo
     // detiene secuenciado
     // enciende los dos equipos
     // enviar mensaje Inicio Compensacion
     // cambiar cartelito sitio.
}

//---------------------------------------------------------------------
scofunc EsperaDetCostates(void)       // espera a que se detengan costates
//---------------------------------------------------------------------
{
    waitfor(isCoRunning(&cSendMail)==0);
}

//---------------------------------------------------------------------
void CrearReporte(int TipoReporte, int CodeEvento, int idTemp, int id220, int idSec, int idXX, int idGral)
//---------------------------------------------------------------------
{
   int RunRepoTemp;
   int RunRepoStat;
   RunRepoTemp,RunRepoStat=0;

   //   char *direnvio;
   wCodeEvento=CodeEvento;
   widTemp=idTemp;
   wid220=id220;
   widSec=idSec;
   widXX=idXX;
   widGral=idGral;


	if(config.inthabimens==0){
	   RunRepoTemp=isCoRunning(&cRepiteRepoTemp);
   	RunRepoStat=isCoRunning(&cMensStatus);

	   if (TipoReporte==1){
   	   strcpy(wTipoReporte,"Informe de Evento ");
      	CoReset(&cMensStatus);
       	CoReset(&cRepiteRepoTemp);

         strcpy(DirSep,config.wDestMensCrit);
  		}


	   if (TipoReporte==2){
   	   strcpy(wTipoReporte,"Informe de Status ");
         strcpy(DirSep,config.wDestMensNotif);
		}
//   DatosMensInforme();
	   CoBegin(&cSendMail);
   	if (RunRepoTemp==1)
      	CoBegin(&cRepiteRepoTemp);

	   if (RunRepoStat==1)
   	   CoBegin(&cMensStatus);
  	}

#ifdef SNMP_ENABLED
  if (config.inthabisnmp==0){
  		EnvioTraps(EstVariables.CodEmail);
  }
#endif


}

//---------------------------------------------------------------------
void CargaTiempos(void)
//---------------------------------------------------------------------
{
  //detener reloj
  CoReset(&cReloj);

// Multiplico vintTimXXX * kWorkDemo para pasar a segundos segun seleccion
// si esta en demo kWorkDemo=1 si no kWorkDemo=3600.

  vintTSPriConm=config.vintTimPriConm*config.kTimPriConm;// paso primera conmutacion a segundos
  vintTSConmut1=config.vintTimConmut1*config.kTimConmut1; // paso conmutacion1 a segundos
  //cargar la nueva hora de conmutación
  vintHoraConmut=SetTimer(vintTSPriConm);

//  intTimEsperaMedH=config.vintTimEsperaMed*kWorkDemo;
  vintTSEsperaTemp=config.vintTHEsperaTemp*config.kTimEsperaTemp;

  vintTSEsperaTComp=config.vintTHEsperaTComp*config.kTimEsperaComp;
//  vintTSEsperaTComp=60; // OJO CAMBIAR !!!!!!!!!!!!!

  intTimSMensStat=config.vintTimRepStat*config.kTimRepStat;
  intTimSMensTemp=config.vintTimRepTemp*config.kTimRepTemp;
  // iniciar reloj
//  CoBegin(&cReloj);

}
//---------------------------------------------------------------------
void CargaDatTiempos(void)
//---------------------------------------------------------------------
{

  Save_Config();
// al cargar nuevos valores para las conmutaciones se resetea
// el sistema
  CoBegin(&cbtReset);

  EqOn=config.intSelAAInit;
  if (EqOn == 1){  // activar infrarojo AA1 y desactivar AA2
       strcpy(wAireSel,"AA1");
  }
  if (EqOn == 2){ //
       strcpy(wAireSel,"AA2");
  }
}


//---------------------------------------------------------------------
unsigned long now()
//---------------------------------------------------------------------
{
   unsigned long hora;
   tm_rd(&rtc);  // lee la hora actual del sistema y lo pone en rtc
   hora=mktime(&rtc); // tiempo en segundos desde 1/1/1980
   return(hora);
}

//---------------------------------------------------------------------
unsigned long SetTimer(unsigned long tc)
//---------------------------------------------------------------------
{
   vintTRestante=tc;
   vintNow=now();
   tm_rd(&rtc);
   return(vintNow+vintTRestante);
}

//---------------------------------------------------------------------
void ResetVarEstados() //resetvariables
//---------------------------------------------------------------------
{
   wDeltaTemp=0;
   vintTempAnt=0;
   bitAcFail=100; // se pone en 1 si actualmente hay un ACFAil
   bitEsperaTemp=0;
   bitRecentConmut=0;
   bitEsperaCompen=0;
   bitMinConmut=0;
   bitTmpAlta=500; //
	config.vintTempNormal=config.vintTempAlta-2;
   config.vintTempDeltaComp=config.vintTempAlta-1;
   bitCompen=600;
   wFijo=800;
   bitSilBuzzer=1; // con 1 funciona el buzzer, con 0 no
   bitEnaRutTmp=1;
   bitEnaRutAcFail=1;
   NroMail=0;
   bitDosEqON=0;
   config.bitSistON=0;
   strcpy(wMailStatus,"");
   EstVariables.CodEmail=0;
   CoReset(&cLecTemp);
   CoBegin(&cLecTemp);

   CoReset(&cSilResetStat);
   CoBegin(&cSilResetStat);


   ActEstadosEq("Off","Off");
   ActEstadosTemp("OK");
   ActEstadosLinea("OK");
   ActEstadosCtrl("Activo","Detenido","X");

	#ifdef INFRARROJO

		strcpy(wBtCalAA1,"enabled");
		strcpy(wBtCalAA2,"enabled");
		strcpy(wBtRegCalAA1,"disabled");
		strcpy(wBtRegCalAA2,"disabled");
	#endif

}

//---------------------------------------------------------------------
void ResetVarPassword() //resetvariables
//---------------------------------------------------------------------
{
   bitPassOK=0;
   strcpy(wPassInput,"");
   strcpy(NewPassConfirm,"");
	strcpy(wOldPass,"");
	strcpy(wNewPass,"");
   strcpy(wRetNewPass,"");
	strcpy(OldPassConfirm,"");
	strcpy(NewPassConfirm,"");

//   HabiSendMail=0;

}

//---------------------------------------------------------------------
void CargaTextosMails()
//---------------------------------------------------------------------
{
   strcpy(RecEstGral[0].DescEstado,"OK .-");
   strcpy(RecEstGral[1].DescEstado,"MAL .-");
   strcpy(RecEstGral[2].DescEstado,"MAL - En Espera .-");
   strcpy(RecEstGral[3].DescEstado,"EN ESPERA .-");
   strcpy(RecEstGral[4].DescEstado,"MAL - SE REQUIERE INTERVENCION INMEDIATA .-");
   strcpy(RecEstGral[6].DescEstado,"OK - VERIFICAR EQUIPOS DE AIRE .-");
   strcpy(RecEstGral[7].DescEstado,"MAL - Un Equipo en falla .-");
   strcpy(RecEstGral[9].DescEstado,"Se reestablece estado OK .-");
//---------------------------------------------------------------------------

   strcpy(RecEst220[0].DescEst220," * Linea de 220v OK .-");
   strcpy(RecEst220[1].DescEst220," * Se ha interrumpido la linea de 220v .-");

   strcpy(RecEst220[8].DescEst220," * Se detiene sensado de falla de linea .-");   //   RecEst220[2].DescEstado=

   strcpy(RecEst220[9].DescEst220," * Se reestablecio linea de 220v .-");
//---------------------------------------------------------------------------

   sprintf(RecEstTemp[0].DescEstTemp," * Temperatura OK") ;
   sprintf(RecEstTemp[1].DescEstTemp," * Se Presento Temperatura Alta en Sala .-");
   sprintf(RecEstTemp[2].DescEstTemp," * Se Mantiene Alta Temperatura en Sala .-");
   sprintf(RecEstTemp[3].DescEstTemp," * Temperatura Algo Elevada. Se inicia compensacion .-");
   sprintf(RecEstTemp[4].DescEstTemp," * La Temperatura normal NO se ha reestablecido .-");
   sprintf(RecEstTemp[5].DescEstTemp," * La Temperatura se redujo, pero no se logro el valor esperado.-");
   sprintf(RecEstTemp[6].DescEstTemp," * La Temperatura se elevo, estando ambos equipos encendidos.-");
   sprintf(RecEstTemp[8].DescEstTemp," * Se Resetea Sensado de temperatura .-");
   sprintf(RecEstTemp[9].DescEstTemp," * Se Reestablecio Temperatura Normal .-");
//---------------------------------------------------------------------------

   strcpy(RecEstSec[0].DescEstSec," * No se modifica funcionamiento de los equipos .-");
   strcpy(RecEstSec[1].DescEstSec," * Se apagan ambos equipos .-");
   strcpy(RecEstSec[2].DescEstSec," * Se retoma secuenciado .-");
   strcpy(RecEstSec[3].DescEstSec," * Se demoro más tiempo del esperado en reestablecer temperatura normal .-");
   strcpy(RecEstSec[4].DescEstSec," * Se Encienden Ambos Equipos.-");
   strcpy(RecEstSec[5].DescEstSec," * Expiro Tiempo de espera .-");
   strcpy(RecEstSec[6].DescEstSec," * 1 solo equipo disponible .-");
   strcpy(RecEstSec[7].DescEstSec," * Se Conmuta a Equipo de Reserva y se deja Fijo .-");
   strcpy(RecEstSec[9].DescEstSec," * Ambos Equipos Encendidos.-");
   strcpy(RecEstSec[10].DescEstSec," * Se Secuencian Equipos.-");
   strcpy(RecEstSec[11].DescEstSec," * Se Inicia Secuenciado de Equipos.-");
   strcpy(RecEstSec[12].DescEstSec," * Se Resetea Secuenciador por acción del usuario .-");



//---------------------------------------------------------------------------

}

//---------------------------------------------------------------------
void SendMensPrueba()
//---------------------------------------------------------------------
{
   char BodyMens[20];
   char DirSep[71];


   while (isCoRunning(&cSendMail)==1){

   }

   strcpy(BodyMens,"Mensaje de Prueba");
   strcpy(DirSep,config.wDestMensCrit);

   smtp_data_handler(NULL,NULL,0);

   #ifdef USE_SMTP_AUTH
      	smtp_setauth (config.wsmtpcuentaval,config.wsmtppass);
   #endif

   smtp_setserver(config.wsmtpsrvr);
   direnvio=strtok(DirSep, ";");

   while (direnvio!=NULL) {
      smtp_sendmail(direnvio,config.wsmtpmailfrom,config.wAsunto,BodyMens);

      while(smtp_mailtick()==SMTP_PENDING){
      }

      if(smtp_status()==SMTP_SUCCESS){
             printf("\n\r\Message sent\n\r");
                 strcpy(wMailStatus," Ultimo e-mail enviado OK ");
             }else{
                 strcpy(wMailStatus," Ultimo e-mail con errores ");

             if(smtp_status()==SMTP_UNEXPECTED)
                   printf("\n\r\Error INESPERADO \n\r");
             else
                   printf("\n\r\Error sending message\n\r");
      }
      direnvio=strtok(NULL, ";");

   }
}

//---------------------------------------------------------------------
void SendMensLowBatt()
//---------------------------------------------------------------------
{
   char BodyMens[20];
   char DirSep[71];


   while (isCoRunning(&cSendMail)==1){

   }

   strcpy(BodyMens,"Baja Batería!.El sistema se apagará en 60 Seg");
   strcpy(DirSep,config.wDestMensCrit);

   smtp_data_handler(NULL,NULL,0);

   #ifdef USE_SMTP_AUTH
      	smtp_setauth (config.wsmtpcuentaval,config.wsmtppass);
   #endif

   smtp_setserver(config.wsmtpsrvr);
   direnvio=strtok(DirSep, ";");

   while (direnvio!=NULL) {
      smtp_sendmail(direnvio,config.wsmtpmailfrom,config.wAsunto,BodyMens);

      while(smtp_mailtick()==SMTP_PENDING){
      }

      if(smtp_status()==SMTP_SUCCESS){
             printf("\n\r\Message sent\n\r");
                 strcpy(wMailStatus," Ultimo e-mail enviado OK ");
             }else{
                 strcpy(wMailStatus," Ultimo e-mail con errores ");

             if(smtp_status()==SMTP_UNEXPECTED)
                   printf("\n\r\Error INESPERADO \n\r");
             else
                   printf("\n\r\Error sending message\n\r");
      }
      direnvio=strtok(NULL, ";");

   }
}

//---------------------------------------------------------------------
void ActEstadosEq(char *pwEstEq1,char *pwEstEq2)
//---------------------------------------------------------------------
{

   strcpy(wEstEq1,pwEstEq1);
   strcpy(wEstEq2,pwEstEq2);

}

//---------------------------------------------------------------------
void ActEstadosTemp(char *pwEstTemp)
//---------------------------------------------------------------------
{

   strcpy(wEstTemp,pwEstTemp);


}

//---------------------------------------------------------------------
void ActEstadosLinea(char *pwEstLinea)
//---------------------------------------------------------------------
{

   strcpy(wEstLinea,pwEstLinea);

}

//---------------------------------------------------------------------
void ActEstadosCtrl(char *pwEstSec,char *pwEstModo, char *pwEstDelayMed)  // estados controlador
//---------------------------------------------------------------------
{

   strcpy(wEstSec,pwEstSec);
   strcpy(wEstModo,pwEstModo);
   strcpy(wEstDelayMed,pwEstDelayMed);
}

#ifdef SNMP_ENABLED
	//---------------------------------------------------------------------
	void EnvioTraps(int nrotrap)
	//---------------------------------------------------------------------
	{
   	 if (strcmp(config.snmpIPtrap1,"")!=0) {
      	  	trapdest_ip = aton(config.snmpIPtrap1);
				snmp_trap(trapdest_ip, SNMP_TRAPDEST, nrotrap, 10, trapindices);
	    }

	    if (strcmp(config.snmpIPtrap2,"") !=0){
				trapdest_ip = aton(config.snmpIPtrap2);
  				snmp_trap(trapdest_ip, SNMP_TRAPDEST, nrotrap, 10, trapindices);
	    }

   	 if (strcmp(config.snmpIPtrap3,"") !=0){
				trapdest_ip = aton(config.snmpIPtrap3);
  				snmp_trap(trapdest_ip, SNMP_TRAPDEST, nrotrap, 10, trapindices);
	    }
	}
#endif



//**** Funciones para lectura de temperatura para SHT71
//----------------------------------------------------------------------------------
char s_write_byte(unsigned char value)
//----------------------------------------------------------------------------------
// writes a byte on the Sensibus and checks the acknowledge
{
  unsigned char i,error;
  for (i=0x80;i>0;i/=2)             //shift bit for masking
  { if (i & value)						//masking value with i
	    BitWrPortI (PDDDR,&PDDDRShadow,0,DATA );	 //write to SENSI-BUS DATA=1 (pull-up)
    else
	    BitWrPortI (PDDDR,&PDDDRShadow,1,DATA );  //write to SENSI-BUS DATA=0
    BitWrPortI (PEDR,&PEDRShadow,1,SCK ); //clk for SENSI-BUS
    BitWrPortI (PEDR,&PEDRShadow,0,SCK );
  }
  BitWrPortI (PDDDR,&PDDDRShadow,0,DATA ); //release DATA-line
  BitWrPortI (PEDR,&PEDRShadow,1,SCK ); //clk #9 for ack
  error=BitRdPortI(PDDR,DATA);          //check ack (DATA will be pulled down by SHT11)
  BitWrPortI (PEDR,&PEDRShadow,0,SCK );
  return error;                     //error=1 in case of no acknowledge
}

//----------------------------------------------------------------------------------
char s_read_byte(unsigned char ack)
//----------------------------------------------------------------------------------
// reads a byte form the Sensibus and gives an acknowledge in case of "ack=1"
{
  unsigned char i,val;

  val=0;
  BitWrPortI (PDDDR,&PDDDRShadow,0,DATA ); //release DATA-line
  for (i=0x80;i>0;i/=2){	             //shift bit for masking
    BitWrPortI (PEDR,&PEDRShadow,1,SCK ); //clk for SENSI-BUS
    if (BitRdPortI(PDDR,DATA)) val=(val | i);        //read bit
    BitWrPortI (PEDR,&PEDRShadow,0,SCK );
  }
  BitWrPortI (PDDDR,&PDDDRShadow,ack,DATA ); //in case of "ack==1" pull down DATA-Line
  BitWrPortI (PEDR,&PEDRShadow,1,SCK ); //clk #9 for ack
  BitWrPortI (PEDR,&PEDRShadow,0,SCK );
  BitWrPortI (PDDDR,&PDDDRShadow,0,DATA ); //release DATA-line
  return val;
}

//----------------------------------------------------------------------------------
void s_transstart(void)
//----------------------------------------------------------------------------------
// generates a transmission start
//       _____         ________
// DATA:      |_______|
//           ___     ___
// SCK : ___|   |___|   |______
{

  BitWrPortI (PDDDR,&PDDDRShadow,0,DATA );  //Initial state
  BitWrPortI (PEDR,&PEDRShadow,0,SCK );
  BitWrPortI (PEDR,&PEDRShadow,1,SCK );
  BitWrPortI (PDDDR,&PDDDRShadow,1,DATA );
  BitWrPortI (PEDR,&PEDRShadow,0,SCK );
  BitWrPortI (PEDR,&PEDRShadow,1,SCK );
  BitWrPortI (PDDDR,&PDDDRShadow,0,DATA );
  BitWrPortI (PEDR,&PEDRShadow,0,SCK );
}

//----------------------------------------------------------------------------------
void s_connectionreset(void)
//----------------------------------------------------------------------------------
// communication reset: DATA-line=1 and at least 9 SCK cycles followed by transstart
//       _____________________________________________________         ________
// DATA:                                                      |_______|
//          _    _    _    _    _    _    _    _    _        ___     ___
// SCK : __| |__| |__| |__| |__| |__| |__| |__| |__| |______|   |___|   |______
{
  unsigned char i;

  BitWrPortI (PDDDR,&PDDDRShadow,0,DATA ); //Initial state
  BitWrPortI (PEDR,&PEDRShadow,0,SCK );
  for(i=0;i<9;i++)                  //9 SCK cycles
  {
  BitWrPortI (PEDR,&PEDRShadow,1,SCK );
  BitWrPortI (PEDR,&PEDRShadow,0,SCK );
  }
  s_transstart();                   //transmission start
}

//----------------------------------------------------------------------------------
char s_softreset(void)
//----------------------------------------------------------------------------------
// resets the sensor by a softreset
{

  s_connectionreset();              //reset communication
  return s_write_byte(RESET);       //send RESET-command to sensor
							  //=1 in case of no response from the sensor
}

//----------------------------------------------------------------------------------
char s_read_statusreg(unsigned char *p_value, unsigned char *p_checksum)
//----------------------------------------------------------------------------------
// reads the status register with checksum (8-bit)
{
  unsigned char error;
  s_transstart();                   //transmission start
  error=s_write_byte(STATUS_REG_R); //send command to sensor
  *p_value=s_read_byte(ACK);        //read status register (8-bit)
  *p_checksum=s_read_byte(noACK);   //read checksum (8-bit)
  return error;                     //error=1 in case of no response form the sensor
}

//----------------------------------------------------------------------------------
char s_write_statusreg(unsigned char *p_value)
//----------------------------------------------------------------------------------
// writes the status register with checksum (8-bit)
{
  unsigned char error;
  s_transstart();                   //transmission start
  error=s_write_byte(STATUS_REG_W);//send command to sensor
  error+=s_write_byte(*p_value);    //send value of status register
  return error;                     //error>=1 in case of no response form the sensor
}

//----------------------------------------------------------------------------------
cofunc char s_measure(unsigned char *p_value, unsigned char *p_checksum, unsigned char mode)
//----------------------------------------------------------------------------------
// makes a measurement (humidity/temperature) with checksum
{
  unsigned error;
  unsigned int i;
  static unsigned long timer;

  s_transstart();                   //transmission start
  error=0;
  switch(mode){                     //send command to sensor
    case TEMP: error=s_write_byte(MEASURE_TEMP);
    				break;
    case HUMI: error=s_write_byte(MEASURE_HUMI);
    				break;
    default  :
    				break;
  }
  timer=MS_TIMER;										// restart timeout
  while(BitRdPortI(PDDR,DATA)) {    //wait until sensor has finished the measurement
     if((MS_TIMER-timer)<SHT_TMOUT)
	  	  	yield;
     else return(error++);	 			// or timeout (~2 sec.) is reached
  }
  *(p_value+1)  =s_read_byte(ACK);    //read the first byte (MSB)
  *(p_value)=s_read_byte(ACK);    //read the second byte (LSB)
  *p_checksum =s_read_byte(noACK);  //read checksum
  return error;
}

//----------------------------------------------------------------------------------------
void calc_sht11(float *p_humidity ,float *p_temperature)
//----------------------------------------------------------------------------------------
// calculates temperature [°C] and humidity [%RH]
// input :  humi [Ticks] (12 bit)
//          temp [Ticks] (14 bit)
// output:  humi [%RH]
//          temp [°C]
{ const static float C1=-4.0;              // for 12 Bit
  const static float C2=+0.0405;           // for 12 Bit
  const static float C3=-0.0000028;        // for 12 Bit
  const static float T1=+0.01;             // for 14 Bit @ 5V
  const static float T2=+0.00008;           // for 14 Bit @ 5V

  float rh;             				// rh:      Humidity [Ticks] 12 Bit
  float t;					            // t:       Temperature [Ticks] 14 Bit
  float rh_lin;                     // rh_lin:  Humidity linear
  float rh_true;                    // rh_true: Temperature compensated humidity
  float t_C;                        // t_C   :  Temperature [°C]

  rh=*p_humidity;
  t=*p_temperature;
  t_C=t*0.01 - 40;                  //calc. temperature from ticks to [°C]
  rh_lin=C3*rh*rh + C2*rh + C1;     //calc. humidity from ticks to [%RH]
  rh_true=(t_C-25)*(T1+T2*rh)+rh_lin;   //calc. temperature compensated humidity [%RH]
  if(rh_true>100)rh_true=100;       //cut if the value is outside of
  if(rh_true<0.1)rh_true=0.1;       //the physical possible range

  *p_temperature=t_C;               //return temperature [°C]
  *p_humidity=rh_true;              //return humidity[%RH]
}

//--------------------------------------------------------------------
float calc_dewpoint(float h,float t)
//--------------------------------------------------------------------
// calculates dew point
// input:   humidity [%RH], temperature [°C]
// output:  dew point [°C]
{ float logEx,dew_point;
  logEx=0.66077+7.5*t/(237.3+t)+(log10(h)-2);
  dew_point = (logEx - 0.66077)*237.3/(0.66077+7.5-logEx);
  return dew_point;
}

#ifdef SNMP_ENABLED

	//--------------------------------------------------------------------
	void arbolSNMP(void)         // SA712
	//--------------------------------------------------------------------
	{
	// Password para las comunidades default
		snmp_set_dflt_communities(config.snmpReadComm,config.snmpWriteComm,config.snmpTrapComm);
	//snmp_set_dflt_communities(config.snmpReadComm,config.snmpWriteComm,config.snmpTrapComm);
	// Set p to be a pointer to _p, for calling convenience.
   	p = &_p;

	// Set parameter structure to default initial state (required).
	   snmp_init_parms(p);

   	p = snmp_append_parse_stem(p, "1");  //Set  SNMP_ENTERPRISE.INECOIEC-SEMI.sa712wir

	//==== variables de solo lectura
		p = snmp_set_access(p, SNMP_PUBLIC_MASK|SNMP_PRIVATE_MASK, 0);
	   p = snmp_add_int(p, "2.1.1.0", &intLectTemp);
   	trapindices[0] = snmp_last_index(p);

   	p = snmp_add_int(p, "2.1.2.0", &intLectHum);
	   trapindices[1] = snmp_last_index(p);

	   p = snmp_add_str(p, "2.2.1.0", &wEstEq1,5);
   	trapindices[2] = snmp_last_index(p);

	   p = snmp_add_str(p, "2.2.2.0", &wEstEq2,5);
   	trapindices[3] = snmp_last_index(p);

	   p = snmp_add_str(p, "2.3.1.0", &wEstLinea,5);
		trapindices[4] = snmp_last_index(p);

	   p = snmp_add_str(p, "2.3.2.0", &wEstTemp,5);
		trapindices[5] = snmp_last_index(p);

	   p = snmp_add_int(p, "2.3.3.0", &EstVariables.CodEmail);
		trapindices[6] = snmp_last_index(p);

	   p = snmp_add_str(p, "2.3.4.0", &wEstSec,5);
  		trapindices[7] = snmp_last_index(p);

	   p = snmp_add_str(p, "2.3.5.0", &wEstModo,10);
		trapindices[8] = snmp_last_index(p);

	   p = snmp_add_str(p, "2.3.6.0", &wEstDelayMed,10);
 		trapindices[9] = snmp_last_index(p);

//==== variables de lectura y escritura
		p = snmp_set_access(p, SNMP_PUBLIC_MASK|SNMP_PRIVATE_MASK, SNMP_PRIVATE_MASK);

   	p = snmp_add_int(p, "2.10.1.0", &config.vintTempAlta);
	   p = snmp_add_int(p, "2.10.2.0", &config.vintTempNormal);
	}
#endif

//----------------------------------------------------------------------------------
scofunc void s_meditemp(void)
//----------------------------------------------------------------------------------
{
    return ;
}




//****--Fin Funciones de lectura SHT71




main()
{
//****** Medicion de temperatura
   value humi_val,temp_val;     //humi_val y temp_val son del tipo Value que es un typedef
   float dew_point;
   unsigned char error,checksum;
   unsigned int i;
//***************************************
   int userid;
   int line_num; // es para el datahandler del SMTP (no se como funciona);
// DLM---
   int LecTempFin;

// para leer la mac ------------
  	char buffer[6];
   word iface;
//---------------------------------
// titulo sitio

	sprintf(TituloPags,"IneControl %s",wModelo);
//---------
   vHoras=0UL;
   vMinutos=0UL;
   vSegundos=0UL;

   ConfiPorts();
//	Montar_fat();

   strcpy(magTHPriConm," Horas");
   strcpy(magTHConmut1," Horas");
   strcpy(magTHEsperaTemp," Horas");
   strcpy(magTimMensStat,"Horas");
   strcpy(magTimMensTemp," Minutos");

//--- lee o escribe valores de default en la flash
   if (!BitRdPortI(PEDR,eiSilReset)) {
        while(!BitRdPortI(PEDR,eiSilReset)) {
        }
        memcpy(&config,&info_defaults,sizeof(Configuracion));
        Save_Config();

    }

   if (Load_Config()==0){
        memcpy(&config,&info_defaults,sizeof(Configuracion));
        Save_Config();
        Load_Config();
   }

#ifdef SNMP_ENABLED
   if (config.inthabisnmp==1){
      snmp_stop();
   }else{
   	snmp_start();
   }


   arbolSNMP();
#endif

   CoReset(&cLowBattery);
   BitWrPortI(PFDR,&PFDRShadow,1,foCorteBaja); // activa relé salida.
   CoReset(&cLecTemp);
   CoReset(&cTestAlarmas2Eq);
   CoReset(&cTestAlarmasFijoHT);
//   CoReset(&cVerifyTempAlta);
   CoReset(&cFallaLinea);

   CoReset(&cReloj);

   CoReset(&cMensStatus);
   CoReset(&cRepiteRepoTemp);
//   CoReset(&cInitCompen);
   CoReset(&cFinCompen);
   CoReset(&cbtReset);

   CoReset(&cBuzzer);
   BitWrPortI(PEDR,&PEDRShadow,0,eoBuzzer); // apaga el buzzer
   BitWrPortI(PBDR,&PBDRShadow,1,boIrAA1);
   BitWrPortI(PBDR,&PBDRShadow,1,boIrAA2);
   BitWrPortI(PCDR,&PCDRShadow,1,2);

   // --carga configuracion default
//  SegXHora=3600;
//  SegXMin=60;
//   delchar=0xFF;
//-------------------------------
   vintLectTemp=0;
   vintLectHum=0;
   intLectTemp=0;
   intLectHum=0;
   wMalTemp=0; // contador de temperatura mal
   wContSense=0; // contador de sensibilidad
//---------------------------------


   defaestados();

//******Medicion de temoeratura SHT71
//
//  BitWrPortI(PEDDR,&PEDDRShadow,1,SCK);       // se agregó para utilizar el port E como clock
//  BitWrPortI (PDDR,&PDDRShadow,0,DATA );		// DATA =0
//  BitWrPortI(PDDCR, &PDDCRShadow,1,DATA);		// DATA = open drain

   s_connectionreset();
//**************************************************

 //  EqOn=1; Creo que no hace falta porque lo seteamos en defaestados
   ResetVarEstados();
   ResetVarPassword();
   CargaTextosMails();

   CoBegin(&cSilResetStat);

   CoBegin(&cLowBattery);
   CoBegin(&cLecTemp); // vemos si queda habilitado
  	sock_init();
	if(pd_havelink(IF_ETH0)){
	   while (ifpending(IF_DEFAULT) == IF_COMING_UP) {
			tcp_tick(NULL);
		}
//  ActStatus();
   }

  for (iface = 0; iface < IF_MAX; ++iface)
      if (is_valid_iface(iface)) {
      	if (!ifconfig(0, IFG_HWA, buffer, IFS_END))
         	sprintf(wMacAddress,"%02x:%02x:%02x:%02x:%02x:%02x",
	            buffer[0],buffer[1],buffer[2],
	            buffer[3],buffer[4],buffer[5]);

 		 }



   http_init();
   tcp_reserveport(80);

// DLM

   DLM_Init("SA712W-CS 23-2 ... ");

   k=DLM_Reboot(UDPDNLD_MODE_RUN_OK); // this will not reboot
   if (k==UDPDNLD_MODE_RUN_PRIMARY)
   	printf("We are the primary program\n");
   else
   if (k==UDPDNLD_MODE_RUN_BACKUP)
  	  printf("We are the backup program.\n");
   else
   	printf("DLM_Reboot(UDPDNLD_MODE_RUN_OK) returned %d\n");
   init_file_data();



   http_set_path("/","index.zhtml");
   sspec_addrule("/", "Inecontrol Administración", inecontrol, inecontrol, SERVER_ANY,
                 SERVER_AUTH_BASIC, NULL);
	// The following two lines create an "admin" user and adds it to the admin
	// group.
   sspec_addrule("/iec", "Area Reservada", admin, admin, SERVER_ANY,
                 SERVER_AUTH_BASIC, NULL);

   userid = sauth_adduser("admin", "2639", SERVER_ANY);
   sauth_setusermask(userid, inecontrol, NULL);

   userid = sauth_adduser("nipapoch","a18473",SERVER_ANY);
   sauth_setusermask(userid, admin, NULL);

   // -Carga datos de la red
	if(pd_havelink(IF_ETH0)){
	   ifconfig(IF_ETH0, IFS_IPADDR, aton(config.wdirip),
              IFS_NETMASK, aton(config.wmask),
              IFS_ROUTER_SET, aton(config.wrouter),
              IFS_UP,
              IFS_END);
	 }
//   CoBegin(&cbtIniciar);
   for (;;){ // loop principal.

//-----------------------------------------------------------------------------
     costate {
//-----------------------------------------------------------------------------
    	if(pd_havelink(IF_ETH0)){
	       http_handler();
      }
     }

// DLM

//-----------------------------------------------------------------------------
     	costate {
//-----------------------------------------------------------------------------
			if(pd_havelink(IF_ETH0)){
      	  	DLM_Tick(); // this should be called at least once a second.
	   		tcp_tick(NULL); // usually in user's main loop somewhere
      	}
      }

//-----------------------------------------------------------------------------
     	costate {
//-----------------------------------------------------------------------------
		   EstVariables.cVerifyTempAlta=isCoRunning(&cVerifyTempAlta);
		   EstVariables.cVerifyTempCompen=isCoRunning(&cVerifyTempCompen);
		   EstVariables.cFallaLinea=isCoRunning(&cFallaLinea);
		   EstVariables.cMensStatus=isCoRunning(&cMensStatus);
		   EstVariables.cRepiteRepoTemp=isCoRunning(&cRepiteRepoTemp);
		   EstVariables.cEsperaTemp=isCoRunning(&cEsperaTemp);
		   EstVariables.cEsperaComp=isCoRunning(&cEsperaComp);
		   EstVariables.cSendMail=isCoRunning(&cSendMail);
		   EstVariables.cTestAlarmas2Eq=isCoRunning(&cTestAlarmas2Eq);
		   EstVariables.cTestAlarmasFijoHT=isCoRunning(&cTestAlarmasFijoHT);
		   EstVariables.bitAcFail=bitAcFail;
		   EstVariables.bitTmpAlta=bitTmpAlta;
		   EstVariables.bitEsperaTemp=bitEsperaTemp;
		   EstVariables.bitCompen=bitCompen;
		   EstVariables.bitEsperaCompen=bitEsperaCompen;
		   EstVariables.bitMinConmut=bitMinConmut;
		   EstVariables.wFijo=wFijo;
		   EstVariables.EqOn=EqOn;
		   EstVariables.EnaRutAcFail=bitEnaRutAcFail;
		   EstVariables.EnaRutTmp=bitEnaRutTmp;
		   EstVariables.wMalTemp=wMalTemp;
		   EstVariables.pfoCorteBaja=BitRdPortI(PFDR,foCorteBaja);
		   EstVariables.pfiLowBattery=BitRdPortI(PFDR,fiLowBattery);
     }
//--------------------------------------------------------
  costate cSilResetStat always_on {
//--------------------------------------------------------
		 if (!BitRdPortI(PEDR,eiSilReset)){
			  wfd PitoBuzz(20000,3);
//           CoReset(&cLecTemp);

           vintTResetPress=now()+5UL;

           while(!BitRdPortI(PEDR,eiSilReset)) {
   	     }

           if (config.bitSistON==0){
   	          // el sistema está apagado, hay que encenderlo
					  if (now()>vintTResetPress){
			   			 wfd PitoBuzz(20000,5);
                      CoBegin(&cbtDosEquiposON);
                     //enciende ambos aires
					  }else{
      			  		CoBegin(&cbtIniciar);
                 }//waitfor(isCoRunning(&cbtIniciar)==0);
           }else{
            // el sistema está encendido, hay que apagarlo
                 CoBegin(&cbtReset);
                 //waitfor(isCoRunning(&cbtReset)==0);
           }

     	   //BitWrPortI(PBDR,&PBDRShadow,On,boAireFijo);
//            CoBegin(&cLecTemp);
            CoReset(&cSilResetStat);

       }

 }

//--------------------------------------------------------
     costate cLecTemp always_on {
//--------------------------------------------------------
//**medicion de temperatura de SHT71
		LecTempFin=0;
      error=0;
      wfd i=s_measure((unsigned char*) &humi_val.i,&checksum,HUMI);  //measure humidity
      error+=i;
      wfd i=s_measure((unsigned char*) &temp_val.i,&checksum,TEMP);  //measure temperature
      error+=i;
      if(error!=0) s_connectionreset();                 //in case of an error: connection reset
      else
      { humi_val.f=(float)humi_val.i;                   //converts integer to float
          temp_val.f=(float)temp_val.i;                   //converts integer to float
          calc_sht11(&humi_val.f,&temp_val.f);            //calculate humidity, temperature
          dew_point=calc_dewpoint(humi_val.f,temp_val.f); //calculate dew point
          printf("temperature: %5.1fC, humidity: %5.1f%%, dew point: %5.1fC\n",temp_val.f,humi_val.f,dew_point);
      }
      //----------wait approx 1s to avoid heating up SHTxx------------------------------
      waitfor(DelaySec(1));
      //-----------------------------------------------------------------------------------
 //        wfd s_meditemp();
        vintTempPura=temp_val.f;
        if (vintTempAnt==0){
             vintTempAnt=vintTempPura;
        }else{
             while (wContSense < config.vintTempSense){
                 error=0;
                 wfd i=s_measure((unsigned char*) &humi_val.i,&checksum,HUMI);  //measure humidity
                 error+=i;
                 wfd i=s_measure((unsigned char*) &temp_val.i,&checksum,TEMP);  //measure temperature
                 error+=i;
                 if(error!=0) s_connectionreset();                 //in case of an error: connection reset
                 else
                 { humi_val.f=(float)humi_val.i;                   //converts integer to float
                      temp_val.f=(float)temp_val.i;                   //converts integer to float
                      calc_sht11(&humi_val.f,&temp_val.f);            //calculate humidity, temperature
                      dew_point=calc_dewpoint(humi_val.f,temp_val.f); //calculate dew point
                      printf("temperature: %5.1fC, humidity: %5.1f%%, dew point: %5.1fC\n",temp_val.f,humi_val.f,dew_point);
                 }
    //----------wait approx 1s to avoid heating up SHTxx------------------------------
                 waitfor(DelaySec(1));
    //-----------------------------------------------------------------------------------

//                 wfd s_meditemp();
                 vintTempPura=temp_val.f;
//                 if (vintTempPura < vintTempAnt+4 && vintTempPura > vintTempAnt-4){
                 if (vintTempPura < vintTempAnt+25 && vintTempPura > vintTempAnt-25){
                     wContSense++;
                 }else{
                     wContSense=0;
                     wMalTemp++;
                     if (wMalTemp > 9000) wMalTemp=0;

                 }
             }
             wContSense=0;
        }

        vintTempAnt=vintTempPura;
        vintLectTemp=vintTempPura+wDeltaTemp;
        vintLectHum=humi_val.f;

        intLectTemp=vintLectTemp;
        intLectHum=vintLectHum;

        printf ("Est.= %s -Temp.= %5.1f Gr (%s) -Hum.= %5.1f%% Eq1: %s, Eq2: %s, Secuenc = %s, Linea = %s \n",wEstSec,vintLectTemp,wEstTemp,vintLectHum,wEstEq1,wEstEq2,wEstSec,wEstLinea);
        LecTempFin=1;
     } // del costate



/* --- hay que sacarlo
//-----------------------------------------------------------------------------
    costate cTestAlarmas2Eq always_on{
//-----------------------------------------------------------------------------

       if (bitEnaRutAcFail==1){// esq 1000
         switch  (bitAcFail) {
            case 101:
               bitEnaRutTmp=0;
               UltimoEqOn=EqOn;  //teóricamente 12
               if (EqOn > 0) {
      	           waitfordone ApagarEquipos();
//                    EqOn=0;
               }

               ActEstadosCtrl("Espera","Servicio","X");
               EstVariables.CodEmail=1011; // para mostrarlo en fparams
               		                      // aunque estén deshabilitados los mails
               if (config.inthabimens==0){
                  CrearReporte(1,1011,0,1,1,0,2);
                  CoBegin(&cRepiteRepoTemp);
               }
               bitAcFail=102;
               break;
           case 102: // la luz permanece cortada
              if (bitTmpAlta==501){ // entro el alta temperatura
                  EstVariables.CodEmail=1110; // para mostrarlo en fparams
               		                        // aunque estén deshabilitados los mails

                  if (config.inthabimens==0){
                       CrearReporte(1,1110,1,1,0,0,4);
                  }
                  bitTmpAlta=502;
             }
             break;

          case 109:
             switch (bitTmpAlta) {
                 case 500: // temperatura normal
                   CoBegin(&cDosEqON);
                   ActEstadosCtrl("Fijo","Servicio","X");
                   EstVariables.CodEmail=1094; // para mostrarlo en fparams
               		                          // aunque estén deshabilitados los mails

                   waitfor(isCoRunning(&cDosEqON)==0);
                   CrearReporte(1,1094,0,9,4,0,0);

                   if (config.inthabimens==0){
                       CoReset(&cRepiteRepoTemp);
                       waitfor(isCoRunning(&cSendMail)==0);
                       CoBegin(&cMensStatus);

                   }
                   bitEnaRutTmp=1;
                   bitAcFail=100;
                   break;
                 case 501://
                   break;
                 case 502://
                    if (EqOn==0){ // los equipos están apagados
                        CoBegin(&cDosEqON);  // enciendo los dos equipos
                        ActEstadosCtrl("Fijo","Servicio","Delay");
                        CoBegin(&cEsperaTemp);
                        bitTmpAlta=503;
                        EstVariables.CodEmail=1294; // para mostrarlo en fparams
               		                      // aunque estén deshabilitados los mails

//                        if (config.inthabimens==0){
                            CrearReporte(1,1294,2,9,4,0,2);
//                        }
                    }
                    break;
                 case 503://
                    if (bitEsperaTemp==1){
                     // espero;;
                    }else{
                       // expiró tiempo de espera y la temperatura no se reestableció.
                       ActEstadosCtrl("Fijo","Servicio","Delay");
                       EstVariables.CodEmail=1205; // para mostrarlo en fparams
                        		                      // aunque estén deshabilitados los mails

//                       if (config.inthabimens==0){
                           CrearReporte(1,1205,2,0,5,0,4);
//                       }
                       bitTmpAlta=504;
                    }
                    break;
                 case 509:// Temperatura volvió a OK
		              BitWrPortI(PFDR,&PFDRShadow,Off,foHighTemp); // enciende led del frente
                    CoReset(&cRepiteRepoTemp);
                    ActEstadosCtrl("Fijo","Servicio","X");
                    if (isCoRunning(&cEsperaTemp)==1) {
                           CoReset(&cEsperaTemp);
                           bitEsperaTemp=0;
                           EstVariables.CodEmail=1909; // para mostrarlo en fparams
                 		                      // aunque estén deshabilitados los mails

//                           if (config.inthabimens==0){
                                CrearReporte(1,1909,9,0,9,0,9);
//                            }
                    }else{
//                           if (config.inthabimens==0){
                                CrearReporte(1,1909,9,0,9,0,6);
//                            }

                    }
                    bitEnaRutTmp=1;
                    bitTmpAlta=500;
                    bitCompen=600;
                    bitAcFail=100;
                    CoReset(&cBuzzer);
                    BitWrPortI(PEDR,&PEDRShadow,0,eoBuzzer); // apaga el buzzer
                    break;
             }
       }
     }
     if (bitEnaRutTmp==1) {  // esq 2000
       switch (bitTmpAlta){
            case 501:
               waitfor (isCoRunning(&cDosEqON)==0);
               bitEnaRutAcFail=0;
               EstVariables.CodEmail=2109; // para mostrarlo en fparams
               		                      // aunque estén deshabilitados los mails

               CrearReporte(1,2109,1,0,9,0,4);
               if (config.inthabimens==0){
                     CoBegin(&cRepiteRepoTemp);
               }
               bitTmpAlta=502;
               break;

            case 502:
               switch (bitAcFail){
                    case 101:
                       UltimoEqOn=EqOn;
                       waitfordone ApagarEquipos();
                       EqOn=0;
                       EstVariables.CodEmail=2211; // para mostrarlo en fparams
               		                      // aunque estén deshabilitados los mails

  //                     if (config.inthabimens==0){
                            CrearReporte(1,2211,2,1,1,0,1);
  //
  //                     }
                       bitAcFail=102;
                       break;
                    case 102:

                       break;

                    case 109: //retorna la linea
                        EqOn=12;
                        CoBegin(&cDosEqON);
                        waitfor (isCoRunning(&cDosEqON)==0);
                        EstVariables.CodEmail=2294; // para mostrarlo en fparams
               		                      // aunque estén deshabilitados los mails

//                        if (config.inthabimens==0){
                            CrearReporte(1,2294,2,9,4,0,2);
//                        }
                        ActEstadosCtrl("Fijo","Servicio","X");
                        CoBegin(&cEsperaTemp);
                        bitAcFail=100;
                        bitTmpAlta=503;
                        break;
               }
               break;

            case 503: // espera temp
                    if (bitEsperaTemp==1){
                     // espero;;
                    }else{
                         // costate para que envie mensajes de temperatura.
                        EstVariables.CodEmail=2205; // para mostrarlo en fparams
                    		                      // aunque estén deshabilitados los mails


//                       if (config.inthabimens==0){

                           CrearReporte(1,2205,2,0,5,0,4);
//                       }
                       bitTmpAlta=504;
                    }
                    break;

            case 509: // se reestablecio temperatura
               CoReset(&cEsperaTemp);
               bitEsperaTemp=0;
               CoReset(&cRepiteRepoTemp);
               ActEstadosCtrl("Fijo","Servicio","X");
               bitEnaRutAcFail=1;
               EstVariables.CodEmail=2903; // para mostrarlo en fparams
               		                      // aunque estén deshabilitados los mails

//               if (config.inthabimens==0){
                     CrearReporte(1,2903,9,0,3,0,6);
//               }
               bitTmpAlta=500;
               bitCompen=600;
               CoReset(&cBuzzer);
               BitWrPortI(PEDR,&PEDRShadow,0,eoBuzzer); // apaga el buzzer
               break;

       }
     }
   }
---- Fin cTestAlarmas2Eq... hay que sacarlo */

//-----------------------------------------------------------------------------
  costate cTestAlarmasFijoHT always_on{
//-----------------------------------------------------------------------------

       if (bitEnaRutAcFail==1) { // esq 3000
           switch (bitAcFail) { // Nros 1xx
                   case 101: // Se cortó la luz
                           bitEnaRutTmp=0;
                           CoReset(&cReloj);
                           CoReset(&cConmutAire);
                           UltimoEqOn=EqOn;

                           if(EqOn > 0){
               		         // Apagar Equipos
    	                         waitfordone ApagarEquipos("EnaRut");
                               ActEstadosCtrl("Espera","Servicio","X");
                               EqOn=0;

                                  // Costate que envie mensajes de la temperatura.
                            }
                            EstVariables.CodEmail=3011; // para mostrarlo en fparams
               		                      // aunque estén deshabilitados los mails
                            CrearReporte(1,3011,0,1,1,0,2);

                            if (config.inthabimens==0){
                                  CoBegin(&cRepiteRepoTemp);
                            }
                            bitAcFail=102;
                            break;
                    case 102: // la luz permanece cortada
                            if (bitTmpAlta==501){ // entro el alta temperatura
                                   EstVariables.CodEmail=3111; // para mostrarlo en fparams
                               		                      // aunque estén deshabilitados los mails



                                  CrearReporte(1,3111,1,1,1,0,4);
                            }
                            break;

                    case 109: // la luz volvió
                            switch (bitTmpAlta) { // Nros 5xx
                                     case 500:// Alta Temp nunca pasó umbral enciendo el equipo que corresponde
                                           if (config.vintTipoCr1!=2 && config.vintTipoCr2!=2){
	                                           VueltaSecuenc();
                                           }
                                           CoBegin(&cInitAire); // enciende el aire inmediatamente
                                           ActEstadosCtrl("Activo","Servicio","X");
                                           waitfor(isCoRunning(&cInitAire)==0);
                                           EstVariables.CodEmail=3092; // para mostrarlo en fparams
               		                                           // aunque estén deshabilitados los mails

                                           CrearReporte(1,3092,0,9,2,0,0);
                                           CoReset(&cRepiteRepoTemp);
                                           CoBegin(&cReloj);
                                           bitAcFail=100;
                                           bitEnaRutTmp=1;

                                           break;
                                     case 501://
                                           break;
                                     case 502://
                                           if (EqOn==0){ // los equipos están apagados pero OK
                                              ActEstadosCtrl("Activo","Detenido","Delay");
                                              CoBegin(&cDosEqON);  // enciendo los dos equipos
                                              CoBegin(&cEsperaTemp);
                                              bitTmpAlta=503;
                                              EstVariables.CodEmail=3294; // para mostrarlo en fparams
               		                                         // aunque estén deshabilitados los mails

                      								 waitfor(isCoRunning(&cDosEqON)==0);
                                              CrearReporte(1,3294,2,9,4,0,2);
//                                            bitEnaRutTmp=1;
                                          	 }
                                           break;
                                     case 503://
                                           if (isCoRunning(&cEsperaTemp)==1){
                                            // espero;;
                                           }else{
                                             EstVariables.CodEmail=3203; // para mostrarlo en fparams

                                             CrearReporte(1,3203,2,0,3,0,4);

                                             bitTmpAlta=504;
                                           }
                                           break;
                                     case 509:// Temperatura volvió a OK


             // hay que ver esta parte
             // la idea es que apague el equipo que originalmente estaba apagado antes del corte de luz
            // pero si se va a encender pronto que apague el que originalmente estaba encendido
                                          ActEstadosCtrl("Activo","Servicio","X");

                                          if (isCoRunning(&cEsperaTemp)==1) {
                                               CoReset(&cEsperaTemp);
                  		                       bitEsperaTemp=0;
                                               EstVariables.CodEmail=3902; // para mostrarlo en fparams
               		                      // aunque estén deshabilitados los mails

					                                CrearReporte(1,3902,9,0,2,0,0);

                                          } else {
                                               EstVariables.CodEmail=3905; // para mostrarlo en fparams
               		                      // aunque estén deshabilitados los mails

                    					              CrearReporte(1,3905,9,0,5,0,6);
                                         	}


                                           // apago equipo que no deba estar encendido
                                           if (config.vintTipoCr1!=2 && config.vintTipoCr2!=2){
	                                           VueltaSecuenc();
                                           }

                                          //posiblemente sea mejor ejecutar init aire primero y apagar después

                                           wfd ApagarEquipoOff(UltimoEqOn);
                                           bitEnaRutTmp=1;
                                           bitTmpAlta=500;
                                           bitCompen=600;
                                           CoBegin(&cReloj); //inicio secuenciado.
                                           // detener reportes de temperatura.
                                           if (config.inthabimens==0){
                                                 CoReset(&cRepiteRepoTemp);
                                           }
                                           CoReset(&cBuzzer);
                                           BitWrPortI(PEDR,&PEDRShadow,0,eoBuzzer); // apaga el buzzer

                                           CoPause(&cFallaLinea);
                                           bitAcFail=100;
                                           bitTmpAlta=500;
                                           bitCompen=600;
                                           CoResume(&cFallaLinea);
                                           break;
                             }


           }
       }
// llegamos hasta acá poniendo todas las rutinas

       if (bitEnaRutTmp==1) { // esq 4000
           if ((bitTmpAlta==501)&&(wFijo==800)&&(bitRecentConmut==0)){   //recentconmut indica si la conmutacion fue reciente
                bitEnaRutAcFail=0;
                wFijo=801;
                BitWrPortI(PBDR,&PBDRShadow,On,boAireFijo);
                CoPause(&cReloj);
                CoBegin(&cBuzzer);
                CoBegin(&cConmutAire);
                CoBegin(&cEsperaTemp);
                bitTmpAlta=503;
                EstVariables.CodEmail=4107; // para mostrarlo en fparams
               		                      // aunque estén deshabilitados los mails

                waitfor(isCoRunning(&cConmutAire)==0);
                CrearReporte(1,4107,1,0,7,0,7);
                if (config.inthabimens==0){
                     CoBegin(&cRepiteRepoTemp);
                // iniciar de mensajes de temperatura
                }

           }
           if (wFijo==801){
              switch (bitAcFail){
                      case 100:
                             switch (bitTmpAlta) {
                                 case 501:
                                      CoBegin(&cEsperaTemp);
                                      ActEstadosCtrl("Fijo","Detenido","Delay");
                                      EstVariables.CodEmail=4106; // para mostrarlo en fparams
               		                      // aunque estén deshabilitados los mails

 // hacer una costate para asegurarse que esté todo detenido

                                      waitfor(isCoRunning(&cSendMail)==0);
                                      CrearReporte(1,4106,1,0,6,0,4);
                                      if (config.inthabimens==0){
                                           CoReset(&cMensStatus);
                                           CoBegin(&cRepiteRepoTemp);
                                      }
                                      bitTmpAlta=502;
                                      break;
                                 case 503:
                                      ActEstadosCtrl("Fijo","Detenido","Delay");
                                      if(isCoRunning(&cEsperaTemp)==1){
                                       //  espero;;
                                      }else{
                                       // problemas serios de temperatura
                                          EstVariables.CodEmail=4205; // para mostrarlo en fparams
               		                      // aunque estén deshabilitados los mails

                                          CrearReporte(1,4205,2,0,5,0,4);
	                                       bitTmpAlta=504;
                                      }
                                      break;
                                 case 509: // volvio de alta temperatura.
                                       ActEstadosCtrl("Fijo","Detenido","X");
                                       if (isCoRunning(&cEsperaTemp)==1) {
                                            CoReset(&cEsperaTemp);
                		                       bitEsperaTemp=0;
                                            EstVariables.CodEmail=4906; // para mostrarlo en fparams
               		                      // aunque estén deshabilitados los mails

				                                CrearReporte(1,4906,9,0,6,0,7);
                                       } else {
                                            EstVariables.CodEmail=4903; // para mostrarlo en fparams
               		                      // aunque estén deshabilitados los mails
                 					              CrearReporte(1,4903,9,0,3,0,7);
                                       }
                                       if (config.inthabimens==0){
                                           // se debe mantener el envio de status
                                             CoReset(&cRepiteRepoTemp);
                                             CoBegin(&cMensStatus);
                                       }

                                       bitTmpAlta=500;
                                       bitCompen=600;

                             		}
                         break;
                      case 101:// se cortó la luz teniendo un solo equipo
                      	UltimoEqOn=EqOn;
  	                     waitfordone ApagarEquipos("EnaRut");
                        EqOn=0;
                        waitfor (isCoRunning(&cSendMail)==0);
                        if ((bitTmpAlta >= 502)&&(bitTmpAlta<=504)){ // la temperatura está alta al producirse el corte
                        	EstVariables.CodEmail=4216; // para mostrarlo en fparams

                           if(config.inthabimens==0){
                           	CoReset(&cMensStatus);
                              waitfor(isCoRunning(&cSendMail)==0);
										CoBegin(&cRepiteRepoTemp);
                           }
                           CrearReporte(1,4216,2,1,6,0,4);
                           bitAcFail=103;
                        }else{
		                     EstVariables.CodEmail=4016; // la temperatura está normal
                                   										// al producirse el corte
                           if (config.inthabimens==0){
                                  // Costate que envie mensajes de la temperatura.
                                  // activar mensajes de temperatura
                           	CoReset(&cMensStatus);
                              waitfor(isCoRunning(&cSendMail)==0);
            	               CoBegin(&cRepiteRepoTemp);
                           }
              	            CrearReporte(1,4016,0,1,6,0,2);
                           bitAcFail=102;
                        }

                        break;

                      case 102://
                      	switch (bitTmpAlta) {
                        	case 501:
                           	EstVariables.CodEmail=4216; // para mostrarlo en fparams
               		                      // aunque estén deshabilitados los mails

                              if (config.inthabimens==0){
                              	CoReset(&cMensStatus);
                                 waitfor(isCoRunning(&cSendMail)==0);
                              }
                              CrearReporte(1,4216,1,1,6,0,4);

                                      // detener status y activar mensajes de temperatura.
                              bitTmpAlta=503;
                              break;
                         }
                         break;

                      case 109: // volvio la luz
                      	CoBegin(&cInitAire); // enciende el aire inmediatamente
                        waitfor (isCoRunning(&cInitAire)==0);

                        ActEstadosCtrl("Fijo","Detenido","X");
                        if(bitTmpAlta==500){
                        	EstVariables.CodEmail=4096;
                        }else{
                           CoBegin(&cEsperaTemp);
                           EstVariables.CodEmail=4196;  // para mostrarlo en fparams
                                                               // aunque estén deshabilitados los mails
                        }

                       	if(bitTmpAlta==500){
	                        if (config.inthabimens==0){
										CoReset(&cRepiteRepoTemp);
                              CoBegin(&cMensStatus);
                           }
                        	EstVariables.CodEmail=4096;
                           CrearReporte(1,4096,0,9,6,0,7);
                        }else{
                        	EstVariables.CodEmail=4196;
                           CrearReporte(1,4196,1,9,6,0,2);
                        }
                        bitAcFail=100;
                        break;

              }
           }
       }
  }



//------------------------------------------------------------------------------
   costate cFallaLinea always_on{
//------------------------------------------------------------------------------
//     if (bitAcFail==109){

//        bitAcFail=100;
//     }

     if (bitAcFail==100){
       if (BitRdPortI(PFDR,fiAcFail)==0){
         ActEstadosLinea("Mal");
         bitAcFail=101;
         BitWrPortI(PBDR,&PBDRShadow,On,boAcFail);
       }
     }else{
       if ((bitAcFail >100)&&(bitAcFail !=109)) { // hubo condicion de falla de linea
         if (BitRdPortI(PFDR,fiAcFail)==1){
//         if (BitRdPortI(PEDR,eiAcFail)==1){
            waitfor(DelaySec(10));
            if (BitRdPortI(PFDR,fiAcFail)==1){
//            if (BitRdPortI(PEDR,eiAcFail)==1){
                ActEstadosLinea("OK");
                BitWrPortI(PBDR,&PBDRShadow,Off,boAcFail);
                bitAcFail=109; // paso por falla de linea y volvio la luz
            }
          }
        }
     }
   }

//------------------------------------------------------------------------------
   costate cVerifyTempAlta always_on{
// 02/08/2008 Incluye bits de compensacion
//------------------------------------------------------------------------------

      if (bitCompen == 600){
           if (vintLectTemp > config.vintTempNormal){ // también umbral de reencendido
					#ifdef INFRARROJO
	               ReEncenderAA();
   				#endif
               bitCompen=601;
           }
      }else{
        if (bitCompen > 600){
           if ((bitTmpAlta == 500)||(bitTmpAlta == 508)){
              if (vintLectTemp > config.vintTempAlta){
                  ActEstadosTemp("Mal");
                  BitWrPortI(PFDR,&PFDRShadow,On,foHighTemp); // enciende led del frente
						 BitWrPortI(PCDR,&PCDRShadow,1,coAlExterna); // Activa Alarma Externa
                  if (config.bitSistON==1)
                         CoBegin(&cBuzzer);
                  // Alta temperatura

                  // deshabilitar por conmutación
                  bitTmpAlta=501; // cuando entra en alta temp
              }
           }
           if ((bitTmpAlta > 500) && (bitTmpAlta !=508) && (bitTmpAlta !=509)){
                if (vintLectTemp < config.vintTempDeltaComp){
                    // está mejorando la temperatura.
                    bitTmpAlta=508;
                }
           }

           if (vintLectTemp < config.vintTempNormal){
                if (bitRecentConmut==1){ // cuando conmutó la temp estaba mas alta que la normal
                    CoReset(&cRecentConmut);
              	     strcpy(wEstDelayMed,"X");
   					  bitRecentConmut=0;
                    bitTmpAlta=500;
                    CoReset(&cBuzzer);
                    BitWrPortI(PEDR,&PEDRShadow,0,eoBuzzer); // apaga el buzzer
		              ActEstadosTemp("OK");
		              BitWrPortI(PFDR,&PFDRShadow,Off,foHighTemp); // enciende led del frente
                    BitWrPortI(PCDR,&PCDRShadow,0,coAlExterna); // Desctiva Alarma Externa
      		    }else{
              		  if (bitTmpAlta == 508){
                    		ActEstadosTemp("OK");
   	                 	BitWrPortI(PFDR,&PFDRShadow,Off,foHighTemp); // enciende led del frente
                        BitWrPortI(PCDR,&PCDRShadow,0,coAlExterna); // Desctiva Alarma Externa
                       	bitTmpAlta=509;
                    }
                }
                bitCompen=600; // cuando sale de temp > normal
           }
        }
      }
   }



//------------------------------------------------------------------------------
   costate cVerifyTempCompen always_on{
//------------------------------------------------------------------------------

   }


//-----------------------------------------------------------------------------
   costate cInitAire {   //
//-----------------------------------------------------------------------------

   EqOn=UltimoEqOn;
   waitfor(LecTempFin==1);
   CoReset(&cLecTemp);
   if (EqOn == 1){  // hay que encender el aire 1
		  if (bitAcFail==109 && config.vintTipoCr1==2 && config.vintReenc1==1){
             // cr tipo TV y enciende solo
 			    BitWrPortI(PFDR,&PFDRShadow,On,foLedAA1);
        }else{
 	       	 wfd AA1ONExec();
   	  }
        if (wFijo==801){
            ActEstadosEq("On","EqMal");   // para el caso de corte de luz con aire en falla
        }else{
            ActEstadosEq("On","Off");
        }

    }
   if (EqOn == 2){  // hay que encender el aire 2
		  if (bitAcFail==109 && config.vintTipoCr2==2 && config.vintReenc2==1){
             // cr tipo TV y enciende solo
 			    BitWrPortI(PBDR,&PBDRShadow,On,boLedAA2);
        }else{
 	       	 wfd AA2ONExec();
   	  }
        if (wFijo==801){
           ActEstadosEq("EqMal","On");    // para el caso de corte de luz con aire en falla
        }else{
           ActEstadosEq("Off","On");
        }

    }
    wfd PitoBuzz(20000,1);
    CoBegin(&cSilResetStat);
    CoBegin(&cLecTemp);

  }


//-----------------------------------------------------------------------------
  costate cConmutAire {
//-----------------------------------------------------------------------------
// si la temp de la sala es mayor que la normal, entonces luego de conmutar
// debe esperar 30 minutos antes de volver a conmutar y dar la alarma
// algo así como disable 501

        CoReset(&cSilResetStat);
        CoReset(&cVerifyTempAlta);
        if (bitCompen==601){
              CoBegin(&cRecentConmut); // para darle al aire que se encendió tiempo de bajar la temp si estaba alta
		  }
        if (EqOn == 1){  // Está encendido el aire 1, hay que encender el aire 2
            wfd AA1OFFExec();
            waitfor(DelaySec(4));

            wfd AA2ONExec();
            waitfor(DelaySec(4));

            EqOn=2; // ahora está encendido el AA2
            if (wFijo==801){
               ActEstadosEq("EqMal","On");
            }else{
               ActEstadosEq("Off","On");
            }


        }else{
            if (EqOn == 2){ // Está encendido el aire 2, hay que encender el aire 1
               wfd AA2OFFExec();
               waitfor(DelaySec(4));

               wfd AA1ONExec();
               waitfor(DelaySec(4));

               EqOn=1; // ahora está encendido el AA1
               if (wFijo==801){
                  ActEstadosEq("On","EqMal");
               }else{
                  ActEstadosEq("On","Off");
               }


            }
        }
        CoBegin(&cSilResetStat);
        CoBegin(&cVerifyTempAlta);
  }

/*/--------------------------------------------------------------------------
   costate cInitCompen {   //contactor      3/4
//--------------------------------------------------------------------------

  // enciende equipo apagado

     waitfor(DelaySec(3));
     if (EqOn==1) { //compensa encendiendo el aire 2
         BitWrPortI(PBDR,&PBDRShadow,1,boContAA2);//
         BitWrPortI(PBDR,&PBDRShadow,On,boLedAA2);

     }

     if (EqOn==2) { //compensa encendiendo el aire 1
         BitWrPortI(PBDR,&PBDRShadow,1,boContAA1);//
         BitWrPortI(PFDR,&PFDRShadow,On,foLedAA1);
     }

     EqOn=12;

     // iniciar costate de espera

   }

*/
//---------------------------------------------------------------------
  costate cFinCompen {    //contactor      4/4
//---------------------------------------------------------------------

   if (config.inthabimens==0){
//       CoBegin(&cMensCompenOK);
   }
   CoBegin(&cInitAire); //
   if (vintHoraConmut < now()){
        vintHoraConmut=SetTimer(vintTRestante);
   }
   CoBegin(&cReloj);
//	ActStatus();
  }




//-----------------------------------------------------------------------------
  costate cDosEqON { // Los dos equipos en ON
//-----------------------------------------------------------------------------

 // Presionar boton AA 1



    if ((EqOn==0)||(EqOn==2)){
       wfd AA1ONExec();
       waitfor (DelaySec(5));
    }
 // Presionar boton AA 2
    if ((EqOn==0)||(EqOn==1)){
    	 wfd AA2ONExec();
       waitfor (DelaySec(5));
    }
    EqOn=12;  //equipos 1 y 2 encendidos.
    ActEstadosEq("On","On");


 }

//-----------------------------------------------------------------------------
  costate cbtReset { // efectua el reset
//-----------------------------------------------------------------------------
       CoReset(&cReloj);
       CoReset(&cFallaLinea);
       CoReset(&cBuzzer);
       BitWrPortI(PEDR,&PEDRShadow,0,eoBuzzer); // apaga el buzzer
       CoReset(&cTestAlarmas2Eq);
       CoReset(&cTestAlarmasFijoHT);
       if (EstVariables.cVerifyTempAlta==0)
           CoBegin(&cVerifyTempAlta);
//       CoReset(&cVerifyTempCompen);
       CoReset(&cFallaLinea);
       CoReset(&cEsperaTemp);
       CoReset(&cRecentConmut);
		 CoReset(&cEsperaComp);
       CoReset(&cMensStatus);
       CoReset(&cRepiteRepoTemp);
       // reiniciar variables de bit
       wfd ApagarEquipos("Reset");
       UltimoEqOn=0;
       BitWrPortI(PFDR,&PFDRShadow,Off,foHighTemp);
       BitWrPortI(PBDR,&PBDRShadow,Off,boAcFail);
       BitWrPortI(PBDR,&PBDRShadow,Off,boAireFijo);
       ResetVarEstados();
       // poner en cero variables de muestra de tiempo conmutacion
       vHoras=0UL;
       vMinutos=0UL;
       vSegundos=0UL;
       EstVariables.CodEmail=9882;  // para mostrarlo en fparams
                                                               // aunque estén deshabilitados los mails

       CrearReporte(2,9882,8,8,12,0,0);

  		 config.bitSistON=0;
       wfd PitoBuzz(20000,2);
       Save_Config();
       CoBegin(&cSilResetStat);

    }


//----------------------------------------------------------------------
   costate cReloj always_on {
//----------------------------------------------------------------------
     if (vintHoraConmut < now()) {
     		vintTRestante = 0;
     }else{
         vintTRestante=vintHoraConmut-now();
     }
     convHMS(vintTRestante); // para mostrar en la página web.
     if (vintTRestante < config.vintTMinACFail*config.kTMinACFail){
         bitMinConmut=1;
     }else{
         bitMinConmut=0;
     }
     if (vintTRestante < 1){
       CoBegin(&cConmutAire);
       // carga de nuevo los parametros de conmutación
       vintHoraConmut=SetTimer(vintTSConmut1);
       waitfor(isCoRunning(&cConmutAire)==0);
       EstVariables.CodEmail=9010;  // para mostrarlo en fparams
                                    // aunque estén deshabilitados los mails

       CrearReporte(1,9010,0,0,10,0,0);

     }


   }


//------------------------------------------------------------------------------
   costate cButtExec {
//------------------------------------------------------------------------------

// no debería presionar el botón.
// si el Control remoto está en ON debería pasar la variable del
// sitio a ON y luego volver a presionar el boton del CR
     waitfor(DelayMs(1000));
     BitWrPortI(PBDR,&PBDRShadow,1,boButt); //
     waitfor(DelayMs(1000));
     BitWrPortI(PBDR,&PBDRShadow,0,boButt); //

   }

//------------------------------------------------------------------------------
   costate cBuzzer always_on {
//------------------------------------------------------------------------------
     if (bitSilBuzzer==1){
        waitfor(DelaySec(10));
        BitWrPortI(PEDR,&PEDRShadow,1,eoBuzzer);
        waitfor(DelaySec(2));
        BitWrPortI(PEDR,&PEDRShadow,0,eoBuzzer);
     }
   }

//------------------------------------------------------------------------------
   costate cEsperaTemp {
//------------------------------------------------------------------------------
       bitEsperaTemp=1;
       waitfor(DelaySec(vintTSEsperaTemp));
       bitEsperaTemp=0;
   }

//------------------------------------------------------------------------------
   costate cRecentConmut {
//------------------------------------------------------------------------------
       bitRecentConmut=1;
 	    strcpy(wEstDelayMed,"Delay");
       waitfor(DelaySec(900));
	    strcpy(wEstDelayMed,"X");
       bitRecentConmut=0;
   }


//------------------------------------------------------------------------------
   costate cEsperaComp {
//------------------------------------------------------------------------------
       bitEsperaCompen=1;
       waitfor(DelaySec(vintTSEsperaTComp)); // ojo, se puso 1 minuto en cargadat tiemp
       bitEsperaCompen=0;
   }


//------------------------------------------------------------------------------
   costate cMensStatus always_on {
//------------------------------------------------------------------------------
      waitfor(DelaySec(intTimSMensStat));
      waitfor(isCoRunning(&cSendMail)==0);
      CrearReporte(2,wCodeEvento,widTemp,wid220,widSec,0,widGral);
  }


//------------------------------------------------------------------------------
   costate cRepiteRepoTemp always_on {
//------------------------------------------------------------------------------

      waitfor(DelaySec(intTimSMensTemp));
      waitfor(isCoRunning(&cSendMail)==0);
      strcpy(wTipoReporte,"Informe Temperatura");
//      DatosMensInforme();
      CoBegin(&cSendMail);
  }



//------------------------------------------------------------------------------
   costate cbtIniciar {
//------------------------------------------------------------------------------
   if  (config.bitSistON==0){ // solo si el secuenciador está detenido
     waitfor(LecTempFin==1);
     CoReset(&cLecTemp);
     ResetVarEstados(); //resetvariables
     CargaTiempos(); // carga los tiempos para las conmutaciones
     UltimoEqOn=config.intSelAAInit;
     CoBegin(&cInitAire); //
     ActEstadosCtrl("Activo","Servicio","X");
     CoBegin(&cReloj);
     CoBegin(&cFallaLinea);
     CoBegin(&cTestAlarmasFijoHT);

     EstVariables.CodEmail=9011;  // para mostrarlo en fparams
                                                               // aunque estén deshabilitados los mails

     CrearReporte(2,9011,0,0,11,0,0);

     config.bitSistON=1;
     Save_Config();
     CoBegin(&cLecTemp);

   }

 }

//------------------------------------------------------------------------------
   costate cbtDosEquiposON {
//------------------------------------------------------------------------------


     if  (config.bitSistON==0){ // solo si el secuenciador está detenido
       CoBegin(&cDosEqON);
       waitfor(isCoRunning(&cDosEqON)==0);
       ActEstadosCtrl("Fijo","Servicio","X"); // el ultimo depende del estado de demo
//       CoBegin(&cVerifyTempAlta);
       CoBegin(&cFallaLinea);
       CoBegin(&cTestAlarmas2Eq);
 	    BitWrPortI(PBDR,&PBDRShadow,On,boAireFijo);

//	     ActEstadosEq("On","On");
      }


      bitDosEqON=1;
      EstVariables.CodEmail=9004;  // para mostrarlo en fparams
                                                               // aunque estén deshabilitados los mails

      waitfor(isCoRunning(&cSendMail)==0);
      CrearReporte(1,9004,0,0,4,0,0);

      waitfor(isCoRunning(&cSendMail)==0);
      widSec=9;
      wCodeEvento=9000;
      CoBegin(&cMensStatus);
      CoBegin(&cSilResetStat);
      config.bitSistON=1;
      Save_Config();
}

//------------------------------------------------------------------------------
   costate cbtInitSecCompen {
//------------------------------------------------------------------------------

      if  (config.bitSistON==0){ // solo si el secuenciador está detenido
        ResetVarEstados(); //resetvariables
        CargaTiempos(); // carga los tiempos para las conmutaciones
        UltimoEqOn=config.intSelAAInit;
        CoBegin(&cInitAire); //
        ActEstadosCtrl("Activo","Servicio","X");
        CoBegin(&cReloj);

        if (isCoRunning(&cVerifyTempAlta)==0)
           CoBegin(&cVerifyTempAlta);
        CoBegin(&cFallaLinea);
        waitfor(isCoRunning(&cInitAire)==0);
        EstVariables.CodEmail=9011;  // para mostrarlo en fparams
                                     // aunque estén deshabilitados los mails

        CrearReporte(1,9011,0,0,11,0,0);

        config.bitSistON=1;
        Save_Config();
      }

 }


//------------------------------------------------------------------------------
  costate cSendMail { // sendmail
//------------------------------------------------------------------------------

          smtp_setserver(config.wsmtpsrvr);
// se pasa a crear reporte          strcpy(DirSep,config.wDestMensCrit);
          direnvio=strtok(DirSep, ";");

      	 #ifdef USE_SMTP_AUTH
	  			 	smtp_setauth (config.wsmtpcuentaval,config.wsmtppass);
          #endif

          NroMail++;
          if (NroMail > 9000) NroMail=1;

          sprintf(NroAsunto,"%s #%d - ",config.wAsunto,NroMail);

	       while (direnvio!=NULL) {
             lineamail=1;
   	       smtp_sendmail(direnvio,config.wsmtpmailfrom,NroAsunto,NULL);
	          smtp_data_handler(mail_generator, &line_num, 0);	// Set message generator function

             line_num=1;

             while(smtp_mailtick()==SMTP_PENDING){
                     strcpy(wMailStatus," Preparando envio de e-mail");
							waitfor(DelayMs(2));
             }
             if(smtp_status()==SMTP_SUCCESS){
                 printf("\n\r\Message sent\n\r");
                 strcpy(wMailStatus," Ultimo e-mail enviado OK");
             }else{
                 strcpy(wMailStatus," Ultimo e-mail con errores" );
                 if(smtp_status()==SMTP_UNEXPECTED)
                     printf("\n\r\Error INESPERADO \n\r");
                 else
                     printf("\n\r\Error sending message\n\r");
             }
            direnvio=strtok(NULL, ";");
    	    }
   }

//------------------------------------------------------------------------------
  costate cLowBattery always_on { // costate baja tension batería
//------------------------------------------------------------------------------
       // si falla de linea presente
       if ((bitAcFail!=100)&&(bitAcFail!=109)){
          if (BitRdPortI(PFDR,fiLowBattery)==0){
             // preparar mensaje baja batería
             if (config.inthabimens==0){
                 SendMensLowBatt();
             }
			#ifdef SNMP_ENABLED
  				if (config.inthabisnmp==0){
  					EnvioTraps(EstVariables.CodEmail);
  				}
			#endif

            waitfor (DelaySec(60));// esperar 1 minuto
             // apagar
            BitWrPortI(PFDR,&PFDRShadow,0,foCorteBaja);
          }
       }
  }
#ifdef INFRARROJO
//------------------------------------------------------------------------------
   costate cCaliDivCr {
//------------------------------------------------------------------------------

         CrDivTest=90;
         while ( CrDivTest< 250){
              waitfor (DelaySec(4));
 			     //WrPortI(TAT7R,&TAT7RShadow,CrDivTest);

              switch (AireACalibrar){
              	  case 1:
//					     BitWrPortI(PBDR,&PBDRShadow,1,boIrAA1);
//						  BitWrPortI(PBDR,&PBDRShadow,0,boIrAA2);
                 	  config.vintDivCr1=CrDivTest;
		              Emite_IR("cr1off",1);
                    break;
                 case 2:
//					     BitWrPortI(PBDR,&PBDRShadow,0,boIrAA1);
//						  BitWrPortI(PBDR,&PBDRShadow,1,boIrAA2);
                 	  config.vintDivCr2=CrDivTest;
		              Emite_IR("cr2off",2);
                    break;
              }
              printf("Emitido \n");
              CrDivTest ++;
        }
   }

#endif

  } // del for
} // del main






