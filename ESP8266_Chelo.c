/*******************************************************************************
*	Codigo de las funciones
*
*	Funciones para utilizar el m�dulo WiFi ESP-8266 AT Commands
*
*	Autor: Marcelo Garcia
*
*******************************************************************************/

#include "string.h"
#include "stdlib.h"
#include "stdio.h"
#include "ESP8266_Chelo.h"
#include "STR_Chelo.h"
#include "ModBUS_Chelo.h"
#include "main.h"




//No se puede quedar esperano en las funciones, hay que detectar a situaci�n y salir del bucle.
//pensar que solo entra cuando recibe datos, si no vuelve a entrar queda ah� eternamentes.
void HW_RESET(struct WIFI *a)
{
    HAL_GPIO_WritePin(a->RESET_PORT, a->RESET_PIN, GPIO_PIN_RESET);
    ITM0_Write("\r\n ESP HW Resetting\r\n",strlen("\r\n ESP HW Resetting\r\n"));
    HAL_Delay(2000);											//Tiempo de reset del módulo
    ITM0_Write("\r\n ESP Reset\r\n",strlen("\r\n ESP Reset\r\n"));
    HAL_GPIO_WritePin(a->RESET_PORT, a->RESET_PIN, GPIO_PIN_SET);		//Habilito módulo
}
void NOP(struct WIFI *a)
{
	a->_instruccion=0;
	a->_enviaruart=0;
	a->_pasos=0;
}
void Cambiar_CWMODE(struct WIFI *a)
{
	a->_instruccion=1;
	a->_enviaruart=1;
	a->_pasos=0; //210416
}
void ServerTCP(struct WIFI *a)
{
	a->_instruccion=-1;
	a->_enviaruart=1;
	a->_pasos=0; //210416
}
void ConectarWIFI(struct WIFI *a)
{
	a->_instruccion=2;
	a->_enviaruart=1;
	a->_pasos=0; //210416
}
void MUX(struct WIFI *a)
{
	a->_instruccion=4;
	a->_enviaruart=1;
	a->_pasos=0; //210416
}
void IPDef(struct WIFI *a)
{
	a->_instruccion=5;
	a->_enviaruart=1;
	a->_pasos=0; //210416
}
void DesconectarWIFI(struct WIFI *a)
	{
	a->_instruccion=0;
	a->_enviaruart=1;
	a->_pasos=0; //210416
	}
void ConectarTCP(struct WIFI *a)
	{
	a->_instruccion=6;
	a->_enviaruart=1;
	a->_pasos=0; //210416
	}
void CrearServidorTCP(struct WIFI *a)
	{
	a->_instruccion=8;
	a->_enviaruart=1;
	a->_pasos=0; //210416
	}
void EnviarDatosSRVR(struct WIFI *a)
	{
	a->_instruccion=9;
	a->_enviaruart=1;
	a->_pasos=0; //210416
	}
void DesconectarTCP(struct WIFI *a)
	{
	//a->_instruccion=4;
	a->_enviaruart=1;
	a->_pasos=0; //210416
	}
void EnviarDatos(struct WIFI *a)
	{
	a->_instruccion=7;
	a->_enviaruart=1;
	a->_pasos=0; //210416
	}
void Inicializar(struct WIFI *a)
{
a->_WF_Net[32]='\0';				//Nombre de la red WIFI  a conectaraurt
a->_WF_Pass[16]='\0';				//Password de la red WIFI
a->_TCP_Remote_Server_IP[16]='\0';			//IP del Servidor TCP al que me voy a conectar
a->_TCP_Remote_Server_Port[6]='\0';			//Puerto del Servidor TCP al que me voy a conectar
a->_TCP_Local_Server_IP[16]='\0';		//IP del Servidor TCP Local
a->_TCP_Local_Server_Port[6]='\0';		//Puerto del Servidor TCP local
a->_TCP_Local_Server_GWY[16]='\0';		//Gateway de red
a->_TCP_Local_Server_MSK[16]='\0';		//Mascara de red
a->_TCP_Local_Server_Initiated=0;		//Servidor TCP no iniciado
a->_estado=0;					//Estado de m�dulo WIFI
a->_estado_rcv_data=0;			//Estado de Recepcion de datos
a->_dataRCV[512]='\0';			//Data recibida por TCP   SOLO PARA EVITAR SOBREESCRITURA VALOR ORIGINAL 64 RESTRINGIR EN EL .C
a->_data2SND[64]='\0';			//Data a enviar por TCP
a->_uart2snd[512]='\0';			//Datos a enviar por UART
a->_uartRCVD[512]='\0';			//Datos recibidos de UART
a->_uartRCVD_tok[512]='\0';		//Datos recibidos de UART
a->_uartCHrcvd=0;				//Cantidad de caracteres recibidos por la uart
a->_TCPchRCVD=0;				//Caracteres recibidos por TCP
a->_enviaruart=0;				//Envia _uart2snd por puerto serie
a->_pasos=0;					//Pasos por la maquina de estados no se puede manejar como variable local porque hay que almacenarlos cuando entra y sale de la llamada
a->_instruccion=0;				//Funcion que se desea realizar
a->_ejecucion=0;				//Funcion en ejecucion	
a->_okenvio[64]='\0';			//String para enviao de datos, comparacion de enviado y recibido strings variables
a->_ticks=0;
a->_ticks2=0;
a->_debug=0;					//utilizada par fines de debugging
a->_debug_str=0;				//utilizada par fines de debugging
a->_debug_pos=0;				//utilizada par fines de debugging
a->_debug_chrs=0;				//utilizada par fines de debugging
a->_debug_rx_ok=0;
a->_debug_rx_no_ok=0;
a->_debug_SEND_OK=0;
a->_debug_CLOSED=0;
a->_debug_WF_DISCONNECT=0;
a->_debug_FAIL=0;
a->_debug_CONNECT=0;
a->_debug_count=0;				//Cuento la catidad de ipds	
a->_debug_count_old=0;			//Cuento la catidad de ipds
a->_debug_count2=0;				//SEND OK para enviar datos
a->_debug_count3=0;				//SEND OK recuperado de un IPD	
a->_debug_count4=0;				//SEDN OK recibido directo
a->_debug_count5=0;				//Cuento la catidad de ipds	
a->_debug_count6=0;				//Cuento la catidad de ipds	
a->_debug_count7=0;				//Cuento la catidad de ipds
a->_debug_count8=0;				//Cuento la catidad de ipds
a->_debug_count9=0;				//Cuento la catidad de ipds
a->_debug_count10=0;			//Cuento la catidad de ipds
a->_subpaso=0;
a->_overflowVector=512;			//Maximo tamaño del vector al procesar un +IPD
}

 // estado no cambia a->estado salvo que dicho estado sea algun valor esperado en la funcion
 //	la recepcion de datos no afecta la variable a->estado, pero puede afectar el registro recibido, en tal caso la funci�n de cae por timeout de rpta a->ticks=5eg
 /*--------------------------------------------------------------*/
