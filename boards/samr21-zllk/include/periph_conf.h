/*
 * This file is subject to the terms and conditions of the GNU Lesser
 * General Public License v2.1. See the file LICENSE in the top level
 * directory for more details.
 */

/**
 */

#ifndef PERIPH_CONF_H_
#define PERIPH_CONF_H_

#include <stdint.h>

#include "cpu.h"
#include "periph_cpu.h"

#ifdef __cplusplus
extern "C" {
#endif

// added by JK as a hack to get ADC working...
#define ADC_0 0
#define LOCAL_RES   ADC_RES_12BIT

/**
 * @brief   External oscillator and clock configuration
 *
 * For selection of the used CORECLOCK, we have implemented two choices:
 *
 * - usage of the PLL fed by the internal 8MHz oscillator divided by 8
 * - usage of the internal 8MHz oscillator directly, divided by N if needed
 *
 *
 * The PLL option allows for the usage of a wider frequency range and a more
 * stable clock with less jitter. This is why we use this option as default.
 *
 * The target frequency is computed from the PLL multiplier and the PLL divisor.
 * Use the following formula to compute your values:
 *
 * CORECLOCK = ((PLL_MUL + 1) * 1MHz) / PLL_DIV
 *
 * NOTE: The PLL circuit does not run with less than 32MHz while the maximum PLL
 *       frequency is 96MHz. So PLL_MULL must be between 31 and 95!
 *
 *
 * The internal Oscillator used directly can lead to a slightly better power
 * efficiency to the cost of a less stable clock. Use this option when you know
 * what you are doing! The actual core frequency is adjusted as follows:
 *
 * CORECLOCK = 8MHz / DIV
 *
 * NOTE: A core clock frequency below 1MHz is not recommended
 *
 * @{
 */
#define CLOCK_USE_PLL       (1)

#if CLOCK_USE_PLL
/* edit these values to adjust the PLL output frequency */
#define CLOCK_PLL_MUL       (47U)               /* must be >= 31 & <= 95 */
#define CLOCK_PLL_DIV       (1U)                /* adjust to your needs */
/* generate the actual used core clock frequency */
#define CLOCK_CORECLOCK     (((CLOCK_PLL_MUL + 1) * 1000000U) / CLOCK_PLL_DIV)
#else
/* edit this value to your needs */
#define CLOCK_DIV           (1U)
/* generate the actual core clock frequency */
#define CLOCK_CORECLOCK     (8000000 / CLOCK_DIV)
#endif
/** @} */

/**
 * @name Timer peripheral configuration
 * @{
 */
#define TIMER_NUMOF         (2U)
#define TIMER_0_EN          1
#define TIMER_1_EN          1

/* Timer 0 configuration */
#define TIMER_0_DEV         TC3->COUNT16
#define TIMER_0_CHANNELS    2
#define TIMER_0_MAX_VALUE   (0xffff)
#define TIMER_0_ISR         isr_tc3

/* Timer 1 configuration */
#define TIMER_1_DEV         TC4->COUNT32
#define TIMER_1_CHANNELS    2
#define TIMER_1_MAX_VALUE   (0xffffffff)
#define TIMER_1_ISR         isr_tc4
/** @} */

/**
 * @name UART configuration
 * @{
 */
static const uart_conf_t uart_config[] = {
    /* device, RX pin, TX pin, mux */
    {&SERCOM3->USART, GPIO_PIN(PA,27), GPIO_PIN(PA,28), GPIO_MUX_F},
    {&SERCOM5->USART, GPIO_PIN(PB,2), GPIO_PIN(PB,3), GPIO_MUX_D},
//    {&SERCOM5->USART, GPIO_PIN(PA,23), GPIO_PIN(PA,22), GPIO_MUX_D},
};

/* interrupt function name mapping */
#define UART_0_ISR          isr_sercom3
#define UART_1_ISR          isr_sercom5

#define UART_NUMOF          (sizeof(uart_config) / sizeof(uart_config[0]))
/** @} */

/**
 * @name PWM configuration
 * @{
 */
#define PWM_NUMOF           (PWM_0_EN + PWM_1_EN)
#define PWM_0_EN            1
#define PWM_1_EN            1
#define PWM_MAX_CHANNELS    3
/* for compatibility with test application */
#define PWM_0_CHANNELS      PWM_MAX_CHANNELS
#define PWM_1_CHANNELS      PWM_MAX_CHANNELS

/* PWM device configuration */
static const pwm_conf_t pwm_config[] = {
#if PWM_0_EN
    {TCC1, {
        /* GPIO pin, MUX value, TCC channel */
        { GPIO_PIN(PA, 6), GPIO_MUX_E, 0 },
        { GPIO_PIN(PA, 7), GPIO_MUX_E, 1 },
        { GPIO_UNDEF, (gpio_mux_t)0, 2 }
    }},
#endif
#if PWM_1_EN
    {TCC0, {
        /* GPIO pin, MUX value, TCC channel */
        { GPIO_PIN(PA, 16), GPIO_MUX_F, 0 },
        { GPIO_PIN(PA, 18), GPIO_MUX_F, 2 },
        { GPIO_PIN(PA, 19), GPIO_MUX_F, 3 }
    }}
#endif
};

/** @} */

/**
 * @name SPI configuration
 * @{
 */
#define SPI_NUMOF          (2)
#define SPI_0_EN           1
#define SPI_1_EN           1

/*      SPI0             */
#define SPI_0_DEV          SERCOM4->SPI
#define SPI_IRQ_0          SERCOM4_IRQn
#define SPI_0_DOPO         (1)
#define SPI_0_DIPO         (0)

#define SPI_0_SCLK_DEV     PORT->Group[2]
#define SPI_0_SCLK_PIN     (18)

#define SPI_0_MISO_DEV     PORT->Group[2]
#define SPI_0_MISO_PIN     (19)

#define SPI_0_MOSI_DEV     PORT->Group[1]
#define SPI_0_MOSI_PIN     (30)

/*      SPI1             */
#define SPI_1_DEV          SERCOM5->SPI
#define SPI_IRQ_1          SERCOM5_IRQn
#define SPI_1_DOPO         (0)
#define SPI_1_DIPO         (3)

#define SPI_1_SCLK_DEV     PORT->Group[1]
#define SPI_1_SCLK_PIN     (03)

#define SPI_1_MISO_DEV     PORT->Group[1]
#define SPI_1_MISO_PIN     (22)

#define SPI_1_MOSI_DEV     PORT->Group[1]
#define SPI_1_MOSI_PIN     (02)
/** @} */

/**
 * @name I2C configuration
 * @{
 */
#define I2C_NUMOF          (1U)
#define I2C_0_EN            1
#define I2C_1_EN            0
#define I2C_2_EN            0
#define I2C_3_EN            0
#define I2C_IRQ_PRIO        1

#define I2C_0_DEV           SERCOM1->I2CM
#define I2C_0_IRQ           SERCOM1_IRQn
#define I2C_0_ISR           isr_sercom1
/* I2C 0 pin configuration */
#define I2C_0_PORT          (PORT->Group[0])
#define I2C_SDA             PIN_PA16
#define I2C_SCL             PIN_PA17
#define I2C_0_PINS          (PORT_PA16 | PORT_PA17)

/**
 * @name RTC configuration
 * @{
 */
#define RTC_NUMOF           (1U)
#define RTC_DEV             RTC->MODE2
/** @} */

/**
 * @name RTT configuration
 * @{
 */
#define RTT_NUMOF           (1U)
#define RTT_DEV             RTC->MODE0
#define RTT_IRQ             RTC_IRQn
#define RTT_IRQ_PRIO        10
#define RTT_ISR             isr_rtc
#define RTT_MAX_VALUE       (0xffffffff)
#define RTT_FREQUENCY       (32768U)    /* in Hz. For changes see `rtt.c` */
#define RTT_RUNSTDBY        (1)         /* Keep RTT running in sleep states */
/** @} */

/** @} */

/**
 * @ ADC Configuration
 * @{
 */
typedef struct {
    PortGroup *port;
    uint8_t pin;
    uint8_t muxpos;                 /* see datasheet sec 30.8.8 */
} adc_conf_t;


// added by JK to get ADC working
// -------------------------------------
#define ADC_NUMOF           (1)
typedef enum {
    SAMR21_ADC_RES_8BIT,           /**< ADC resolution: 8 bit */
    SAMR21_ADC_RES_10BIT,          /**< ADC resolution: 10 bit */
    SAMR21_ADC_RES_12BIT,          /**< ADC resolution: 12 bit */
    SAMR21_ADC_RES_16BIT,          /**< ADC resolution: 16 bit */
} samr21_adc_res_t;
typedef unsigned int samr21_adc_t;

int adc_sample_local(samr21_adc_t dev, int channel);
int adc_init_local(samr21_adc_t dev, samr21_adc_res_t precision);
// -------------------------------------

#define ADC_0_EN                           1
#define ADC_MAX_CHANNELS                   8

/* ADC 0 device configuration */
#define ADC_0_DEV                          ADC
#define ADC_0_PORT                         (PORT->Group[0])
#define ADC_0_IRQ                          ADC_IRQn
#define ADC_0_CHANNELS                     8

/* ADC 0 Default values */
#define ADC_0_CLK_SOURCE                   0 /* GCLK_GENERATOR_0 */
#define ADC_0_PRESCALER                    ADC_CTRLB_PRESCALER_DIV512
#define ADC_0_WINDOW_MODE                  ADC_WINCTRL_WINMODE_DISABLE
#define ADC_0_WINDOW_LOWER                 0
#define ADC_0_WINDOW_HIGHER                0

#define ADC_0_CORRECTION_EN                0 /* enabled */
//#define ADC_0_GAIN_CORRECTION              3388 /* 2048 -> 1; 4095/2475 -> 3388 */
#define ADC_0_GAIN_CORRECTION              100
//#define ADC_0_OFFSET_CORRECTION            25 /* 24 by determined by testing */
#define ADC_0_OFFSET_CORRECTION            0 
#define ADC_0_SAMPLE_LENGTH                0
#define ADC_0_PIN_SCAN_OFFSET_START        0 /* disabled */
#define ADC_0_PIN_SCAN_INPUT_TO_SCAN       0 /* disabled */
#define ADC_0_LEFT_ADJUST                  0 /* disabled */
#define ADC_0_DIFFERENTIAL_MODE            0 /* disabled */
#define ADC_0_FREE_RUNNING                 0 /* disabled */
#define ADC_0_EVENT_ACTION                 0 /* disabled */
#define ADC_0_RUN_IN_STANDBY               0 /* disabled */

/* ADC 0 Module Status flags */
#define ADC_0_STATUS_RESULT_READY          (1UL << 0)
#define ADC_0_STATUS_WINDOW                (1UL << 1)
#define ADC_0_STATUS_OVERRUN               (1UL << 2)

#if ADC_0_CHANNELS
#if ADC_0_DIFFERENTIAL_MODE
/*  In differential mode, we can only pick one positive
 *  and one negative input.
 */
#define ADC_0_POS_INPUT                    ADC_INPUTCTRL_MUXPOS_PIN6
#define ADC_0_NEG_INPUT                    ADC_INPUTCTRL_MUXNEG_PIN7
#else
/*  If we're not in differential mode, we can use any of
 *  ADC positive input pins the SAMR21-XPRO provides.  Each pin is
 *  a separate ADC "channel".
 */
static const adc_conf_t adc_config[] = {
    /* port, pin, muxpos */
    {&PORT->Group[0], 4, 0x4}, // we cannot use these as well as use EDBG serial TX/RX
    {&PORT->Group[0], 5, 0x5}, // because PA04 & PA05 read/write directly to EDBG serial port.
    {&PORT->Group[0], ADC_INPUTCTRL_MUXPOS_PIN6, 0x6},    // PA06
    {&PORT->Group[0], ADC_INPUTCTRL_MUXPOS_PIN7, 0x7},    // PA07
    {&PORT->Group[0], ADC_INPUTCTRL_MUXPOS_PIN16, 0x10},   // PA08
    {&PORT->Group[0], ADC_INPUTCTRL_MUXPOS_PIN17, 0x11},   // PA09
    {&PORT->Group[0], ADC_INPUTCTRL_MUXPOS_PIN10, 0xA},    // PB02
    {&PORT->Group[0], ADC_INPUTCTRL_MUXPOS_PIN11, 0xB},    // PB03
};
#endif
#endif


/* ADC 0 Gain Factor */
#define ADC_0_GAIN_FACTOR_1X               ADC_INPUTCTRL_GAIN_1X
#define ADC_0_GAIN_FACTOR_2X               ADC_INPUTCTRL_GAIN_2X
#define ADC_0_GAIN_FACTOR_4X               ADC_INPUTCTRL_GAIN_4X
#define ADC_0_GAIN_FACTOR_8X               ADC_INPUTCTRL_GAIN_8X
#define ADC_0_GAIN_FACTOR_16X              ADC_INPUTCTRL_GAIN_16X
/* Use this to define the value used */
#define ADC_0_GAIN_FACTOR_DEFAULT          ADC_0_GAIN_FACTOR_1X

/* ADC 0 Resolutions */
#define ADC_0_RES_8BIT                     ADC_CTRLB_RESSEL_8BIT
#define ADC_0_RES_10BIT                    ADC_CTRLB_RESSEL_10BIT
#define ADC_0_RES_12BIT                    ADC_CTRLB_RESSEL_12BIT
#define ADC_0_RES_16BIT                    ADC_CTRLB_RESSEL_16BIT

/* ADC 0 Voltage reference */
#define ADC_0_REF_INT_1V                   ADC_REFCTRL_REFSEL_INT1V
#define ADC_0_REF_INT_VCC0                 ADC_REFCTRL_REFSEL_INTVCC0
#define ADC_0_REF_INT_VCC1                 ADC_REFCTRL_REFSEL_INTVCC1
#define ADC_0_REF_EXT_B                    ADC_REFCTRL_REFSEL_AREFB
#define ADC_0_REF_COM_EN                   1
/* Use this to define the value used */
//#define ADC_0_REF_DEFAULT                  ADC_REFCTRL_REFSEL_AREFB//ADC_0_REF_INT_1V
#define ADC_0_REF_DEFAULT                  ADC_REFCTRL_REFSEL_INTVCC0

/* ADC 0 ACCUMULATE */
#define ADC_0_ACCUM_DISABLE                ADC_AVGCTRL_SAMPLENUM_1
#define ADC_0_ACCUM_2                      ADC_AVGCTRL_SAMPLENUM_2
#define ADC_0_ACCUM_4                      ADC_AVGCTRL_SAMPLENUM_4
#define ADC_0_ACCUM_8                      ADC_AVGCTRL_SAMPLENUM_8
#define ADC_0_ACCUM_16                     ADC_AVGCTRL_SAMPLENUM_16
#define ADC_0_ACCUM_32                     ADC_AVGCTRL_SAMPLENUM_32
#define ADC_0_ACCUM_64                     ADC_AVGCTRL_SAMPLENUM_64
#define ADC_0_ACCUM_128                    ADC_AVGCTRL_SAMPLENUM_128
#define ADC_0_ACCUM_256                    ADC_AVGCTRL_SAMPLENUM_256
#define ADC_0_ACCUM_512                    ADC_AVGCTRL_SAMPLENUM_512
#define ADC_0_ACCUM_1024                   ADC_AVGCTRL_SAMPLENUM_1024
/* Use this to define the value used */
#define ADC_0_ACCUM_DEFAULT                ADC_0_ACCUM_DISABLE

/* ADC 0 DIVIDE RESULT */
#define ADC_0_DIV_RES_DISABLE              0
#define ADC_0_DIV_RES_2                    1
#define ADC_0_DIV_RES_4                    2
#define ADC_0_DIV_RES_8                    3
#define ADC_0_DIV_RES_16                   4
#define ADC_0_DIV_RES_32                   5
#define ADC_0_DIV_RES_64                   6
#define ADC_0_DIV_RES_128                  7
/* Use this to define the value used */
#define ADC_0_DIV_RES_DEFAULT             ADC_0_DIV_RES_DISABLE
/** @} */

#ifdef __cplusplus
}
#endif

#endif /* PERIPH_CONF_H_ */
/** @} */
