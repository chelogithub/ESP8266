/*******************************************************************************
*	Declaraciones de la Librer�a
*
*	Funciones para utilizar el m�dulo WiFi ESP-8266 AT Commands
*
*	Autor: Marcelo Garcia
*
*******************************************************************************/
#include "main.h"
//------------------------- PARA FUNCION de automatizacion---------------------//
#define WF_CONFIG 0
#define WF_CONNECT 1
#define WF_CONNECT_TCP 2
#define WF_SEND 3

//------------------------- PARA FUNCION FT_String_ND -------------------------//
#define FIND 0
#define TOK  1
#define TOKDATA 2
#define TOKVECT 3
#define TOKIPDDATA 4
#define TOKIPDDATASRVR 5
//------------------------- PARA Comandos AT		 -------------------------//
#define at_ok 			   		1	//OK
#define at_cambiar_modo_ok 		28	//AT+CWMODE
#define at_deconectar_ok   		13	//AT+CWQAP
#define at_multiple_conn_ok 	7	//AT+CIPMUX=1
#define at_no_multiple_conn_ok 	8	//AT+CIPMUX=0
#define at_def_ip_ok	    	27	//AT+CIPSTA=
#define at_tcp_conectado		15	// CONNECT
#define at_tcp_ok_to_send		20	// \r\n>
#define at_tcp_enviado_ok		19	//SEND OK
#define at_tcp_srvr_ok 			21	//AT+CIPSERVER
#define at_tcp_srvr_ok_noch		23	//no change
#define at_tcp_closed_ok 		9   //AT+CIPCLOSE
#define at_read_ip_cfg_ok 		0
#define at_timeout_tcp_ok	    0
#define at_ping_ok 				0

#define at_fail 				3	//FAIL
#define at_conn_tcp_fail 		0
#define at_send_tcp_fail 		0

#define at_error				2	//ERROR
#define at_cambiar_modo_err		29	//AT+CWMODE
#define at_conn_wifi_err		32	//AT+CWJAP
#define at_multiple_conn_err	30	//AT+CIPMUX=1
#define at_no_multiple_conn_err	31	//AT+CIPMUX=0
#define at_def_ip_err			33	//AT+CIPSTA=
#define at_tcp_noip_err			34	//no ip
#define at_tcp_alrdy_cnntd_err  14 //ALREADY CONNECTED
#define at_tcp_snd_err			17 //link is not valid
#define at_tcp_conn_err			6  //ERROR CLOSED
#define at_tcp_srvr_err 		22 //AT+CIPSERVER
#define at_tcp_close_err		26 //AT+CIPCLOSE
#define at_timeot_err			0
#define at_set_out_err			0
#define at_ping_tout_err		0

#define at_wifi_disconnect		10	// WIFI DISCONNECT
#define at_wifi_connected		11	// WIFI CONNECTED
#define at_wifi_gotip			12	// WIFI GOT IP
#define at_tcp_conectado		15	// CONNECT
#define at_tcp_desconectado		16	// CLOSED
#define at_tcp_client_conectado	24	//,CONNECT
#define at_tcp_client_desc		25	//,CLOSED

#define at_tcp_enviado_error	18	//SEND FAIL
#define at_info_conn			0

#define at_restart				4	//ready


//------------------------- PARA Maquina de estados -------------------------//
#define CAMBIAR_MODO_EN_CURSO 	100
#define CAMBIAR_MODO_OK 		109
#define CAMBIAR_MODO_ERROR 		105

#define CONEXION_EN_CURSO 		200
#define CONEXION_OK 			209
#define CONEXION_ERROR 			205

#define DESCONEXION_EN_CURSO 	300
#define DESCONEXION_OK 			309
#define DESCONEXION_ERROR 		305

#define MUX_CONN_EN_CURSO 		400
#define MUX_CONN_OK 			409
#define MUX_CONN_ERROR 			405

#define DEF_IP_EN_CURSO 		500
#define DEF_IP_OK 				509
#define DEF_IP_ERROR 			505