int AT_ESP8266_ND( struct WIFI *a )
 {
	 int l3=0, AT_decode=0, estado=0, str=0,chr_pos_fnc=0;
	 char *token;
	 char memstr=' ';
		char comillas='"';
		char separador[]={'"',',','"','\0'};
		char finalizar[]={'"','\r','\n'};
		char separador2[]={'"',','};
		char finalizar2[]={'\r','\n'};
		char vectorcopia[512];
	 //Agregar condicion de falla al conectar strcmp(a->_uartRCVD,"+CWJAP 1\r\n\r\nFAIL\r\n")
	 //a->_estado=0;
//--------BUSCO ERRRORES DE SOLPAMIENTO------------//
int pos=0, pos2=0;
a->_debug_pos=0;
a->_debug_str=strlen(a->_uartRCVD);//str=strlen(a->_uartRCVD)-1;
a->_estado_data=0; //Al entrar, nunca se como se recibió la info

	 a->_uartCHrcvd=strlen(a->_uartRCVD);
//***************************************************************//
//------------------PREGUNTO RECEPCION DE DATOS------------------//
//***************************************************************//

	 // Si tiene datos recibidos los proceso y dejo el vector original

	 a->_n_fcomp=strlen("\r\n+IPD");
	 if (FT_String_ND(a->_uartRCVD,&a->_n_orig,"\r\n+IPD",&a->_n_fcomp,a->_uartRCVD_tok,&a->_n_tok,&chr_pos_fnc,&a->_id_conn,a->_overflowVector,FIND)==1)
	 {

	 	a->_n_fcomp=strlen("\r\n+IPD");
	 	//a->_estado_data=0;

	 	if(a->_TCP_Local_Server_EN==0)
	 	{
	 		a->_estado_data=FT_String_ND(a->_uartRCVD,&a->_n_orig,"\r\n+IPD",&a->_n_fcomp,a->_uartRCVD_tok,&a->_n_tok,&chr_pos_fnc,&a->_id_conn,a->_overflowVector,TOKIPDDATA);
	 	}
	 	else
	 	{
	 		a->_estado_data=FT_String_ND(a->_uartRCVD,&a->_n_orig,"\r\n+IPD",&a->_n_fcomp,a->_uartRCVD_tok,&a->_n_tok,&chr_pos_fnc,&a->_id_conn,a->_overflowVector,TOKIPDDATASRVR);
	 	}

	 	if(a->_estado_data==4)
	 	{
	 		a->_estado_rcv_data=99;
	 		a->_n_dataRCV=a->_n_tok;
	 		CopiaVector(a->_dataRCV,a->_uartRCVD_tok,a->_n_tok,0,'A');  //Solo copio la info si es correcta
	 		CopiaVector(a->_data2SND,a->_uartRCVD_tok,5,0,'A');  //Solo copio la info si es correcta

	 		a->_debug_rx_ok++;
	 		a->_new_data_rcv=1;
	 	}
	 	if(a->_estado_data==5)
	 	{
	 		a->_estado_rcv_data=55;
	 		a->_n_dataRCV=0;
	 		a->_debug_rx_no_ok++;
	 		a->_new_data_rcv=1;
	 	}
	 }

	 //--------------------------------------------------------------------------------------------------------------//
	 // 	Arranco preguntando por la sentencia OK, y luego busco otros string dentro de lo recibido     			 //
	 //--------------------------------------------------------------------------------------------------------------//
	 a->_n_fcomp=strlen("OK\r\n");
	if (FT_String_ND(a->_uartRCVD,&a->_n_orig,"OK\r\n",&a->_n_fcomp,a->_uartRCVD_tok,&a->_n_tok,&chr_pos_fnc,&a->_id_conn,a->_overflowVector,FIND)==1)
	{

		AT_decode=at_ok;

		//------------------CONFIRMO ENVIO DE INFO OK ------------------//
		a->_n_fcomp=strlen("\r\nSEND");
		 if (FT_String_ND(a->_uartRCVD,&a->_n_orig,"\r\nSEND",&a->_n_fcomp,a->_uartRCVD_tok,&a->_n_tok,&chr_pos_fnc,&a->_id_conn,a->_overflowVector,FIND)==1)//Transmision ok
			 {
			 AT_decode=at_tcp_enviado_ok;
			 a->_debug_SEND_OK++;
			 }
		 else
		 {
		//------------------CONFIRMO CIERRE DE  CONEXION------------------//
		a->_n_fcomp=strlen("AT+CWQAP");
		if (FT_String_ND(a->_uartRCVD,&a->_n_orig,"AT+CWQAP",&a->_n_fcomp,a->_uartRCVD_tok,&a->_n_tok,&chr_pos_fnc,&a->_id_conn,a->_overflowVector,FIND)==1)
			{
			AT_decode=at_deconectar_ok;
			}
		else
		{
		//------------------CAMBIAR MODO DE FUNCIONAMIENTO------------------//
		a->_n_fcomp=strlen("AT+CWMODE=");
		if (FT_String_ND(a->_uartRCVD,&a->_n_orig,"AT+CWMODE=",&a->_n_fcomp,a->_uartRCVD_tok,&a->_n_tok,&chr_pos_fnc,&a->_id_conn,a->_overflowVector,FIND)==1)//Error desconectar TCP ya desconectado
			{
				AT_decode=at_cambiar_modo_ok;
			}
		else
		{
		//------------------INTENTO DE CREACION SEVER YA CREADO ------------------//
		a->_n_fcomp=strlen("no change\r\nOK\r\n");
			if (FT_String_ND(a->_uartRCVD,&a->_n_orig,"no change\r\nOK\r\n",&a->_n_fcomp,a->_uartRCVD_tok,&a->_n_tok,&chr_pos_fnc,&a->_id_conn,a->_overflowVector,FIND)==1)//if (strcmp(a->_uartRCVD,"CLOSED\r\n")==0)//Cerrado TCP desde la APP
			{
				AT_decode=at_tcp_srvr_ok_noch;
			}
		else
		{
		//------------------OK PARA ENVIAR------------------//
		a->_n_fcomp=strlen("\r\n>");
		if (FT_String_ND(a->_uartRCVD,&a->_n_orig,"\r\n>",&a->_n_fcomp,a->_uartRCVD_tok,&a->_n_tok,&chr_pos_fnc,&a->_id_conn,a->_overflowVector,FIND)==1)//Conectado desde el modulo
			{
				AT_decode=at_tcp_ok_to_send;
			}
		else
		{
		//------------------OK SERVIDOR------------------//
		a->_n_fcomp=strlen("AT+CIPSERVER");
		if (FT_String_ND(a->_uartRCVD,&a->_n_orig,"AT+CIPSERVER",&a->_n_fcomp,a->_uartRCVD_tok,&a->_n_tok,&chr_pos_fnc,&a->_id_conn,a->_overflowVector,FIND)==1)
		{
			AT_decode=at_tcp_srvr_ok;
		}
		else
		{
		//------------------DESCONECTAR TCP CUANDOYA EST� DESCONECTADO------------------//
		a->_n_fcomp=strlen("AT+CIPCLOSE");
		if (FT_String_ND(a->_uartRCVD,&a->_n_orig,"AT+CIPCLOSE",&a->_n_fcomp,a->_uartRCVD_tok,&a->_n_tok,&chr_pos_fnc,&a->_id_conn,a->_overflowVector,FIND)==1)
			{
				AT_decode=at_tcp_closed_ok;
			}
		else
		{
		//------------------DEFIIR MULTIPLES CONEXIONES OK------------------//
		a->_n_fcomp=strlen("AT+CIPMUX=1");
		if (FT_String_ND(a->_uartRCVD,&a->_n_orig,"AT+CIPMUX=1",&a->_n_fcomp,a->_uartRCVD_tok,&a->_n_tok,&chr_pos_fnc,&a->_id_conn,a->_overflowVector,FIND)==1)
			{
				AT_decode=at_multiple_conn_ok;
			}
		else
		{
		//------------------DEFIIR NO MULTIPLES CONEXIONES OK------------------//
		a->_n_fcomp=strlen("AT+CIPMUX=0");
		if (FT_String_ND(a->_uartRCVD,&a->_n_orig,"AT+CIPMUX=0",&a->_n_fcomp,a->_uartRCVD_tok,&a->_n_tok,&chr_pos_fnc,&a->_id_conn,a->_overflowVector,FIND)==1)
			{
				AT_decode=at_no_multiple_conn_ok;
			}
		else
		{
		//------------------DEFIIR IP OK------------------//
		a->_n_fcomp=strlen("AT+CIPSTA=");
		if (FT_String_ND(a->_uartRCVD,&a->_n_orig,"AT+CIPSTA=",&a->_n_fcomp,a->_uartRCVD_tok,&a->_n_tok,&chr_pos_fnc,&a->_id_conn,a->_overflowVector,FIND)==1)
			{
				AT_decode=at_def_ip_ok;
			}
		else
		{
		//------------------TCP CONECTADO------------------//
		a->_n_fcomp=strlen("CONNECT");
		if (FT_String_ND(a->_uartRCVD,&a->_n_orig,"CONNECT",&a->_n_fcomp,a->_uartRCVD_tok,&a->_n_tok,&chr_pos_fnc,&a->_id_conn,a->_overflowVector,FIND)==1)//Conectado desde el modulo
			{
			AT_decode=at_tcp_conectado;
			}
		else
		{
			}}}}}}}}}}}

	}
	else
	{
	//--------------------------------------------------------------------------------------------------------------//
	// 	Continuo preguntando por la sentencia ERROR, y luego busco otros string dentro de lo recibido     			 //
	//--------------------------------------------------------------------------------------------------------------//
		 a->_n_fcomp=strlen("ERROR\r\n");
		if (FT_String_ND(a->_uartRCVD,&a->_n_orig,"ERROR\r\n",&a->_n_fcomp,a->_uartRCVD_tok,&a->_n_tok,&chr_pos_fnc,&a->_id_conn,a->_overflowVector,FIND)==1)
		{
				AT_decode=at_error;
				//------------------TCP CAIDO AL INTENTAR CONECTAR------------------//
				a->_n_fcomp=strlen("CLOSED\r\n");
				if (FT_String_ND(a->_uartRCVD,&a->_n_orig,"CLOSED\r\n",&a->_n_fcomp,a->_uartRCVD_tok,&a->_n_tok,&chr_pos_fnc,&a->_id_conn,a->_overflowVector,FIND)==1)//Intentando conectar con servidor TCP caido
				{
						AT_decode=at_tcp_conn_err;
				}
				 else
				 {
				//------------------CONECTAR TCP YA CONECTADO------------------//
				a->_n_fcomp=strlen("\r\nALREADY CONNECTED\r\n");
				if (FT_String_ND(a->_uartRCVD,&a->_n_orig,"\r\nALREADY CONNECTED\r\n",&a->_n_fcomp,a->_uartRCVD_tok,&a->_n_tok,&chr_pos_fnc,&a->_id_conn,a->_overflowVector,FIND)==1)//Error desconectar TCP ya desconectado
				{
					 	AT_decode=at_tcp_alrdy_cnntd_err;
				}
				else
				{
				//------------------OK PARA ENVIAR------------------//
				a->_n_fcomp=strlen("AT+CIPSERVER");
				if (FT_String_ND(a->_uartRCVD,&a->_n_orig,"AT+CIPSERVER",&a->_n_fcomp,a->_uartRCVD_tok,&a->_n_tok,&chr_pos_fnc,&a->_id_conn,a->_overflowVector,FIND)==1)
				{
					AT_decode=at_tcp_srvr_err;
				}
				else
				{
				//------------------DESCONECTAR TCP CUANDOYA EST� DESCONECTADO------------------//
				a->_n_fcomp=strlen("AT+CIPCLOSE");
				if (FT_String_ND(a->_uartRCVD,&a->_n_orig,"AT+CIPCLOSE",&a->_n_fcomp,a->_uartRCVD_tok,&a->_n_tok,&chr_pos_fnc,&a->_id_conn,a->_overflowVector,FIND)==1)//Error desconectar TCP ya desconectado
					{
						AT_decode=at_tcp_close_err;
					}
				else
				{
				//------------------INTENTAR ENVIAR DATOS EN TCP CAIDO------------------//
				a->_n_fcomp=strlen("link is not valid");
				if (FT_String_ND(a->_uartRCVD,&a->_n_orig,"link is not valid",&a->_n_fcomp,a->_uartRCVD_tok,&a->_n_tok,&chr_pos_fnc,&a->_id_conn,a->_overflowVector,FIND)==1)//if (strcmp(a->_uartRCVD,"CLOSED\r\n")==0)//Cerrado TCP desde la APP
					{
						AT_decode=at_tcp_snd_err;
					}
				else
				{
				//------------------CAMBIAR MODO DE FUNCIONAMIENTO------------------//
				a->_n_fcomp=strlen("AT+CWMODE=");
				if (FT_String_ND(a->_uartRCVD,&a->_n_orig,"AT+CWMODE=",&a->_n_fcomp,a->_uartRCVD_tok,&a->_n_tok,&chr_pos_fnc,&a->_id_conn,a->_overflowVector,FIND)==1)//Error desconectar TCP ya desconectado
					{
						AT_decode=at_cambiar_modo_err;
					}
				else
				{
				//------------------DEFIIR MULTIPLES CONEXIONES OK------------------//
				a->_n_fcomp=strlen("AT+CIPMUX=1");
				if (FT_String_ND(a->_uartRCVD,&a->_n_orig,"AT+CIPMUX=1",&a->_n_fcomp,a->_uartRCVD_tok,&a->_n_tok,&chr_pos_fnc,&a->_id_conn,a->_overflowVector,FIND)==1)
					{
						AT_decode=at_multiple_conn_err;
					}
				else
				{
				//------------------DEFIIR NO MULTIPLES CONEXIONES OK------------------//
				a->_n_fcomp=strlen("AT+CIPMUX=0");
				if (FT_String_ND(a->_uartRCVD,&a->_n_orig,"AT+CIPMUX=0",&a->_n_fcomp,a->_uartRCVD_tok,&a->_n_tok,&chr_pos_fnc,&a->_id_conn,a->_overflowVector,FIND)==1)
					{
						AT_decode=at_no_multiple_conn_err;
					}
				else
				{
				//------------------ERROR AL CONECTAR WIFI------------------//
				a->_n_fcomp=strlen("AT+CWJAP");
				if (FT_String_ND(a->_uartRCVD,&a->_n_orig,"AT+CWJAP",&a->_n_fcomp,a->_uartRCVD_tok,&a->_n_tok,&chr_pos_fnc,&a->_id_conn,a->_overflowVector,FIND)==1)
					{
						AT_decode=at_conn_wifi_err;
					}
				else
				{
				//------------------ERROR AL CONECTAR WIFI------------------//
				a->_n_fcomp=strlen("AT+CIPSTA=");
				if (FT_String_ND(a->_uartRCVD,&a->_n_orig,"AT+CIPSTA=",&a->_n_fcomp,a->_uartRCVD_tok,&a->_n_tok,&chr_pos_fnc,&a->_id_conn,a->_overflowVector,FIND)==1)
					{
						AT_decode=at_def_ip_err;
					}
				else
				{
				//------------------ERROR AL CONECTAR WIFI------------------//
				a->_n_fcomp=strlen("no ip");
				if (FT_String_ND(a->_uartRCVD,&a->_n_orig,"no ip",&a->_n_fcomp,a->_uartRCVD_tok,&a->_n_tok,&chr_pos_fnc,&a->_id_conn,a->_overflowVector,FIND)==1)
					{
						AT_decode=at_tcp_noip_err;  //PROCESAR
					}
				else
				{




				 }}}}}}}}}}}
		}
		else
		{
			 //--------------------------------------------------------------------------------------------------------------//
			 // 	Continuo preguntando por las string individuales, los que nos tienen datos comunes		     			 //
			 //--------------------------------------------------------------------------------------------------------------//
			a->_n_fcomp=strlen("WIFI DISCONNECT");
			if (FT_String_ND(a->_uartRCVD,&a->_n_orig,"WIFI DISCONNECT",&a->_n_fcomp,a->_uartRCVD_tok,&a->_n_tok,&chr_pos_fnc,&a->_id_conn,a->_overflowVector,FIND)==1)//if (strcmp(a->_uartRCVD,"WIFI DISCONNECT\r\n\r\nOK\r\n")==0)//WIFI DESCONECTADA
				{
					 AT_decode=at_wifi_disconnect;
					 a->_debug_WF_DISCONNECT++;
				}
			else
			{
			//------------------CLIENTE DESCONECTADO DEL SERVIDOR ------------------//
			a->_n_fcomp=strlen(",CLOSED\r\n");
			if (FT_String_ND(a->_uartRCVD,&a->_n_orig,",CLOSED\r\n",&a->_n_fcomp,a->_uartRCVD_tok,&a->_n_tok,&chr_pos_fnc,&a->_id_conn,a->_overflowVector,FIND)==1)//if (strcmp(a->_uartRCVD,"CLOSED\r\n")==0)//Cerrado TCP desde la APP
				{
					AT_decode=at_tcp_client_desc;
					a->_debug_CLOSED++;
				}
			else
			{
			//------------------TCP CAIDO DESPUES DE CONECTADO------------------//
			// En el envío contínuo el CLOSED se concatena con el SEND OK y si //
			// primero ve el SEND OKse queda en el estado 705 por error al     //
			//  enviar														  //
			//-----------------------------------------------------------------//
			a->_n_fcomp=strlen("CLOSED\r\n");
			if (FT_String_ND(a->_uartRCVD,&a->_n_orig,"CLOSED\r\n",&a->_n_fcomp,a->_uartRCVD_tok,&a->_n_tok,&chr_pos_fnc,&a->_id_conn,a->_overflowVector,FIND)==1)//if (strcmp(a->_uartRCVD,"CLOSED\r\n")==0)//Cerrado TCP desde la APP
				{
					AT_decode=at_tcp_desconectado;
				}
			else
			{

			//------------------ENVIADO = RECIBIDO ------------------//
			if (FT_String_ND(a->_uartRCVD,&a->_n_orig,a->_uart2snd,&a->_n_cmp,a->_uartRCVD_tok,&a->_n_tok,&chr_pos_fnc,&a->_id_conn,a->_overflowVector,FIND)==1)//if (strcmp(a->_uartRCVD,a->_uart2snd)==0)//Recibo OK para transmitir
				 {
				 	if (a->_uartRCVD[0] != '\0') //Borro el vector de recepción
				 		{
				 		//AT_decode=10;
				 		a->_n_orig=0;
				 		}
				 	}
			else
			{
			//------------------PAQUETE ENVIADO EN FALLA--------------//
			a->_n_fcomp=strlen("\r\nSEND FAIL\r\n");
			 if (FT_String_ND(a->_uartRCVD,&a->_n_orig,"\r\nSEND FAIL\r\n",&a->_n_fcomp,a->_uartRCVD_tok,&a->_n_tok,&chr_pos_fnc,&a->_id_conn,a->_overflowVector,FIND)==1)//if (strcmp(a->_uartRCVD,"\r\nSEND OK\r\n")==0)//Transmision ok
			 	{
			 	 AT_decode=at_tcp_enviado_error;
			 	 a->_debug_FAIL++;
			 	}
			 else
			 {



			//------------------OBTENCI�N DE IP------------------//
			// vienen concatenadas no logra ver el GOT IP se queda en el WIFI CONNECTED
			a->_n_fcomp=strlen("WIFI GOT IP\r\n");
			if (FT_String_ND(a->_uartRCVD,&a->_n_orig,"WIFI GOT IP\r\n",&a->_n_fcomp,a->_uartRCVD_tok,&a->_n_tok,&chr_pos_fnc,&a->_id_conn,a->_overflowVector,FIND)==1)//Obtiene IP
				{
					a->_debug_WF_GOTIP++;
					AT_decode=at_wifi_gotip;
				}
			else
			{
				//------------------WIFI CONECTADA------------------//
			a->_n_fcomp=strlen("WIFI CONNECTED");
			if (FT_String_ND(a->_uartRCVD,&a->_n_orig,"WIFI CONNECTED",&a->_n_fcomp,a->_uartRCVD_tok,&a->_n_tok,&chr_pos_fnc,&a->_id_conn,a->_overflowVector,FIND)==1)//WIFI CONECTADA
				{
					a->_debug_WF_CNNCTD++;
						 AT_decode=at_wifi_connected;
					//Cuando se cuelga por falla de alimentación queda acá, si es que el módulo
					//llega a responder nuevamnte.
				}
			else
			{

			//------------------CLIENTE TCP CONECTADO------------------//
			a->_n_fcomp=strlen(",CONNECT\r\n");
			if (FT_String_ND(a->_uartRCVD,&a->_n_orig,",CONNECT\r\n",&a->_n_fcomp,a->_uartRCVD_tok,&a->_n_tok,&chr_pos_fnc,&a->_id_conn,a->_overflowVector,FIND)==1)//Conectado desde el modulo
				  {
					  AT_decode=at_tcp_client_conectado;
					  a->_debug_CONNECT++;
				  }
			else
			{

			//------------------FALLA------------------//
			a->_n_fcomp=strlen("\r\nFAIL\r\n");
				if (FT_String_ND(a->_uartRCVD,&a->_n_orig,"\r\nFAIL\r\n",&a->_n_fcomp,a->_uartRCVD_tok,&a->_n_tok,&chr_pos_fnc,&a->_id_conn,a->_overflowVector,FIND)==1)//Conectado desde el modulo
				{
					AT_decode=at_fail;
				}
			else
			{
			//------------------busy processing------------------//
				a->_n_fcomp=strlen("busy p...");
				if (FT_String_ND(a->_uartRCVD,&a->_n_orig,"busy p...",&a->_n_fcomp,a->_uartRCVD_tok,&a->_n_tok,&chr_pos_fnc,&a->_id_conn,a->_overflowVector,FIND)==1)//Conectado desde el modulo
					{
					AT_decode=at_busy_p;
					}
				else
				{

			}

		}}}}}}}}}}
	}

 //Eval�o todo lo recibido desde el puerto serie

		switch (a->_instruccion)
		{
			//--------------------------------------------------------//
			//					Cambiar CWMODE						  //
		    //--------------------------------------------------------//
			case 1:
			{
				a->_uart2snd[0]='\0';		//En teor�a borro lo que tenga el vector
				if(( a->_enviaruart==1)&&(AT_decode!=at_ok)
									   &&(AT_decode!=at_error)
									   &&(AT_decode!=at_restart)
									   &&(AT_decode!=at_tcp_enviado_ok))
						{
						a->_pasos=0;
						a->_ejecucion=1;
						strncat(a->_uart2snd,"AT+CWMODE=1\r\n",strlen("AT+CWMODE=1\r\n"));
						a->_n_uart2SND=strlen(a->_uart2snd);
						a->_estado=100;
						}
				a->_pasos++;
				if (((a->_enviaruart==0)&&((AT_decode==at_ok)||(AT_decode==at_cambiar_modo_ok)
															 ||(AT_decode==at_error)
															 ||(AT_decode==at_restart)
															 ||(AT_decode==at_wifi_connected)
															 ||(AT_decode==at_wifi_gotip)
															 ||(AT_decode==at_tcp_enviado_ok)))
															 ||(a->_ticks > 5000))//210630 Se agrega ATDecode 11 y 12 //Que analice luego de enviar por uart
					{
						a->_estado=AT_decode;	//Devuelvo el estado WIFI Conectada
						a->_instruccion=0;	//Finalizo la instrucci�n
						a->_ejecucion=0;
						a->_ticks=0;
					}

			}
			break;
			//--------------------------------------------------------//
			//					Conectar WiFi						  //
		    //--------------------------------------------------------//
			case 2:
			{
				a->_uart2snd[0]='\0';		//En teor�a borro lo que tenga el vector
				if(( a->_enviaruart==1)&&(AT_decode!=at_ok)
									   &&(AT_decode!=at_error)
									   &&(AT_decode!=at_fail)
									   &&(AT_decode!=at_restart)
									   &&(AT_decode!=at_wifi_disconnect)
									   &&(AT_decode!=at_wifi_connected)
									   &&(AT_decode!=at_wifi_gotip))
						{
						a->_pasos=0;
						a->_ejecucion=1;
						////HAL_GPIO_WritePin(a->RESET_PORT, a->RESET_PIN , GPIO_PIN_SET);
						//------Generacion del comando para conectar---------//
						strncat(a->_uart2snd,"AT+CWJAP=",strlen("AT+CWJAP="));
						strncat(a->_uart2snd,&comillas,1);
						strncat(a->_uart2snd,a->_WF_Net,strlen(a->_WF_Net));
						strncat(a->_uart2snd,separador,strlen(separador));//strncat(a->_uart2snd,separador,3);//
						strncat(a->_uart2snd,a->_WF_Pass,strlen(a->_WF_Pass));
						strncat(a->_uart2snd,finalizar,3);//strncat(a->_uart2snd,finalizar,strlen(finalizar));
						a->_n_uart2SND=strlen(a->_uart2snd);//210418
						a->_estado=200;
						//------Generacion del comando para conectar---------//
						}

				a->_pasos++;

				if (((a->_enviaruart==0)&&((AT_decode==at_ok)||(AT_decode==at_error)
															||(AT_decode==at_fail)
															||(AT_decode==at_restart)
															||(AT_decode==at_wifi_disconnect)
															||(AT_decode==at_wifi_connected)
															||(AT_decode==at_wifi_gotip)))
															||(a->_ticks2 > 20000))  //Que analice luego de enviar por uart
					{
						a->_estado=AT_decode;	//Devuelvo el estado WIFI Conectada
						if(AT_decode!=at_wifi_disconnect)
						{
							a->_instruccion=0;	//Para que siga contando por timeout al recibir WIFI DISCONNECT//Finalizo la instrucci�n
							a->_ejecucion=0;
							//HW_RESET(a);
							//HAL_GPIO_WritePin(a->RESET_PORT, a->RESET_PIN , GPIO_PIN_RESET);
							//HAL_GPIO_WritePin(GPIOB, GPIO_PIN_9, GPIO_PIN_RESET);
						}

						a->_ticks2=0;
					}

			}
			break;
			//--------------------------------------------------------//
			//					Desconectar WiFi					  //
		    //--------------------------------------------------------//
			case 3:
			{
				a->_uart2snd[0]='\0';		//En teor�a borro lo que tenga el vector

				if(( a->_enviaruart==1)&&(AT_decode!=at_restart	)&&(AT_decode!=at_wifi_disconnect)&&(AT_decode!=at_deconectar_ok ))
						{
						a->_pasos=0;
						a->_ejecucion=1;
						//------Generacion del comando para desconectar---------//
						strncat(a->_uart2snd,"AT+CWQAP\r\n",strlen("AT+CWQAP\r\n"));
						a->_n_uart2SND=strlen(a->_uart2snd);
						//------Generacion del comando para desconectar---------//
						a->_estado=300;
						}

				a->_pasos++;
				if (((a->_enviaruart==0)&&((AT_decode==at_wifi_disconnect)||(AT_decode==at_wifi_connected)
																		  ||(AT_decode==at_wifi_gotip)
																		  ||(AT_decode==at_restart	)
																		  ||(AT_decode==at_deconectar_ok )))
																		  ||(a->_ticks > 5000))  //Que analice luego de enviar por uart
					{
						a->_estado=AT_decode;	//Devuelvo el estado WIFI Conectada
						a->_instruccion=0;	//Finalizo la instrucci�n
						a->_ejecucion=0;
						a->_ticks=0;
					}
					//a->_ticks=0;						//Reseteo cada vez que entro
  		}
			break;
			//--------------------------------------------------------//
			//					Multiples Conexiones     			  //
		    //--------------------------------------------------------//
			case 4:
			{
				a->_uart2snd[0]='\0';		//En teor�a borro lo que tenga el vector

				if(( a->_enviaruart==1)&&(AT_decode!=at_restart	)
									   &&(AT_decode!=at_ok)
									   &&(AT_decode!=at_error))
						{
						a->_pasos=0;
						a->_ejecucion=1;
						//------Generacion del comando para desconectar---------//
						if(a->_TCP_Local_Server_EN==1)
						{
							strncat(a->_uart2snd,"AT+CIPMUX=1\r\n",strlen("AT+CIPMUX=1\r\n"));
						}
						else
						{
							strncat(a->_uart2snd,"AT+CIPMUX=0\r\n",strlen("AT+CIPMUX=0\r\n"));
						}
						a->_n_uart2SND=strlen(a->_uart2snd);
						//------Generacion del comando para desconectar---------//
						a->_estado=400;
						}

				a->_pasos++;
				if (((a->_enviaruart==0)&&((AT_decode==at_ok)||(AT_decode==at_wifi_connected)
															||(AT_decode==at_wifi_gotip)
															||(AT_decode==at_restart)
															||(AT_decode==at_error)
															||(AT_decode==at_multiple_conn_ok)
															||(AT_decode==at_no_multiple_conn_ok)
															||(AT_decode==at_multiple_conn_err)
															||(AT_decode==at_no_multiple_conn_err)))
															||(a->_ticks > 5000))  //Que analice luego de enviar por uart
					{
						a->_estado=AT_decode;	//Devuelvo el estado WIFI Conectada
						a->_instruccion=0;	//Finalizo la instrucci�n
						a->_ejecucion=0;
						a->_ticks=0;
					}
					//a->_ticks=0;						//Reseteo cada vez que entro
  		}
			break;
			//--------------------------------------------------------//
			//					Especificar IP           			  //
		    //--------------------------------------------------------//
			case 5:
			{
				a->_uart2snd[0]='\0';		//En teor�a borro lo que tenga el vector

				if(( a->_enviaruart==1)&&(AT_decode!=at_restart)
									   &&(AT_decode!=at_ok)
									   &&(AT_decode!=at_error)
									   &&(AT_decode!=at_def_ip_err)
									   &&(AT_decode!=at_def_ip_ok))
						{
						a->_pasos=0;
						a->_ejecucion=1;
						//------Generacion del comando para desconectar---------//
						strncat(a->_uart2snd,"AT+CIPSTA=",strlen("AT+CIPSTA="));
						strncat(a->_uart2snd,&comillas,1);
						strncat(a->_uart2snd,a->_TCP_Local_Server_IP,strlen(a->_TCP_Local_Server_IP));
						strncat(a->_uart2snd,separador,3);
						strncat(a->_uart2snd,a->_TCP_Local_Server_GWY,strlen(a->_TCP_Local_Server_GWY));
						strncat(a->_uart2snd,separador,3);
						strncat(a->_uart2snd,a->_TCP_Local_Server_MSK,strlen(a->_TCP_Local_Server_MSK));
						strncat(a->_uart2snd,&comillas,1);
						strncat(a->_uart2snd,finalizar2,2);//strncat(a->_uart2snd,finalizar2,strlen(finalizar2));

						a->_n_uart2SND=strlen(a->_uart2snd);
						//------Generacion del comando para desconectar---------//
						a->_estado=500;
						}

				a->_pasos++;
				if (((a->_enviaruart==0)&&((AT_decode==at_ok)||(AT_decode==at_wifi_connected)
															 ||(AT_decode==at_wifi_gotip)
															 ||(AT_decode==at_restart)
															 ||(AT_decode==at_error)
															 ||(AT_decode==at_def_ip_err)
															 ||(AT_decode==at_def_ip_ok)))
															 ||(a->_ticks > 5000))  //Que analice luego de enviar por uart
					{
						a->_estado=AT_decode;	//Devuelvo el estado WIFI Conectada
						a->_instruccion=0;	//Finalizo la instrucci�n
						a->_ejecucion=0;
						a->_ticks=0;
					}
					//a->_ticks=0;						//Reseteo cada vez que entro
  		}
			break;
			//--------------------------------------------------------//
			//					Conectar a TCP Server    			  //
		    //--------------------------------------------------------//
			case 6:
			{
				a->_uart2snd[0]='\0';		//En teor�a borro lo que tenga el vector

				if(( a->_enviaruart==1)&&(AT_decode!=at_wifi_connected)
									   &&(AT_decode!=at_wifi_gotip)
									   &&(AT_decode!=at_tcp_alrdy_cnntd_err)
									   &&(AT_decode!=at_tcp_conectado)
									   &&(AT_decode!=at_tcp_desconectado)
									   &&(AT_decode!=at_error))
						{
						a->_pasos=0;
						a->_ejecucion=1;
						//------Generacion del comando ---------//
						strncat(a->_uart2snd,"AT+CIPSTART=",strlen("AT+CIPSTART="));
						strncat(a->_uart2snd,&comillas,1);
						strncat(a->_uart2snd,"TCP",strlen("TCP"));
						strncat(a->_uart2snd,separador,3);
						strncat(a->_uart2snd,a->_TCP_Remote_Server_IP,strlen(a->_TCP_Remote_Server_IP));
						strncat(a->_uart2snd,separador2,2);
						strncat(a->_uart2snd,a->_TCP_Remote_Server_Port,strlen(a->_TCP_Remote_Server_Port));
						strncat(a->_uart2snd,finalizar2,strlen(finalizar2));
						a->_n_uart2SND=strlen(a->_uart2snd);
						a->_estado=600;
						//------Generacion del comando ---------//
						}

				a->_pasos++;
				if (((a->_enviaruart==0)&&((AT_decode==at_wifi_disconnect)||(AT_decode==at_wifi_connected)
																		  ||(AT_decode==at_wifi_gotip)
																		  ||(AT_decode==at_tcp_alrdy_cnntd_err)
																		  ||(AT_decode==at_tcp_conectado)
																		  ||(AT_decode==at_tcp_desconectado)
																		  ||(AT_decode==at_error)
																		  ||(AT_decode==at_busy_p)))
																		  ||(a->_ticks > 5000))//210419 if (((a->_enviaruart==0)&&((estado==5)||(estado==6)||(estado==7)||(estado==13)||(estado==14)))||(a->_ticks > 5000))  //Que analice luego de enviar por uart
					{
						a->_estado=AT_decode;	//Devuelvo el estado WIFI Conectada
						a->_instruccion=0;	//Finalizo la instrucci�n
						a->_ejecucion=0;
						a->_ticks=0;
					}
					//a->_ticks=0;						//Reseteo cada vez que entro
			}
			break;
			//--------------------------------------------------------//
			//			   Enviar datos a TCP Server    			  //
		    //--------------------------------------------------------//
			case 7:
			{
					unsigned char caracteres[8]="";



				if((AT_decode==at_tcp_desconectado)||(AT_decode==at_wifi_disconnect))
				{
					a->_estado=AT_decode;
					a->_instruccion=0;		//Finalizo la instrucci�n
					a->_ejecucion=0;
					a->_subpaso=0;
				}
				else
				{
							if( (a->_enviaruart==1)&&(a->_subpaso==0))
								{
										a->_ejecucion=1;
										a->_estado=24;
										a->_uart2snd[0]='\0';		//En teor�a borro lo que tenga el vector
										a->_subpaso++;					//Asigno para que no vuelva a entrar
										a->_pasos=0;
										a->_okenvio[0]='\0';	  //Borro el vector a->_okenvio
										//------Generacion del comando ---------//
		// El problema está al enviar el caracater nulo, se deben contar los caracteres nulo del vector uart2nd, o bien informar
		//la cantidad de datos a enviar, es posible recorrer todo el vector con un while y contar hasta el ultimo dato (que lo detecte
										//------Generacion del comando ---------//
										strncat(a->_uart2snd,"AT+CIPSEND=",strlen("AT+CIPSEND="));
										INTOA(a->_n_D2SND, caracteres);	//INTOA(strlen(a->_data2SND), caracteres);		        //Longitud  de datos a enviar
										strncat(a->_uart2snd,caracteres,strlen(caracteres));//strncat(a->_uart2snd,caracteres,strlen(caracteres));//AT+CIPSEND="cdad caracteres"
										strcpy(a->_okenvio,a->_uart2snd);
										strncat(a->_uart2snd,finalizar2,2);//strncat(a->_uart2snd,finalizar2,strlen(finalizar2));
										a->_n_uart2SND=strlen(a->_uart2snd);
										strncat(a->_okenvio,"\r\r\n\r\nOK\r\n> ",strlen("\r\r\n\r\nOK\r\n> "));
										a->_debug_count3++;
										a->_estado=700;
										//------Generacion del comando ---------//
								}

								a->_pasos++;

							if ((a->_enviaruart==0)&&(AT_decode!=at_tcp_enviado_ok)
												   &&((strcmp(a->_uartRCVD,a->_okenvio)==0)||(AT_decode==at_ok)
														   	   	   	   	   	   	   	   	   ||(AT_decode==at_tcp_ok_to_send)
																						   ||((a->_ticks > 5000)&&(a->_subpaso==1))))
							{//---------------------------------------------------Env�o el dato
								if(a->_ticks > 5000)//if((a->_ticks > 5000)||(a->_subpaso==1))//((a->_ticks > 5000)&&(a->_subpaso==1))
								{
									a->_debug_count7++;//-----------------------------Cuento los que entran por timeout
								}
								a->_debug_count4++;
								a->_subpaso++;
								a->_enviaruart=1;
								CopiaVector(a->_uart2snd,a->_data2SND,a->_n_D2SND,1,'a');//strcpy(a->_uart2snd,a->_data2SND);
								a->_n_uart2SND=a->_n_D2SND; //Asigno cantidad de elementos del vector
								a->_ticks=0;//----------------------------------Evito que entre de toque en el siguiente
							}

							if((a->_estado==700)&&((AT_decode==at_tcp_enviado_ok)||(a->_ticks > 5000)
																				 ||(AT_decode==at_wifi_disconnect)
																				 ||(AT_decode==at_wifi_connected)
																				 ||(AT_decode==at_wifi_gotip)
																				 ||(AT_decode==at_tcp_desconectado)
																				 ||(AT_decode==at_tcp_snd_err)
																				 ||(AT_decode==at_tcp_enviado_error)
																				 ||(AT_decode==at_busy_p)))
								{
									a->_debug_count5++;
									if(a->_ticks > 5000)
									{
										a->_debug_count8++;
										a->_subpaso++;
										a->_ticks=0;
									}

									if((a->_subpaso > 2)&&(AT_decode!=at_tcp_desconectado)&&(AT_decode!=at_wifi_disconnect))//--------------No considero los etados de fallas por TCP o WIFI caido
									{
										a->_estado=15;//-----------------------------Error de Env�o TCP
									}
									else
									{
										a->_estado=AT_decode;	//Devuelvo el estado
									}
									a->_instruccion=0;		//Finalizo la instrucci�n
									a->_ejecucion=0;
									a->_subpaso=0;
								}
				}
					//a->_ticks=0;						//Reseteo cada vez que entro
			}
			break;
			//--------------------------------------------------------//
			//					Crear Local TCP Server    			  //
		    //--------------------------------------------------------//
			case 8:
			{
				a->_uart2snd[0]='\0';		//En teor�a borro lo que tenga el vector

				if( a->_enviaruart==1)
						{
						a->_pasos=0;
						a->_ejecucion=1;
						//------Generacion del comando ---------//
						strncat(a->_uart2snd,"AT+CIPSERVER=1,",strlen("AT+CIPSERVER=1,"));
						strncat(a->_uart2snd,a->_TCP_Local_Server_Port,strlen(a->_TCP_Local_Server_Port));
						strncat(a->_uart2snd,finalizar2,strlen(finalizar2));
						a->_n_uart2SND=strlen(a->_uart2snd);
						a->_estado=800;
						//------Generacion del comando ---------//
						}
				a->_pasos++;
				if (((a->_enviaruart==0)&&((AT_decode==at_wifi_disconnect)||(AT_decode==at_wifi_connected)||(AT_decode==at_wifi_gotip)||(AT_decode==at_tcp_alrdy_cnntd_err)||(AT_decode==at_tcp_conectado)||(AT_decode==at_tcp_desconectado)||(AT_decode==21)||(AT_decode==22)||(AT_decode==23)))||(a->_ticks > 5000))//210419 if (((a->_enviaruart==0)&&((estado==5)||(estado==6)||(estado==7)||(estado==13)||(estado==14)))||(a->_ticks > 5000))  //Que analice luego de enviar por uart
					{
						a->_estado=AT_decode;	//Devuelvo el estado WIFI Conectada
						a->_instruccion=0;	//Finalizo la instrucci�n
						a->_ejecucion=0;
						a->_ticks=0;
					}
			}
			break;
			//--------------------------------------------------------//
			//			   Enviar datos desde TCP Server    			  //
		    //--------------------------------------------------------//
			case 9:
			{
					unsigned char caracteres[8]="";

					if((AT_decode==at_tcp_desconectado)||(AT_decode==at_wifi_disconnect))
				{
					a->_estado=AT_decode;
					a->_instruccion=0;		//Finalizo la instrucci�n
					a->_ejecucion=0;
					a->_subpaso=0;
				}
				else
				{
							if( (a->_enviaruart==1)&&(a->_subpaso==0))
								{
										a->_ejecucion=1;
										a->_estado=24;
										a->_uart2snd[0]='\0';		//En teor�a borro lo que tenga el vector
										a->_subpaso++;					//Asigno para que no vuelva a entrar
										a->_pasos=0;
										a->_okenvio[0]='\0';	  //Borro el vector a->_okenvio
										//------Generacion del comando ---------//
		// El problema está al enviar el caracater nulo, se deben contar los caracteres nulo del vector uart2nd, o bien informar
		//la cantidad de datos a enviar, es posible recorrer todo el vector con un while y contar hasta el ultimo dato (que lo detecte
										//------Generacion del comando ---------//
										strncat(a->_uart2snd,"AT+CIPSEND=",strlen("AT+CIPSEND="));//strncat(a->_uart2snd,"AT+CIPSEND=0,",strlen("AT+CIPSEND=0,"));
										INTOA(a->_id_conn, caracteres);
										strncat(a->_uart2snd,caracteres,strlen(caracteres));
										strncat(a->_uart2snd,",",1);
										INTOA(a->_n_D2SND, caracteres);	//INTOA(strlen(a->_data2SND), caracteres);		        //Longitud  de datos a enviar
										strncat(a->_uart2snd,caracteres,strlen(caracteres));//strncat(a->_uart2snd,caracteres,strlen(caracteres));//AT+CIPSEND="cdad caracteres"
										strcpy(a->_okenvio,a->_uart2snd);
										strncat(a->_uart2snd,finalizar2,2);//strncat(a->_uart2snd,finalizar2,strlen(finalizar2));
										a->_n_uart2SND=strlen(a->_uart2snd);
										strncat(a->_okenvio,"\r\r\n\r\nOK\r\n> ",strlen("\r\r\n\r\nOK\r\n> "));
										a->_debug_count3++;
										a->_estado=900;
										//------Generacion del comando ---------//
								}

								a->_pasos++;

							if ((a->_enviaruart==0)&&(AT_decode!=at_tcp_enviado_ok)&&((strcmp(a->_uartRCVD,a->_okenvio)==0)||(AT_decode==at_ok)||(AT_decode==20)||((a->_ticks > 5000)&&(a->_subpaso==1))))
							{//---------------------------------------------------Env�o el dato
								if((a->_ticks > 5000)&&(a->_subpaso==1))
								{
									a->_debug_count7++;//-----------------------------Cuento los que entran por timeout
								}
								a->_debug_count4++;
								a->_subpaso++;
								a->_enviaruart=1;
								CopiaVector(a->_uart2snd,a->_data2SND,a->_n_D2SND,1,'a');//strcpy(a->_uart2snd,a->_data2SND);
								a->_n_uart2SND=a->_n_D2SND; //Asigno cantidad de elementos del vector
								a->_ticks=0;//----------------------------------Evito que entre de toque en el siguiente
							}

							if((a->_estado==900)&&((AT_decode==at_tcp_enviado_ok)||(AT_decode==at_wifi_connected)||(AT_decode==at_wifi_gotip)||(a->_ticks > 5000)||(AT_decode==at_wifi_disconnect)||(AT_decode==at_tcp_desconectado)||(AT_decode==at_tcp_snd_err)||(AT_decode==at_tcp_enviado_error)||(AT_decode==22)||(AT_decode==25)))
								{
									a->_debug_count5++;
									if(a->_ticks > 5000)
									{
										a->_debug_count8++;
										a->_subpaso++;
										a->_ticks=0;
									}

									if((a->_subpaso > 2)&&(AT_decode!=at_tcp_desconectado)&&(AT_decode!=at_wifi_disconnect))//--------------No considero los etados de fallas por TCP o WIFI caido
									{
										//a->_estado=15;//-----------------------------Error de Env�o TCP
										//No tengo definido el estado
									}
									else
									{
										a->_estado=AT_decode;	//Devuelvo el estado
									}
									a->_instruccion=0;		//Finalizo la instrucci�n
									a->_ejecucion=0;
									a->_subpaso=0;
								}
				}
					//a->_ticks=0;						//Reseteo cada vez que entro
			}
			break;
			default:		//Si no hay instruccion paso el estado a la estructura
			{
				a->_estado=AT_decode;
			}
			break;
		}


		a->_uartRCVD[0]='\0'; //200829  Elimino el registro de recepcion, solo se vuelve a actualizar en TIMEOUT
		a->_n_orig=0;

		return (a->_estado);

 }
 // estado no cambia a->estado salvo que dicho estado sea algun valor esperado en la funcion
 //	la recepcion de datos no afecta la variable a->estado, pero puede afectar el registro recibido, en tal caso la funci�n de cae por timeout de rpta a->ticks=5eg
 /*--------------------------------------------------------------*/
 void AT_Enviar_String( char *B , char *D, char *E)
 {
	int el=0, b=0;
	int a=strlen(B); //Obtengo la cantidad de caracteres de la info
	char string_envio[15]="AT+CIPSEND=";
	char string_envio2[]={""};
	b=INTOA(a, string_envio2);

	strncat(string_envio,string_envio2,strlen(string_envio2));

	while (el< strlen(string_envio))
	{
		*D++=string_envio[el];
		*E++=string_envio[el];
		el++;
	}
	  *D++=0x0D;
	  *D++=0x0A;
	  *E++=0x0D;
		*E++=0x0D;
		*E++=0x0A;
		*E++=0x0D;
		*E++=0x0A;
		*E++='O';
		*E++='K';
		*E++=0x0D;
		*E++=0x0A;
		*E++='>';
		*E++=0x20;

 }
 /*--------------------------------------------------------------*/

		//--------------Maquina de estados para conectar a TCP
