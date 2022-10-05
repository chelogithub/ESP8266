# ESP8266
 Library to manage ESP8266 as TCP Server/Client

05/10/22
Changed serial debug flag wf._WF_DBG_EN

Pending to detect ETH5100 hang up, after that test HTTP Library

04/10/22

Test performed to avoid sending AT+CIPSEND=0

/**************[ INICIO SysTick ]**************/

// ENVIO DATOS WF ---------------------------------------------------------------//

	if((wf._estado_conexion==609 || wf._estado_conexion==700)&&(wf._TCP_Local_Server_EN==0))  wf_snd_flag_ticks++;

	if(wf_snd_flag_ticks>= 2000 && wf._ejecucion!=1 && wf._TCP_Local_Server_EN==0)		 	  WF_SND_FLAG=1;

// ENVIO DATOS WF ----------------------------------- ---------------------------//


if(wf._ejecucion==1)
	{
		if (FLAG_TIMEOUT!=1)
		{
			if(wf._instruccion!=2) wf._ticks++;//-----------------------Solo cuento una vez reconcido el timeout, cuando entro al timeout no cuento
			if(wf._instruccion==2) wf._ticks2++;
		}


		if ((wf._instruccion!=2)&&(wf._ticks > 5500)) //if (wf._ticks > 5000)
		{
			FLAG_TIMEOUT=1;
			if(huart4.Instance->CR1 == 0x200C)  //--------------------Evito error UART colgado
			{
				HAL_UART_Receive_IT(&huart4,(uint8_t *)UART_RX_byte,1);
				EN_UART2_TMR=0; //OBS-VER Para que me vuelva a habilitar el timer
			}
			//wf._ticks=0;
		}
		if ((wf._instruccion==2)&&(wf._ticks2 > 20500)) //if (wf._ticks > 5000)
		{
			FLAG_TIMEOUT=1;
			if(huart4.Instance->CR1 == 0x200C)  //--------------------Evito error UART colgado
			{
				HAL_UART_Receive_IT(&huart4,(uint8_t *)UART_RX_byte,1);
				EN_UART2_TMR=0; //OBS-VER Para que me vuelva a habilitar el timer
			}
			//wf._ticks=0;
		}

	}
	else
	{
		wf._ticks=0;
	}

/**************[ FIN SysTick ]**************/


/**************[ INICIO PIDO ENVIAR DATOS ]**************/
	  if (ESP_HW_Init==1)
	  {
			if((WF_SND_FLAG==1)&&(wf._TCP_Local_Server_EN==0)&&(wf._estado_conexion>=609))
			{	wf_snd_flag_ticks=0;
				WF_SND_FLAG=0;
				ModBUS_F03_Request(&mb_wf, 0 , 10);
				ModBUS(&mb_wf);							// Create ModBUS info to be sent
				CopiaVector(wf._data2SND,mb_wf._MBUS_2SND,mb_wf._n_MBUS_2SND,0,'A');
				wf._n_D2SND=mb_wf._n_MBUS_2SND;

				if(wf._automatizacion < WF_SEND)		// Send only with automation sent diasabled
				{
					EnviarDatos(&wf);
					wf._estado_conexion=TCP_SND_EN_CURSO;
				}
			}
	  }
/**************[ FIN PIDO ENVIAR DATOS ]**************/



/**************[ INICIO MAQ ESTADO WiFi_Conn_ND ]**************/

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
						b->_n_orig=0; //Borro el vector RX
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

					if(b->_automatizacion >= WF_SEND)  // El envío por este medio es permanente
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
								if (EN_DEBUG==1) ITM0_Write((uint8_t*)b->_uart2snd, b->_n_uart2SND); //210915
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
								if (EN_DEBUG==1) ITM0_Write((uint8_t*)b->_uart2snd, b->_n_uart2SND); //210915
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
					if(b->_automatizacion >= WF_SEND )
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

/**************[ FIN MAQ ESTADO WiFi_Conn_ND ]**************/

03/10/22
- Implemented, send data by demand.
  There is two timeouts, connect to wifi and other instructions like chang mod, connect tcp, etc.
  Timeout counts over instruction requested  _ejecucion flag
  Situation:
  This module has two steps to send data, first step you advise that you'll send data, the module waits for data to be sent. All information recived by the module will be sent.
  So if ypu write AT+CIP . . . it will be sent, to avoid sending wrong data.
  Module reset should be implemented.
  
  turn to fault state. This timeout will destroy performance, but will increase reliability.
