/* DATA: 12/10/2022
 * UNIVERSIDADE FEDERAL DE MINAS GERAIS
 * AUTOR: Caio Teraoka de Menezes Câmara e Cristovão Eustaquio da Silva
 * VERSÃO 1.0
 * API: Ultrassonico
 * DESCRIÇÃO: API desenvolvida para uso do módulo Sensor Ultrassônico HC-SR04. Suas funções contam com
 * REQUISITOS de HARDWARE: Módulo Sensor Ultrassônico HC-SR04, Microcontrolador STM32F103RBT6, LEDs, Resistores de 220 Ohms e Jumpers
 * REQUISITOS DE SOFTWARE: STM32CubeIDE
 *
 * Esta API foi desenvolvida como trabalho da disciplina de Programação de Sistemas Embarcados da UFMG –Prof. Ricardo de Oliveira Duarte –Departamento de Engenharia Eletrônica
 */

#include "main.h"
#include "Ultrassonico.h"


extern TIM_HandleTypeDef htim1;
extern TIM_HandleTypeDef htim3;
int tempo = 0;
uint32_t Time1 = 0;
uint32_t Time2 = 0;
uint32_t Diferenca = 0;
uint8_t Primeira_Captura = 0;
uint32_t Distancia  = 0;

uint32_t Medir_Distancia_CM(void){
	HAL_GPIO_WritePin(GPIOA, Trigger_Pin, 1);
	for(int x = 0; x < 40; ++x){}
	HAL_GPIO_WritePin(GPIOA, Trigger_Pin, 0);
	__HAL_TIM_ENABLE_IT(&htim1, TIM_IT_CC1);
	return Distancia;

} //Vai retornar a distância medida em centimétros


uint32_t Medir_Distancia_INCH(void){
	HAL_GPIO_WritePin(GPIOA, Trigger_Pin, 1);
	for(int x = 0; x < 40; ++x){}
	HAL_GPIO_WritePin(GPIOA, Trigger_Pin, 0);
	__HAL_TIM_ENABLE_IT(&htim1, TIM_IT_CC1);
	return Distancia/2.54;

} //Vai retornar a distância em polegadas


void Aproximacao(void){
	uint32_t dist = 0;
	while(1){
		dist = Medir_Distancia_CM();
		HAL_GPIO_WritePin(GPIOA, LED_1_Pin, 1);
		for(int x = 0; x < dist*100000; ++x){}
		dist = Medir_Distancia_CM();
		HAL_GPIO_WritePin(GPIOA, LED_1_Pin, 0);
		for(int x = 0; x < dist*100000; ++x){}
	}

} //Irá funcionar como um sensor de ré, o LED irá piscar com uma frequência inversamente proporcional a distância

void Alerta_Distancia(uint32_t dist){
	uint32_t dist_atual = 0;
	while(1){
	dist_atual = Medir_Distancia_CM();
	if (dist_atual < dist) HAL_GPIO_WritePin(GPIOA, LED_1_Pin, 1);
	else HAL_GPIO_WritePin(GPIOA, LED_1_Pin, 0);
	for(int x = 0; x < 240000; ++x){}
	}
} //Recebe um valor de distância como parâmetro e quando o objeto estiver em uma distância menor um led irá acender como alerta

/*
void delay_us(uint16_t us){
	__HAL_TIM_SET_COUNTER(&htim1, 0);
	while (__HAL_TIM_GET_COUNTER(&htim1) < us);
}
*/
void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim)
{
	if (htim->Channel == HAL_TIM_ACTIVE_CHANNEL_1)
	{
		if (Primeira_Captura==0)
		{
			Time1 = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_1);
			Primeira_Captura = 1;


			__HAL_TIM_SET_CAPTUREPOLARITY(htim, TIM_CHANNEL_1, TIM_INPUTCHANNELPOLARITY_FALLING);
		}

		else if (Primeira_Captura==1)
		{
			Time2 = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_1);
			__HAL_TIM_SET_COUNTER(htim, 0);

			if (Time2 > Time1)
			{
				Diferenca = Time2-Time1;
			}

			else if (Time1 > Time2)
			{
				Diferenca = (0xffff - Time1) + Time2;
			}

			Distancia = Diferenca * 0.034/2;
			Primeira_Captura = 0;


			__HAL_TIM_SET_CAPTUREPOLARITY(htim, TIM_CHANNEL_1, TIM_INPUTCHANNELPOLARITY_RISING);
			__HAL_TIM_DISABLE_IT(&htim1, TIM_IT_CC1);
		}
	}
}