int WiFi_Conn_ND( struct WIFI *b, UART_HandleTypeDef *PORTSER, int EN_DEBUG )
{
		switch (b->_estado_conexion)
		{
			case CAMBIAR_MODO_EN_CURSO:			//WIFI Desconectado --> Conectar a WIFI nuevamente
			{
				if((b->_estado!=100)&&(b->_estado!=at_ok)
									&&(b->_estado!=at_cambiar_modo_ok)
									&&(b->_estado!=at_error)
									&&(b->_estado!=at_restart)
									&&(b->_estado!=at_tcp_enviado_ok))	//Si estoy conectando, no vuelvo a conectar.
				{
						Cambiar_CWMODE(b);
						if(b->_enviaruart==1)
							{
								b->_estado=AT_ESP8266_ND(b);  //Genero las instrucciones a enviar
								b->_enviaruart=0;
								if (b->_DBG_EN==1)
								{
									ITM0_Write((uint8_t*)b->_uart2snd, b->_n_uart2SND); //210915
								}else{}
								HAL_UART_Transmit(PORTSER,(uint8_t*)b->_uart2snd, b->_n_uart2SND,100);
//Acá debería estar le ejecución, cuando transmite, a partir de este momento se
//encuentra a la espera de recibir datos.
//Si los datos no se reciben, meto timeput y vuelvo a enviar.
//Ejecución no tiene condicionales en estructuras IF
							}
						else{}
				}
				else
				{
					//----Condiciones de cambio de estado
					if((b->_estado==at_ok)||
					   (b->_estado==at_cambiar_modo_ok))
						{
						b->_estado_conexion=CAMBIAR_MODO_OK; 	//Cambio de estado exitoso
						}

					if((b->_estado==at_error)||(b->_estado==at_restart)
											 ||(b->_estado==at_wifi_connected)
											 ||(b->_estado==at_wifi_gotip))
						{
						b->_estado_conexion=CAMBIAR_MODO_ERROR; //210630 Se agrega ATDecode 4, 11 y 12	//Cambio de estado erroneo
						}
					//----Condiciones de cambio de estado
				}
			}
			break;
			case CAMBIAR_MODO_ERROR://Error al cambiar de modo
			{
				Cambiar_CWMODE(b);
				b->_estado_conexion=CAMBIAR_MODO_EN_CURSO;
				b->_estado=0;
				b->_n_orig=0; //Borro el vector RX

				b->_TCP_Local_Server_Initiated=0;	//Indico servidor debe ser iniciado
			}
			break;
			case CAMBIAR_MODO_OK ://Cambio de modo OK
			{
//220818
				if (b->_TCP_Local_Server_EN)//
				{//
					MUX(b);	//								//Si me conecté seteo las múltiples Conexiones
					b->_estado_conexion=MUX_CONN_EN_CURSO;//
					b->_estado=0;//

				}//
				else//
				{//
					ConectarWIFI(b);
					b->_estado_conexion=CONEXION_EN_CURSO;
					b->_estado=0;
					b->_n_orig=0; //Borro el vector RX
				}//

				if(b->_estado==at_restart)		//Reinicio involuntario de módulo
					{
						Cambiar_CWMODE(b);
						b->_estado_conexion=CAMBIAR_MODO_EN_CURSO;
						b->_estado=0;

						b->_TCP_Local_Server_Initiated=0;	//Indico servidor debe ser iniciado
					}
			}
			break;
			case CONEXION_EN_CURSO:			//WIFI Desconectado --> Conectar a WIFI nuevamente
			{
				if((b->_estado!=200)&&((b->_estado!=at_ok)
									&&(b->_estado!=at_error)
									&&(b->_estado!=at_fail)
									&&(b->_estado!=at_restart)
						            &&(b->_estado!=at_wifi_disconnect)
									&&(b->_estado!=at_wifi_connected)
									&&(b->_estado!=at_wifi_gotip)))	//Si estoy conectando, no vuelvo a conectar.
				{
						ConectarWIFI(b);
						if(b->_enviaruart==1)
							{
								b->_estado=AT_ESP8266_ND(b);
								b->_enviaruart=0;
								if (b->_DBG_EN==1) ITM0_Write((uint8_t*)b->_uart2snd, b->_n_uart2SND); //210915
								HAL_UART_Transmit(PORTSER,(uint8_t*)b->_uart2snd, b->_n_uart2SND,100);
							}
				}

				//----Condiciones de cambio de estado
				if((b->_estado==at_ok)||(b->_estado==at_wifi_gotip))

					{
						b->_estado_conexion=CONEXION_OK; 	//Ya puedo pedir conexi�n TCP
					}
				if((b->_estado==at_error)||(b->_estado==at_fail)||(b->_estado==at_conn_wifi_err)) b->_estado_conexion=CONEXION_ERROR; 	//Ya puedo pedir conexi�n TCP
				//----Condiciones de cambio de estado
			}
			break;
			case CONEXION_ERROR://Error al conectar
			{
				HAL_Delay(5000); //Cuando entrega el error, lo hace rápido no demora.
				//--------------------------------------------------------------------------------------//
				// Al arrojar error la rutina vuelve a pedir conexion, normalmente el módulo			//
				// tarda en realizar el proceso de conexión cuando AT+CWMODE=1, si AT+CWMODE=2			//
				// el módulo de la respuesta inmediata de error al querer conectar, geberando un		//
				// bucle infinito de ERRORES al querer conectar.										//
				// La unica manera de solucionarlo es seteando nuevamente AT+CWMODE=1					//
				// La realidad es que es muy dificil que se borre la flash una vez inciado el módulo	//
				// se le podría dar un tratamiento pero de momento no lo haremos salvo que sea necesario//
				//--------------------------------------------------------------------------------------//
				ConectarWIFI(b);
				b->_estado_conexion=CONEXION_EN_CURSO;
				b->_estado=0;
				b->_n_orig=0; //Borro el vector RX
				if(b->_estado==at_restart	)		//Reinicio involuntario de módulo
					{
						Cambiar_CWMODE(b);
						b->_estado_conexion=CAMBIAR_MODO_EN_CURSO;
						b->_estado=0;

						b->_TCP_Local_Server_Initiated=0;	//Indico servidor debe ser iniciado
					}
			}
			break;
			case CONEXION_OK://Conecta OK
			{
				b->_n_orig=0; //Borro el vector RX

				if(b->_TCP_Local_Server_Initiated==1)  //Si el servidor ya estaba iniciado, voy al servidor inciado
				{
					b->_estado_conexion=TCP_SRVR_OK ;
					b->_estado=0;
				}

				if((b->_estado==at_wifi_disconnect)||(b->_estado==at_deconectar_ok)) //Si me desconecto, me vuelvo a conectar
				{
					ConectarWIFI(b);
					b->_estado_conexion=CONEXION_EN_CURSO;
					b->_estado=0;
				}

				if(b->_estado==at_restart	)					  //Reinicio involuntario de módulo
					{
						Cambiar_CWMODE(b);
						b->_estado_conexion=CAMBIAR_MODO_EN_CURSO;
						b->_estado=0;
						b->_TCP_Local_Server_Initiated=0;	//Indico servidor debe ser iniciado
					}
//220818
				if(b->_TCP_Local_Server_EN)//
				{//
					CrearServidorTCP(b);
					b->_estado_conexion=TCP_SRVR_EN_CURSO;
					b->_estado=0;
					b->_n_orig=0; //Borro el vector RX
				}//
				else//
				{//
					MUX(b);									//Si me conecté seteo las múltiples Conexiones
					b->_estado_conexion=MUX_CONN_EN_CURSO;
					b->_estado=0;
				}//
			}
			break;
			case DESCONEXION_EN_CURSO:			//Desconectar WiFi
			{
				if((b->_estado!=300)&&(b->_estado!=at_restart)&&(b->_estado!=at_wifi_disconnect)&&(b->_estado!=at_deconectar_ok ))	//Si estoy conectando, no vuelvo a conectar.
				{
						DesconectarWIFI(b);
						if(b->_enviaruart==1)
							{
								b->_estado=AT_ESP8266_ND(b);
								b->_enviaruart=0;
								if (b->_DBG_EN==1) ITM0_Write((uint8_t*)b->_uart2snd, b->_n_uart2SND); //210915
								HAL_UART_Transmit(PORTSER,(uint8_t*)b->_uart2snd, b->_n_uart2SND,100);

							}
				}

				//----Condiciones de cambio de estado
				if((b->_estado==at_wifi_disconnect)||(b->_estado==at_deconectar_ok )) b->_estado_conexion=DESCONEXION_OK; 	//Ya puedo pedir conexi�n TCP
				//----Condiciones de cambio de estado
			}
			break;
			case DESCONEXION_ERROR ://Error al Desconectar WiFi
			{
				NOP(b);
			}
			break;
			case DESCONEXION_OK ://Desconectar WiFi OK
			{
				b->_n_orig=0; //Borro el vector RX

				ConectarWIFI(b);
				b->_estado_conexion=CONEXION_EN_CURSO;
				b->_estado=0;

					if(b->_estado==at_restart	)		//Reinicio involuntario de módulo
					{
						Cambiar_CWMODE(b);
						b->_estado_conexion=CAMBIAR_MODO_EN_CURSO;
						b->_estado=0;


						b->_TCP_Local_Server_Initiated=0;	//Indico servidor debe ser iniciado
					}
			}
			break;
			case MUX_CONN_EN_CURSO:			//Multiples Conexiones
			{
				if((b->_estado!=400)&&(b->_estado!=at_restart)
									&&(b->_estado!=at_ok)
									&&(b->_estado!=at_error)
									&&(b->_estado!=at_multiple_conn_ok)
						            &&(b->_estado!=at_no_multiple_conn_ok)
									&&(b->_estado!=at_multiple_conn_err)
									&&(b->_estado!=at_no_multiple_conn_err))	//Si estoy conectando, no vuelvo a conectar.
				{
						MUX(b);
						if(b->_enviaruart==1)
							{
								b->_estado=AT_ESP8266_ND(b);
								b->_enviaruart=0;
								if (b->_DBG_EN==1) ITM0_Write((uint8_t*)b->_uart2snd, b->_n_uart2SND); //210915
								HAL_UART_Transmit(PORTSER,(uint8_t*)b->_uart2snd, b->_n_uart2SND,100);


							}
				}

				//----Condiciones de cambio de estado
				if((b->_estado==at_ok)||(b->_estado==at_multiple_conn_ok)||(b->_estado==at_no_multiple_conn_ok)) b->_estado_conexion=MUX_CONN_OK;
				if((b->_estado==at_error)||(b->_estado==at_multiple_conn_err)||(b->_estado==at_no_multiple_conn_err)) b->_estado_conexion=MUX_CONN_ERROR;
				//----Condiciones de cambio de estado
			}
			break;
			case MUX_CONN_ERROR://Error Multiples Conexiones
			{
				NOP(b);
				if(b->_estado==at_restart)		//Reinicio involuntario de módulo
				{
					Cambiar_CWMODE(b);
					b->_estado_conexion=CAMBIAR_MODO_EN_CURSO;
					b->_estado=0;
					b->_n_orig=0; //Borro el vector RX

					b->_TCP_Local_Server_Initiated=0;	//Indico servidor debe ser iniciado
				}
				if((b->_estado==at_wifi_disconnect)||(b->_estado==at_deconectar_ok )) //Si me desconecto, me vuelvo a conectar
				{
					ConectarWIFI(b);
					b->_estado_conexion=CONEXION_EN_CURSO;
					b->_estado=0;
					b->_n_orig=0; //Borro el vector RX
				}
			}
			break;
			case MUX_CONN_OK://Multiples Conexiones OK
			{
				NOP(b);
				b->_n_orig=0; //Borro el vector RX

					if(b->_estado==at_restart)		//Reinicio involuntario de módulo
					{
						Cambiar_CWMODE(b);
						b->_estado_conexion=CAMBIAR_MODO_EN_CURSO;
						b->_estado=0;

						b->_TCP_Local_Server_Initiated=0;	//Indico servidor debe ser iniciado
					}
					if((b->_estado==at_wifi_disconnect)||(b->_estado==at_deconectar_ok )) //Si me desconecto, me vuelvo a conectar
					{
						ConectarWIFI(b);
						b->_estado_conexion=CONEXION_EN_CURSO;
						b->_estado=0;
					}

					IPDef(b);
					b->_estado_conexion=DEF_IP_EN_CURSO;
					b->_estado=0;
			}
			break;
			case DEF_IP_EN_CURSO:			//Definir IP
			{
				if((b->_estado!=500)&&(b->_estado!=at_restart)
									&&(b->_estado!=at_ok)
									&&(b->_estado!=at_error)
									&&(b->_estado!=at_def_ip_ok)
									&&(b->_estado!=at_def_ip_err))	//Si estoy conectando, no vuelvo a conectar.
				{
						IPDef(b);
						if(b->_enviaruart==1)
							{
								b->_estado=AT_ESP8266_ND(b);
								b->_enviaruart=0;
								if (b->_DBG_EN==1) ITM0_Write((uint8_t*)b->_uart2snd, b->_n_uart2SND); //210915
								HAL_UART_Transmit(PORTSER,(uint8_t*)b->_uart2snd, b->_n_uart2SND,100);


							}
				}

				//----Condiciones de cambio de estado
				if((b->_estado==at_ok)||(b->_estado==at_def_ip_ok)) b->_estado_conexion=DEF_IP_OK;
				if((b->_estado==at_error)||(b->_estado==at_def_ip_err)) b->_estado_conexion=DEF_IP_ERROR;
				//----Condiciones de cambio de estado
			}
			break;
			case DEF_IP_ERROR://Error Definir IP
			{
				NOP(b);
				if((b->_estado==at_restart)||(b->_estado==at_error))		//Reinicio involuntario de módulo
				{
					Cambiar_CWMODE(b);
					b->_estado_conexion=CAMBIAR_MODO_EN_CURSO;
					b->_estado=0;
					b->_n_orig=0; //Borro el vector RX

					b->_TCP_Local_Server_Initiated=0;	//Indico servidor debe ser iniciado
				}
				if((b->_estado==at_wifi_disconnect)||(b->_estado==at_deconectar_ok )) //Si me desconecto, me vuelvo a conectar
				{
					ConectarWIFI(b);
					b->_estado_conexion=CONEXION_EN_CURSO;
					b->_estado=0;
					b->_n_orig=0; //Borro el vector RX
				}
			}
			break;
			case DEF_IP_OK://Definir IP OK
			{
				NOP(b);

					if(b->_estado==at_restart)		//Reinicio involuntario de módulo
					{
						Cambiar_CWMODE(b);
						b->_estado_conexion=CAMBIAR_MODO_EN_CURSO;
						b->_estado=0;
						b->_n_orig=0; //Borro el vector RX

						b->_TCP_Local_Server_Initiated=0;	//Indico servidor debe ser iniciado
					}
					if((b->_estado==at_wifi_disconnect)||(b->_estado==at_deconectar_ok )) //Si me desconecto, me vuelvo a conectar
					{
						ConectarWIFI(b);
						b->_estado_conexion=CONEXION_EN_CURSO;
						b->_estado=0;
						b->_n_orig=0; //Borro el vector RX
					}

//220818 lo anulado esta presente, para volver atras descomentar

					if (b->_TCP_Local_Server_EN==0)
					{
						ConectarTCP(b);
						b->_estado_conexion=TCP_CONN_EN_CURSO;
						b->_estado=0;
						b->_n_orig=0; //Borro el vector RX
					}
					else
					{
						ConectarWIFI(b);//
						b->_estado_conexion=CONEXION_EN_CURSO;//
						b->_estado=0;//
						b->_n_orig=0;// //Borro el vector RX
						//CrearServidorTCP(b);
						//b->_estado_conexion=TCP_SRVR_EN_CURSO;
						//b->_estado=0;
						//b->_n_orig=0; //Borro el vector RX
					}
			}
			break;
			case TCP_CONN_EN_CURSO:			//Conectar a Servidor TCP
			{
				if((b->_estado!=600)&&(b->_estado!=at_error)
									&&(b->_estado!=at_ok)
									&&(b->_estado!=at_wifi_disconnect)
									&&(b->_estado!=at_tcp_alrdy_cnntd_err)
									&&(b->_estado!=at_tcp_conectado)
									&&(b->_estado!=at_tcp_desconectado))	//Si estoy conectando, no vuelvo a conectar.
				{
					ConectarTCP(b);
						if(b->_enviaruart==1)
							{
								b->_estado=AT_ESP8266_ND(b);
								b->_enviaruart=0;
								if (b->_DBG_EN==1) ITM0_Write((uint8_t*)b->_uart2snd, b->_n_uart2SND); //210915
								HAL_UART_Transmit(PORTSER,(uint8_t*)b->_uart2snd, b->_n_uart2SND,100);


							}
				}

				//----Condiciones de cambio de estado
				if((b->_estado==at_ok)||(b->_estado==at_tcp_alrdy_cnntd_err)||(b->_estado==at_tcp_conectado))
					{
					b->_estado_conexion=TCP_CONN_OK;
					}
				else
				{
					if((b->_estado==at_error)||(b->_estado==at_tcp_desconectado)) b->_estado_conexion=TCP_CONN_ERROR;
				}
				//----Condiciones de cambio de estado
			}
			break;
			case TCP_CONN_ERROR://Error al Conectar a Servidor TCP
			{
				NOP(b);
				if(b->_estado==at_restart)		//Reinicio involuntario de módulo
				{
					Cambiar_CWMODE(b);
					b->_estado_conexion=CAMBIAR_MODO_EN_CURSO;
					b->_estado=0;
					b->_n_orig=0; //Borro el vector RX

					b->_TCP_Local_Server_Initiated=0;	//Indico servidor debe ser iniciado
				}
				if((b->_estado==at_wifi_disconnect)||(b->_estado==at_deconectar_ok )) //Si me desconecto, me vuelvo a conectar
				{
					ConectarWIFI(b);
					b->_estado_conexion=CONEXION_EN_CURSO;
					b->_estado=0;
					b->_n_orig=0; //Borro el vector RX
				}

				if((b->_estado==at_error)||(b->_estado==at_tcp_desconectado)||(b->_estado==at_tcp_alrdy_cnntd_err))	//Si se cierra la conexión vuelvo a conectar
				{
					ConectarTCP(b);
					b->_estado_conexion=TCP_CONN_EN_CURSO;
					b->_estado=0;
					b->_n_orig=0; //Borro el vector RX
				}
			}
			break;
			case TCP_CONN_OK://Conectar a Servidor TCP OK
			{
				NOP(b);

					if(b->_estado==at_restart)		//Reinicio involuntario de módulo
					{
						Cambiar_CWMODE(b);
						b->_estado_conexion=CAMBIAR_MODO_EN_CURSO;
						b->_estado=0;
						b->_n_orig=0; //Borro el vector RX

						b->_TCP_Local_Server_Initiated=0;	//Indico servidor debe ser iniciado
					}
					if((b->_estado==at_wifi_disconnect)||(b->_estado==at_deconectar_ok )) //Si me desconecto, me vuelvo a conectar
					{
						ConectarWIFI(b);
						b->_estado_conexion=CONEXION_EN_CURSO;
						b->_estado=0;
						b->_n_orig=0; //Borro el vector RX
					}
					if((b->_estado==at_error)||(b->_estado==at_tcp_desconectado))	//Si se cierra la conexión vuelvo a conectar
					{
						ConectarTCP(b);
						b->_estado_conexion=TCP_CONN_EN_CURSO;
						b->_estado=0;
						b->_n_orig=0; //Borro el vector RX2
					}
					if((b->_estado==at_tcp_alrdy_cnntd_err)||(b->_estado==at_tcp_conectado))
					{
						if(b->_automatizacion >= WF_SEND )
						{
							EnviarDatos(b);
							b->_estado_conexion=TCP_SND_EN_CURSO;
							b->_estado=0;
							b->_n_orig=0; //Borro el vector Rx
						}
					}
			}
			break;
			case TCP_SND_EN_CURSO:			//Enviar datos a Servidor TCP
			{
				if((b->_estado!=700)&&(b->_estado!=at_error)
									&&(b->_estado!=at_wifi_disconnect)
									&&(b->_estado!=at_tcp_desconectado)
									&&(b->_estado!=at_tcp_snd_err)
									&&(b->_estado!=at_tcp_ok_to_send)
									&&(b->_estado!=at_tcp_enviado_error))	//Si estoy conectando, no vuelvo a conectar.
				{

					if((b->_automatizacion >= WF_SEND)&&(b->_n_D2SND!=0))  // El envío por este medio es permanente
					{	/*Ensure to have data before sending*/
							EnviarDatos(b);
							/*b->_data2SND[0]=0x00;//strcpy(wf._data2SND,"01;03;00;00;00;0A;C5;CD");//strcpy(wf._data2SND,"20;352;52#");
							b->_data2SND[1]=0x00;
							b->_data2SND[2]=0x00;
							b->_data2SND[3]=0x00;
							b->_data2SND[4]=0x00;
							b->_data2SND[5]=0x06;
							b->_data2SND[6]=0x01;
							b->_data2SND[7]=0x03;
							b->_data2SND[8]=0x00;//strcpy(wf._data2SND,"01;03;00;00;00;0A;C5;CD");//strcpy(wf._data2SND,"20;352;52#");
							b->_data2SND[9]=0x00;
							b->_data2SND[10]=0x00;
							b->_data2SND[11]=0x0A;
							b->_n_D2SND=12;*/
					}
						if(b->_enviaruart==1)
							{
								b->_estado=AT_ESP8266_ND(b);
								b->_enviaruart=0;
								if (b->_DBG_EN==1) ITM0_Write((uint8_t*)b->_uart2snd, b->_n_uart2SND); //210915
								HAL_UART_Transmit(PORTSER,(uint8_t*)b->_uart2snd, b->_n_uart2SND,100);


							}
				}
						if((b->_estado==700)&&(b->_enviaruart==1)
											&&(b->_estado!=at_error)
											&&(b->_estado!=at_wifi_disconnect)
											&&(b->_estado!=at_tcp_desconectado)
											&&(b->_estado!=at_tcp_snd_err)
											&&(b->_estado!=at_tcp_enviado_error))
							{
								b->_enviaruart=0;
								if (b->_DBG_EN==1) ITM0_Write((uint8_t*)b->_uart2snd, b->_n_uart2SND); //210915
								HAL_UART_Transmit(PORTSER,(uint8_t*)b->_uart2snd, b->_n_uart2SND,100);


							}

				//----Condiciones de cambio de estado
				if((b->_estado==at_tcp_enviado_ok)) b->_estado_conexion=TCP_SND_OK;
				if((b->_estado==at_error)||(b->_estado==at_wifi_disconnect)
										 ||(b->_estado==at_tcp_desconectado)
										 ||(b->_estado==at_tcp_snd_err)
										 ||(b->_estado==at_tcp_enviado_error))
					{
					b->_estado_conexion=TCP_SND_ERROR;
					b->_enviaruart==0;		//Si hay error no me arriesgo a que envíe algo
					}
				//----Condiciones de cambio de estado
			}
			break;
			case TCP_SND_ERROR://Error al Enviar datos a Servidor TCP
			{
				NOP(b);
				if(b->_estado==at_restart)		//Reinicio involuntario de módulo
				{
					Cambiar_CWMODE(b);
					b->_estado_conexion=CAMBIAR_MODO_EN_CURSO;
					b->_estado=0;
					b->_n_orig=0; //Borro el vector RX

					b->_TCP_Local_Server_Initiated=0;	//Indico servidor debe ser iniciado
				}
				if((b->_estado==at_wifi_disconnect)||(b->_estado==at_deconectar_ok )) //Si me desconecto, me vuelvo a conectar
				{
					ConectarWIFI(b);
					b->_estado_conexion=CONEXION_EN_CURSO;
					b->_estado=0;
					b->_n_orig=0; //Borro el vector RX
				}

				if((b->_estado==at_wifi_gotip))	//220130 Si se cayó la conexion y vuelve a conectar, que conecte TCP nuevamente
				{
					ConectarTCP(b);
					b->_estado_conexion=TCP_CONN_EN_CURSO;
					b->_estado=0;
					b->_n_orig=0; //Borro el vector RX
				}

				if((b->_estado==at_error)||(b->_estado==at_tcp_desconectado)
										 ||(b->_estado==at_tcp_snd_err)
										 ||(b->_estado==at_tcp_enviado_error))	//Si se cierra la conexión vuelvo a conectar
				{
					ConectarTCP(b);
					b->_estado_conexion=TCP_CONN_EN_CURSO;
					b->_estado=0;
					b->_n_orig=0; //Borro el vector RX
				}
			}
			break;
			case TCP_SND_OK://Enviar datos a Servidor TCP OK
			{
				NOP(b);

					if(b->_estado==at_restart)		//Reinicio involuntario de módulo
					{
						Cambiar_CWMODE(b);
						b->_estado_conexion=CAMBIAR_MODO_EN_CURSO;
						b->_estado=0;
						b->_n_orig=0; //Borro el vector RX

						b->_TCP_Local_Server_Initiated=0;	//Indico servidor debe ser iniciado
					}
					if((b->_estado==at_wifi_disconnect)||(b->_estado==at_deconectar_ok )) //Si me desconecto, me vuelvo a conectar
					{
						ConectarWIFI(b);
						b->_estado_conexion=CONEXION_EN_CURSO;
						b->_estado=0;
						b->_n_orig=0; //Borro el vector RX
					}
					if((b->_estado==at_error)||(b->_estado==at_wifi_connected)||(b->_estado==at_wifi_gotip)||(b->_estado==at_tcp_desconectado))	//Si se cierra la conexión vuelvo a conectar
					{
						ConectarTCP(b);
						b->_estado_conexion=TCP_CONN_EN_CURSO;
						b->_estado=0;
						b->_n_orig=0; //Borro el vector RX
					}
					//EVITO EL REENVÍO PERMANENTE CON CLIENTE TCO, SOLO BAJO DEMANDA

					HAL_Delay(200);
					if((b->_automatizacion >= WF_SEND)&&(b->_n_D2SND!=0))//Check data availability after send process
					{
							EnviarDatos(b);
							b->_estado_conexion=TCP_SND_EN_CURSO;
							b->_estado=0;
							b->_n_orig=0; //Borro el vector RX
					}
					else
					{
							b->_estado_conexion=TCP_CONN_OK;
							b->_estado=0;
							b->_n_orig=0;

					}
			}
			break;
			case TCP_SRVR_EN_CURSO:			//Conectar Servidor TCP local
						{
							if((b->_estado!=800)&&(b->_estado!=at_error)&&(b->_estado!=at_wifi_disconnect)&&(b->_estado!=21)&&(b->_estado!=22)&&(b->_estado!=23))	//Si estoy conectando, no vuelvo a conectar.
							{
								CrearServidorTCP(b);
									if(b->_enviaruart==1)
										{
											b->_estado=AT_ESP8266_ND(b);
											b->_enviaruart=0;
											if (b->_DBG_EN==1) ITM0_Write((uint8_t*)b->_uart2snd, b->_n_uart2SND); //210915
											HAL_UART_Transmit(PORTSER,(uint8_t*)b->_uart2snd, b->_n_uart2SND,100);


										}
							}

							//----Condiciones de cambio de estado
							if((b->_estado==21)||(b->_estado==23)) b->_estado_conexion=TCP_SRVR_OK;
							if((b->_estado==22)) b->_estado_conexion=TCP_SRVR_ERROR;
							//----Condiciones de cambio de estado
						}
						break;
						case TCP_SRVR_ERROR://Error al Conectar Servidor TCP local
						{
							NOP(b);
							if(b->_estado==at_restart)		//Reinicio involuntario de módulo
							{
								Cambiar_CWMODE(b);
								b->_estado_conexion=CAMBIAR_MODO_EN_CURSO;
								b->_estado=0;
								b->_n_orig=0; //Borro el vector RX

								b->_TCP_Local_Server_Initiated=0;	//Indico servidor debe ser iniciado
							}
							if((b->_estado==at_wifi_disconnect)||(b->_estado==at_deconectar_ok )) //Si me desconecto, me vuelvo a conectar
							{
								ConectarWIFI(b);
								b->_estado_conexion=CONEXION_EN_CURSO;
								b->_estado=0;
								b->_n_orig=0; //Borro el vector RX
							}

							if((b->_estado==at_error)||(b->_estado==at_tcp_desconectado))	//Si se cierra la conexión vuelvo a conectar
							{
								ConectarTCP(b);
								b->_estado_conexion=TCP_CONN_EN_CURSO;
								b->_estado=0;
								b->_n_orig=0; //Borro el vector RX
							}
//220818
/*							if(b->_estado==at_tcp_srvr_err)//		//Reinicio involuntario de módulo
							{//
								Cambiar_CWMODE(b);//
								b->_estado_conexion=CAMBIAR_MODO_EN_CURSO;//
								b->_estado=0;//
								b->_n_orig=0;// //Borro el vector RX

								b->_TCP_Local_Server_Initiated=0;//	//Indico servidor debe ser iniciado
							}*/

						}
						break;
						case TCP_SRVR_CLIENT_DSCNN://Error Conectar Servidor TCP local
						{
							NOP(b);
							if(b->_estado==at_restart)		//Reinicio involuntario de módulo
							{
								Cambiar_CWMODE(b);
								b->_estado_conexion=CAMBIAR_MODO_EN_CURSO;
								b->_estado=0;
								b->_n_orig=0; //Borro el vector RX

								b->_TCP_Local_Server_Initiated=0;	//Indico servidor debe ser iniciado
							}
							if((b->_estado==at_wifi_disconnect)||(b->_estado==at_deconectar_ok )) //Si me desconecto, me vuelvo a conectar
							{
								ConectarWIFI(b);
								b->_estado_conexion=CONEXION_EN_CURSO;
								b->_estado=0;
								b->_n_orig=0; //Borro el vector RX
							}

							if((b->_estado==at_error)||(b->_estado==at_tcp_desconectado))	//Si se cierra la conexión vuelvo a conectar
							{
								ConectarTCP(b);
								b->_estado_conexion=TCP_CONN_EN_CURSO;
								b->_estado=0;
								b->_n_orig=0; //Borro el vector RX
							}
						}
						break;
						case TCP_SRVR_CLIENT_RX://Error al Conectar Servidor TCP local
						{
							NOP(b);
							if(b->_estado==at_restart)		//Reinicio involuntario de módulo
							{
								Cambiar_CWMODE(b);
								b->_estado_conexion=CAMBIAR_MODO_EN_CURSO;
								b->_estado=0;
								b->_n_orig=0; //Borro el vector RX

								b->_TCP_Local_Server_Initiated=0;	//Indico servidor debe ser iniciado
							}
							if((b->_estado==at_wifi_disconnect)||(b->_estado==at_deconectar_ok )) //Si me desconecto, me vuelvo a conectar
							{
								ConectarWIFI(b);
								b->_estado_conexion=CONEXION_EN_CURSO;
								b->_estado=0;
								b->_n_orig=0; //Borro el vector RX
							}

							if((b->_estado==at_error)||(b->_estado==at_tcp_desconectado))	//Si se cierra la conexión vuelvo a conectar
							{
								ConectarTCP(b);
								b->_estado_conexion=TCP_CONN_EN_CURSO;
								b->_estado=0;
								b->_n_orig=0; //Borro el vector RX
							}
						}
						break;
						case TCP_SRVR_CLIENT_CNN://Error al Conectar Servidor TCP local
						{
							NOP(b);
							if(b->_estado==at_restart)		//Reinicio involuntario de módulo
							{
								Cambiar_CWMODE(b);
								b->_estado_conexion=CAMBIAR_MODO_EN_CURSO;
								b->_estado=0;
								b->_n_orig=0; //Borro el vector RX

								b->_TCP_Local_Server_Initiated=0;	//Indico servidor debe ser iniciado
							}
							if((b->_estado==at_wifi_disconnect)||(b->_estado==at_deconectar_ok )) //Si me desconecto, me vuelvo a conectar
							{
								ConectarWIFI(b);
								b->_estado_conexion=CONEXION_EN_CURSO;
								b->_estado=0;
								b->_n_orig=0; //Borro el vector RX
							}

							if((b->_estado==at_error)||(b->_estado==at_tcp_desconectado))	//Si se cierra la conexión vuelvo a conectar
							{
								ConectarTCP(b);
								b->_estado_conexion=TCP_CONN_EN_CURSO;
								b->_estado=0;
								b->_n_orig=0; //Borro el vector RX
							}
						}
						break;
						case TCP_SRVR_OK://Conectar Servidor TCP local OK
						{
							NOP(b);
								b->_TCP_Local_Server_Initiated=1;

								if(b->_estado==at_restart)		//Reinicio involuntario de módulo
								{
									Cambiar_CWMODE(b);
									b->_estado_conexion=CAMBIAR_MODO_EN_CURSO;
									b->_estado=0;
									b->_n_orig=0; //Borro el vector RX

									b->_TCP_Local_Server_Initiated=0;	//Indico servidor debe ser iniciado
								}
								if((b->_estado==at_wifi_disconnect)||(b->_estado==at_deconectar_ok )) //Si me desconecto, me vuelvo a conectar
								{
									ConectarWIFI(b);
									b->_estado_conexion=CONEXION_EN_CURSO;
									b->_estado=0;
									b->_n_orig=0; //Borro el vector RX
								}
								if((b->_estado==at_error)||(b->_estado==at_tcp_desconectado))	//Si se cierra la conexión vuelvo a conectar
								{
									ConectarTCP(b);
									b->_estado_conexion=TCP_CONN_EN_CURSO;
									b->_estado=0;
									b->_n_orig=0; //Borro el vector RX
								}
								//Send data when given conditions
								/*if((b->_estado==at_tcp_alrdy_cnntd_err)||(b->_estado==at_tcp_conectado))
								{
									EnviarDatosSRVR(b);
									b->_estado_conexion=TCP_SRVR_TX_EN_CURSO;
									b->_estado=0;
									b->_n_orig=0; //Borro el vector RX
								}*/

								if((b->_send_data==1)&&(b->_n_D2SND!=0)) //Check data availability after send process
								{
									EnviarDatosSRVR(b);
									b->_estado_conexion=TCP_SRVR_TX_EN_CURSO;
									b->_estado=0;
									b->_n_orig=0; //Borro el vector RX
									b->_send_data==0;//b->_new_data_rcv=0; 220219
								}
						}
						break;

						case TCP_SRVR_TX_EN_CURSO:			//Enviar datos desde Servidor TCP
						{
							if((b->_estado!=900)&&(b->_estado!=at_error)&&
							   (b->_estado!=at_wifi_disconnect)&&
							   (b->_estado!=at_wifi_connected)&&
							   (b->_estado!=at_wifi_gotip)&&
							   (b->_estado!=at_tcp_desconectado)&&
							   (b->_estado!=at_tcp_snd_err)&&
							   (b->_estado!=at_tcp_enviado_error)&&
							   (b->_estado!=at_tcp_enviado_ok)&&
							   (b->_estado!=22)&&(b->_estado!=25)&&(b->_n_D2SND!=0))	//Check data availability after send process
								{
								EnviarDatosSRVR(b);
									if(b->_enviaruart==1)
										{
											b->_estado=AT_ESP8266_ND(b);
											b->_enviaruart=0;
											if (b->_DBG_EN==1) ITM0_Write((uint8_t*)b->_uart2snd, b->_n_uart2SND); //210915
											HAL_UART_Transmit(PORTSER,(uint8_t*)b->_uart2snd, b->_n_uart2SND,100);


										}
							}
									if((b->_estado==900)&&(b->_enviaruart==1)&&
									   (b->_estado!=at_error)&&
									   (b->_estado!=at_wifi_disconnect)&&
									   (b->_estado!=at_wifi_connected)&&
									   (b->_estado!=at_wifi_gotip)&&
									   (b->_estado!=at_tcp_desconectado)&&
									   (b->_estado!=at_tcp_snd_err)&&
									   (b->_estado!=at_tcp_enviado_error)&&
									   (b->_estado!=at_tcp_enviado_ok)&&
									   (b->_estado!=22)&&(b->_estado!=25))//210630 Se agrega 11 y 12 //
										{
											b->_enviaruart=0;
											if (b->_DBG_EN==1) ITM0_Write((uint8_t*)b->_uart2snd, b->_n_uart2SND); //210915
											HAL_UART_Transmit(PORTSER,(uint8_t*)b->_uart2snd, b->_n_uart2SND,100);


										}

							//----Condiciones de cambio de estado
							if((b->_estado==at_tcp_enviado_ok)) b->_estado_conexion=TCP_SRVR_TX_OK;

							if((b->_estado==at_error)||(b->_estado==at_restart)||
							   (b->_estado==at_wifi_disconnect)||(b->_estado==at_wifi_connected)||
							   (b->_estado==at_wifi_gotip)||(b->_estado==at_tcp_desconectado)||
							   (b->_estado==at_tcp_snd_err)||(b->_estado==at_tcp_enviado_error)||
							   (b->_estado==22)||(b->_estado==25))//210630 Se agrega ATDecode 4, 11 y 12
								{
								b->_estado_conexion=TCP_SRVR_TX_ERROR;
								b->_enviaruart==0;		//Si hay error no me arriesgo a que envíe algo
								}
							//----Condiciones de cambio de estado
						}
						break;
						case TCP_SRVR_TX_ERROR://Error al Enviar datos a Servidor TCP
						{
							NOP(b);
							if((b->_estado==at_restart)||(b->_estado==at_wifi_connected)||(b->_estado==at_wifi_gotip))	//210630 Si se reconecta a este nivel hay que resetear		//Reinicio involuntario de módulo
							{
								Cambiar_CWMODE(b);
								b->_estado_conexion=CAMBIAR_MODO_EN_CURSO;
								b->_estado=0;
								b->_n_orig=0; //Borro el vector RX

								b->_TCP_Local_Server_Initiated=0;	//Indico servidor debe ser iniciado
							}
							if((b->_estado==at_wifi_disconnect)||(b->_estado==at_deconectar_ok )) //Si me desconecto, me vuelvo a conectar
							{
								ConectarWIFI(b);
								b->_estado_conexion=CONEXION_EN_CURSO;
								b->_estado=0;
								b->_n_orig=0; //Borro el vector RX
							}

							if(b->_estado==22)		//Error con el Servidor TCP
							{
								b->_estado_conexion=TCP_SRVR_ERROR;
								b->_estado=0;
								b->_n_orig=0; //Borro el vector RX
							}

							if(b->_estado==24)		//Se conecta nuevamente el cliente
							{
								b->_estado_conexion=TCP_SRVR_OK;  //Listo para enviar datos
								b->_estado=0;
								b->_n_orig=0; //Borro el vector RX
							}

							if((b->_estado==25)||(b->_estado==at_tcp_snd_err)||(b->_estado=at_tcp_enviado_error))		//Desconexion del cliente o error al enviar vuelvo al proceso de envío
							{
								if(b->_new_data_rcv==1) //Vuelvo a enviar solo cuando recibo nuevos datos
								{
									b->_estado_conexion=TCP_SRVR_TX_EN_CURSO;
									b->_estado=0;
									b->_n_orig=0; //Borro el vector RX
								}

							}


						}
						break;
						case TCP_SRVR_TX_OK://Enviar datos a Servidor TCP OK
						{
							NOP(b);

								if((b->_estado==at_restart)||(b->_estado==at_wifi_connected)||(b->_estado==at_wifi_gotip))	//210630 Si se reconecta a este nivel hay que resetear	//Reinicio involuntario de módulo
								{
									Cambiar_CWMODE(b);
									b->_estado_conexion=CAMBIAR_MODO_EN_CURSO;
									b->_estado=0;
									b->_n_orig=0; //Borro el vector RX

									b->_TCP_Local_Server_Initiated=0;	//Indico servidor debe ser iniciado

								}
								if((b->_estado==at_wifi_disconnect)||(b->_estado==at_deconectar_ok )) //Si me desconecto, me vuelvo a conectar
								{
									ConectarWIFI(b);
									b->_estado_conexion=CONEXION_EN_CURSO;
									b->_estado=0;
									b->_n_orig=0; //Borro el vector RX
								}

								if(((b->_new_data_rcv==1)||(b->_send_data==1))&&(b->_n_D2SND!=0)) // Only send with data
								{
									EnviarDatosSRVR(b);
									b->_estado_conexion=TCP_SRVR_TX_EN_CURSO;
									b->_estado=0;
									b->_n_orig=0; //Borro el vector RX
									b->_new_data_rcv=0;
									b->_send_data=0;
								}

						}
						break;

		}
		return(b->_estado_conexion);
	}
		//--------------Maquina de estados para conectar a TCP