#define TCP_CONN_EN_CURSO 		600
#define TCP_CONN_OK 			609
#define TCP_CONN_ERROR 			605
#define TCP_CONN_ERROR_WIFI		607

#define TCP_SND_EN_CURSO 		700
#define TCP_SND_OK 				709
#define TCP_SND_ERROR 			705
#define TCP_SND_ERROR_TCP		707

#define TCP_SRVR_EN_CURSO 		800
#define TCP_SRVR_OK 			809
#define TCP_SRVR_ERROR 			805
#define TCP_SRVR_CLIENT_DSCNN	806
#define TCP_SRVR_CLIENT_RX		807
#define TCP_SRVR_CLIENT_CNN		808

#define TCP_SRVR_TX_EN_CURSO 	900
#define TCP_SRVR_TX_OK 			909
#define TCP_SRVR_TX_ERROR 		905

#define at_busy_p				1000

struct MBUS_FIFO
{
	uint8_t IDRX;	//
	int  	Conexion;
	char 	MODBUS_DATA[512];//char 	MODBUS_DATA[260];
	uint8_t _n_MODBUS_DATA;
	int  	timeout;
};

struct WIFI
{
	GPIO_TypeDef  *RESET_PORT;		//Port for NSS
	uint16_t RESET_PIN;				//Pin number
	char _WF_Net[32];					//Nombre de la red WIFI  a conectaraurt
	char _WF_Pass[16];					//Password de la red WIFI
	char _TCP_Remote_Server_IP[16];			//IP del Servidor TCP
	char _TCP_Remote_Server_Port[6];				//Puerto del Servidor TCP
	char _TCP_Local_Server_IP[16];			//IP del Servidor TCP Local
	char _TCP_Local_Server_Port[6];			//Puerto del Servidor TCP local
	char _TCP_Local_Server_GWY[16];			//Gateway Servidor TCP Local
	char _TCP_Local_Server_MSK[16];			//Mascara Servidor TCP Local
	uint8_t  _TCP_Local_Server_EN;				//Habilitar modo Servidor TCP 0=DESACTIVADO 1=ACTIVADO
	uint8_t  _TCP_Local_Server_Initiated;		//Servidor TCP ya iniciado TCP 0=DESACTIVADO 1=ACTIVADO
	uint8_t  _automatizacion;					//Tareas gestionadas automaticamente
	int	 _estado;					//Estado de m�dulo WIFI
	int  _estado_conexion;			//Estado de la conexion del m�dulo
	int  _estado_rcv_data;			//Estado de Recepcion de datos
	int  _estado_old;
	char _dataRCV[512];	//char _dataRCV[384];				//Data recibida por TCP   SOLO PARA EVITAR SOBREESCRITURA VALOR ORIGINAL 64 RESTRINGIR EN EL .C
	char _data2SND[512];//char _data2SND[384];				//Data a enviar por TCP
	char _uart2snd[512];//char _uart2snd[384];			//Datos a enviar por UART
	char _uartRCVD[512];//char _uartRCVD[384];			//Datos recibidos de UART
	char _uartRCVD_tok[512];//char _uartRCVD_tok[384];		//Datos recibidos de UART
	int  _id_conn;					//ID de conexion del cliente con el servidor
	int  _send_data;					//Aviso que voy a enviar datos
	int	 _uartCHrcvd;				//Cantidad de caracteres recibidos por la uart
	int  _TCPchRCVD;				//Caracteres recibidos por TCP
	int	 _enviaruart;				//Envia _uart2snd por puerto serie
	int  _pasos;					//Pasos por la maquina de estados no se puede manejar como variable local porque hay que almacenarlos cuando entra y sale de la llamada
	int	 _instruccion;				//Funcion que se desea realizar
	int  _ejecucion;				//Funcion en ejecucion	
	char _okenvio[512];//char _okenvio[384];				//String para enviao de datos, comparacion de enviado y recibido strings variables
	int  _ticks;
	int  _ticks2;
	int  _wtchdog;					//Monitoreo de actividad del módulo
	int  _rst_rq;					//pedido de reset
	int	 _debug;					//utilizada par fines de debugging
	int	 _debug_str;				//utilizada par fines de debugging
	int  _debug_pos;				//utilizada par fines de debugging
	int  _debug_chrs;				//utilizada par fines de debugging
	int  _debug_rx_ok;
	int  _new_data_rcv;				//Flag de nuevos datos recibidos
	int  _debug_rx_no_ok;
	int  _n_orig;					//Cantidad de caracteres del vector original
	int  _n_cmp;					//Cantidad de caracteres del vector de comparacion
	int  _n_tok;					//Cantidad de caracteres del vector extraido
	int  _n_D2SND;					//Cantidad de caracteres del vector de datos
	int  _n_uart2SND;				//Cantidad de caracteres del vector a enviar por uart
	int  _n_fcomp;					//Cantidad de caracteres del vector de comparacion2
	int  _estado_data;				//Variable para analizar el estado de los datos recibidos
	int  _n_dataRCV;				//Cantidad de caracteres del vector dataRCV
	int  _debug_count;				//Cuento la catidad de ipds	
	int  _debug_count_old;			//Cuento la catidad de ipds
	int  _debug_count2;				//SEND OK para enviar datos
	int  _debug_count3;				//SEND OK recuperado de un IPD	
	int  _debug_count4;				//SEDN OK recibido directo
	int  _debug_count5;				//Cuento la catidad de ipds	
	int  _debug_count6;				//Cuento la catidad de ipds	
	int  _debug_count7;				//Cuento la catidad de ipds	
	int  _debug_count8;				//Cuento la catidad de ipds
	int  _debug_count9;				//Cuento la catidad de ipds
	int  _debug_count10;			//Cuento la catidad de ipds
	int  _debug_SEND_OK;
	int  _debug_CONNECT;
	int  _debug_CLOSED;
	int  _debug_WF_DISCONNECT;
	int  _debug_FAIL;
	int  _debug_ready;
	int  _debug_WF_CNNCTD;
	int  _debug_WF_GOTIP;
	int	 _subpaso;
	int  _overflowVector;			//Maximo tamaño del vector conproceso de +IPD
	int  _NO_IP;					//No define IP cuando está en 1
	int	 _DBG_EN;/*			1 - Conectar WIFI
														2 - Desconectar WIFI
														3 - Conectar TCP
														4 - Desconectar TCP
														5 - Enviar DATOS x TCP
														6 - 
*/													
};
void HW_RESET(struct WIFI *);
void NOP(struct WIFI *);
void Cambiar_CWMODE(struct WIFI *);
void MUX(struct WIFI *);
void IPDef(struct WIFI *);
void ConectarWIFI(struct WIFI *);
void DesconectarWIFI(struct WIFI *);
void ConectarTCP(struct WIFI *);
void CrearServidorTCP(struct WIFI *);
void EnviarDatosSRVR(struct WIFI *);
void DesconectarTCP(struct WIFI *);
void EnviarDatos(struct WIFI *);
void Inicializar(struct WIFI *);
int AT_ES8266(struct WIFI *);
/*----------------------------------------------
int  AT_Recibir_String_struct(struct WIFI *);

int:		El retorno entreo de la funci�n es el estado de lo que recibe
				0:			COMANDO NO RECONOCIDO
				1:			CERRADA CONEXION TCP NO ACTIVA
				2:			WIFI DESCONECTADA
				3:			WIFI CONECTADA
				4:			OBTENCION DE IP
				5:			CONEXION TCP CERRADA
				6:			SERVIDOR TCP CAIDO NO SE PUEDE CONECTAR
				7:			CONECTADO LUEGO DE INTENTO DE CONEXI�N
				8:			OK Luego de conexion a internet
				9:			CERRAR TCP DESDE EL MODULO OK
				10:			LISTO PARA ENVIAR DATA
				11:			DATA ENVIADA CORRECTAMENTE (OJO SOLO VERIFICA EL OK NO LO QUE ENVI�)
				12:			ERROR AL CERRAR CONEXION TCP 
				13:			ERROR AL CONECTAR TCP POR LAN CAIDA
				14:			ERROR CONECTAR TCP YA CONECTADO
				15:			ERROR AL ENVIAR DATOS
				20:			CONECTANDO
				21:			DESCONECTANDO
				22:			CONECTANDO TCP
				23:			DESCONECTANDO TCP
				24:			ENVIANDO DATOS TCP

				..
				54:			Listo para el env�o de datos
				55:			TEXTO RECIBIDO CON ERRORES
				99:			TEXTO RECIBIDO OK


-----------------------------------------------*/
int AT_ES8266_ND(struct WIFI *);
/*----------------------------------------------
int  AT_Recibir_String_struct(struct WIFI *);

int:		El retorno entreo de la funci�n es el estado de lo que recibe
				0:			COMANDO NO RECONOCIDO
				1:			CERRADA CONEXION TCP NO ACTIVA
				2:			WIFI DESCONECTADA
				3:			WIFI CONECTADA
				4:			OBTENCION DE IP
				5:			CONEXION TCP CERRADA
				6:			SERVIDOR TCP CAIDO NO SE PUEDE CONECTAR
				7:			CONECTADO LUEGO DE INTENTO DE CONEXI�N
				8:			OK Luego de conexion a internet
				9:			CERRAR TCP DESDE EL MODULO OK
				10:			LISTO PARA ENVIAR DATA
				11:			DATA ENVIADA CORRECTAMENTE (OJO SOLO VERIFICA EL OK NO LO QUE ENVI�)
				12:			ERROR AL CERRAR CONEXION TCP
				13:			ERROR AL CONECTAR TCP POR LAN CAIDA
				14:			ERROR CONECTAR TCP YA CONECTADO
				15:			ERROR AL ENVIAR DATOS
				16:			ESTABLECIDA MULTIPLES CONEXIONES
				19:			DEFINIENDO CONEXIONES MULTIPLES
				20:			CONECTANDO
				21:			DESCONECTANDO
				22:			CONECTANDO TCP
				23:			DESCONECTANDO TCP
				24:			ENVIANDO DATOS TCP

				..
				54:			Listo para el env�o de datos
				55:			TEXTO RECIBIDO CON ERRORES
				99:			TEXTO RECIBIDO OK


-----------------------------------------------*/
void AT_Enviar_String(char *,  char *,  char *);
/*----------------------------------------------
void AT_Enviar_String(int, char *1, int *, int *);

int:		Define el tipo de dato que se va a enviar
				1:			Envio de datos estandard
				2:			
				3:			WIFI CONECTADA
				4:			OBTENCION DE IP
				5:			CONEXION TCP CERRADA
				6:			SERVIDOR TCP CAIDO NO SE PUEDE CONECTAR
				7:			CONECTADO LUEGO DE INTENTO DE CONEXI�N
				..
				55:			TEXTO RECIBIDO CON ERRORES
				99:			TEXTO RECIBIDO OK

char *1:	Cadena de datos a enviar
char *2:	Cadena de datos para  enviar al ESP8266
char *3:	Cadena de datos que se espera recibir luego de la instrucci�n
int  *4:	
-----------------------------------------------*/
int WiFi_Conn( struct WIFI *,UART_HandleTypeDef *,UART_HandleTypeDef *, int);
int WiFi_Conn_ND( struct WIFI *,UART_HandleTypeDef *, int);
/*----------------------------------------------
int WiFi_Conn_ND( struct WIFI *,UART_HandleTypeDef *,UART_HandleTypeDef *,int);

int:		Devuelve el estado de la conexion
				1:			Desconectado
				2:			WiFi Conectado
				3:			Enviar_datos		//Lo deb setear para enviar datos.
				4:			Conexi�n TCP Exitosa
				5:			Envio de datos erroneo

				99:		  Desconectado al iniciar el equipo

struct WIFI *			:Estructura de datos para la funcion
UART_HandleTypeDef *	:Puerto utilizado para el modulo
int						: 0 no debug 1 debug*/
