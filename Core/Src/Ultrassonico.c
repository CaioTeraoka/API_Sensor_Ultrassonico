/* DATA: 12/10/2022
 * UNIVERSIDADE FEDERAL DE MINAS GERAIS
 * AUTOR: Caio Teraoka de Menezes Câmara
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


uint32_t Medir_Distancia_CM(void){
	uint32_t time1 = 0;
	uint32_t time2 = 0;
	HAL_GPIO_WritePin(GPIOA, Trigger_Pin, 1);
	Delay_us(10);
	time1 = micros();
	HAL_GPIO_WritePin(GPIOA, Trigger_Pin, 1);
	while(HAL_GPIO_ReadPin(GPIOA, Echo_Pin) != 1){
		time2 = micros();
	};
	return (time1-time2)/58;

} //Vai retornar a distância medida em centimétros


uint32_t Medir_Distancia_INCH(void){
	uint32_t time1 = 0;
	uint32_t time2 = 0;
	HAL_GPIO_WritePin(GPIOA, Trigger_Pin, 1);
	Delay_us(10);
	time1 = micros();
	HAL_GPIO_WritePin(GPIOA, Trigger_Pin, 1);
	while(HAL_GPIO_ReadPin(GPIOA, Echo_Pin) != 1){
		time2 = micros();
	};
	return (time1-time2)/148;

} //Vai retornar a distância em polegadas


void Aproximacao(void){
	uint32_t dist = 0;
	while(1){
		dist = Medir_Distancia_CM();
		HAL_GPIO_WritePin(GPIOA, LED_1_Pin, 1);
		Delay_us(dist*1000);
		HAL_GPIO_WritePin(GPIOA, LED_1_Pin, 0);
	}

} //Irá funcionar como um sensor de ré, o LED irá piscar com uma frequência inversamente proporcional a distância

void Alerta_Distancia(uint32_t dist){
	uint32_t dist_atual = 0;
	while(1){
	dist_atual = Medir_Distancia_CM();
	if (dist_atual < dist) HAL_GPIO_WritePin(GPIOA, LED_2_Pin, 1);
	else HAL_GPIO_WritePin(GPIOA, LED_2_Pin, 0);
	}
} //Recebe um valor de distância como parâmetro e quando o objeto estiver em uma distância menor um led irá acender como alerta

void Delay_us(uint16_t us){
	__HAL_TIM_SET_COUNTER(&htim1, 0);
	while (__HAL_TIM_GET_COUNTER(&htim1) < us);
}

uint32_t micros(void) {

}
