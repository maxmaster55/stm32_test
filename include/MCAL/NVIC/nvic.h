#pragma once
#include "glob.h"


#define NVIC_u32BASE_ADDRESS		(0xE000E100)


typedef struct{
	volatile uint32_t ISER[8];
	uint32_t res0[24];
	volatile uint32_t ICER[8];
	uint32_t res1[24];
	volatile uint32_t ISPR[8];
	uint32_t res2[24];
	volatile uint32_t ICPR[8];
	uint32_t res3[24];
	volatile uint32_t IABR[8];
	uint32_t res4[56];
	volatile uint32_t IPR[240];
	uint32_t res5[644];
	volatile uint32_t STIR;
}NVIC_Reg_t;


#define NVIC ((NVIC_Reg_t *) NVIC_u32BASE_ADDRESS)



typedef enum {
    WWDG_IRQn                = 0,    /*!< Window WatchDog interrupt                                       */
    PVD_IRQn                 = 1,    /*!< EXTI Line 16 interrupt / PVD                                    */
    TAMP_STAMP_IRQn          = 2,    /*!< Tamper + TimeStamp interrupt / EXTI line 21                     */
    RTC_WKUP_IRQn            = 3,    /*!< RTC Wakeup interrupt / EXTI line 22                             */
    FLASH_IRQn               = 4,    /*!< Flash global interrupt                                          */
    RCC_IRQn                 = 5,    /*!< RCC global interrupt                                            */
    EXTI0_IRQn               = 6,    /*!< EXTI Line0 interrupt                                            */
    EXTI1_IRQn               = 7,    /*!< EXTI Line1 interrupt                                            */
    EXTI2_IRQn               = 8,    /*!< EXTI Line2 interrupt                                            */
    EXTI3_IRQn               = 9,    /*!< EXTI Line3 interrupt                                            */
    EXTI4_IRQn               = 10,   /*!< EXTI Line4 interrupt                                            */
    DMA1_Stream0_IRQn        = 11,   /*!< DMA1 Stream0 global interrupt                                   */
    DMA1_Stream1_IRQn        = 12,   /*!< DMA1 Stream1 global interrupt                                   */
    DMA1_Stream2_IRQn        = 13,   /*!< DMA1 Stream2 global interrupt                                   */
    DMA1_Stream3_IRQn        = 14,   /*!< DMA1 Stream3 global interrupt                                   */
    DMA1_Stream4_IRQn        = 15,   /*!< DMA1 Stream4 global interrupt                                   */
    DMA1_Stream5_IRQn        = 16,   /*!< DMA1 Stream5 global interrupt                                   */
    DMA1_Stream6_IRQn        = 17,   /*!< DMA1 Stream6 global interrupt                                   */
    ADC_IRQn                 = 18,   /*!< ADC1 global interrupt                                           */
    EXTI9_5_IRQn             = 23,   /*!< EXTI Line[9:5] interrupts                                       */
    TIM1_BRK_TIM9_IRQn       = 24,   /*!< TIM1 Break & TIM9 global interrupt                              */
    TIM1_UP_TIM10_IRQn       = 25,   /*!< TIM1 Update & TIM10 global interrupt                            */
    TIM1_TRG_COM_TIM11_IRQn  = 26,   /*!< TIM1 Trigger/Commutation & TIM11 global interrupt               */
    TIM1_CC_IRQn             = 27,   /*!< TIM1 Capture Compare interrupt                                  */
    TIM2_IRQn                = 28,   /*!< TIM2 global interrupt                                           */
    TIM3_IRQn                = 29,   /*!< TIM3 global interrupt                                           */
    TIM4_IRQn                = 30,   /*!< TIM4 global interrupt                                           */
    I2C1_EV_IRQn             = 31,   /*!< I2C1 Event interrupt                                            */
    I2C1_ER_IRQn             = 32,   /*!< I2C1 Error interrupt                                            */
    I2C2_EV_IRQn             = 33,   /*!< I2C2 Event interrupt                                            */
    I2C2_ER_IRQn             = 34,   /*!< I2C2 Error interrupt                                            */
    SPI1_IRQn                = 35,   /*!< SPI1 global interrupt                                           */
    SPI2_IRQn                = 36,   /*!< SPI2 global interrupt                                           */
    USART1_IRQn              = 37,   /*!< USART1 global interrupt                                         */
    USART2_IRQn              = 38,   /*!< USART2 global interrupt                                         */
    EXTI15_10_IRQn           = 40,   /*!< EXTI Line[15:10] interrupt                                      */
    RTC_Alarm_IRQn           = 41,   /*!< RTC Alarm (A and B) + EXTI line 17                              */
    OTG_FS_WKUP_IRQn         = 42,   /*!< USB OTG FS Wakeup through EXTI line                             */
    DMA1_Stream7_IRQn        = 47,   /*!< DMA1 Stream7 global interrupt                                   */
    SDIO_IRQn                = 49,   /*!< SDIO global interrupt                                           */
    TIM5_IRQn                = 50,   /*!< TIM5 global interrupt                                           */
    SPI3_IRQn                = 51,   /*!< SPI3 global interrupt                                           */

    DMA2_Stream0_IRQn        = 56,  /*!< DMA2 Stream0 global interrupt                                   */
    DMA2_Stream1_IRQn        = 57,  /*!< DMA2 Stream1 global interrupt                                   */
    DMA2_Stream2_IRQn        = 58,  /*!< DMA2 Stream2 global interrupt                                   */
    DMA2_Stream3_IRQn        = 59,  /*!< DMA2 Stream3 global interrupt                                   */
    DMA2_Stream4_IRQn        = 60,  /*!< DMA2 Stream4 global interrupt                                   */

    OTG_FS_IRQn              = 67,  /*!< USB On-The-Go FS global interrupt                               */

    DMA2_Stream5_IRQn        = 68,  /*!< DMA2 Stream5 global interrupt                                   */
    DMA2_Stream6_IRQn        = 69,  /*!< DMA2 Stream6 global interrupt                                   */
    DMA2_Stream7_IRQn        = 70,  /*!< DMA2 Stream7 global interrupt                                   */

    USART6_IRQn              = 71,  /*!< USART6 global interrupt                                         */

    I2C3_EV_IRQn             = 72,  /*!< I2C3 event interrupt                                            */
    I2C3_ER_IRQn             = 73,  /*!< I2C3 error interrupt                                            */

    FPU_IRQn                 = 81,  /*!< FPU global interrupt                                            */

    SPI4_IRQn                = 84   /*!< SPI4 global interrupt                                           */
} IRQn_Type;


void NVIC_EnableIRQ(IRQn_Type IRQn);
void NVIC_DisableIRQ(IRQn_Type IRQn);
void NVIC_SetPendingIRQ(IRQn_Type IRQn);
void NVIC_ClearPendingIRQ(IRQn_Type IRQn);
uint32_t NVIC_GetPendingIRQ(IRQn_Type IRQn);
void NVIC_SetPriority(IRQn_Type IRQn, uint32_t priority);
uint32_t NVIC_GetPriority(IRQn_Type IRQn);