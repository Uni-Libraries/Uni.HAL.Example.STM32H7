//
// Includes
//

// ox app
#include "config/app_config_adc.h"

#include "app_config_telemetry.h"


//
// Global
//


volatile uint16_t __attribute__((section(".AdcSection"))) adc2_data[UNI_HAL_ADC_CHANNELS_MAX] = {0};
volatile uint16_t __attribute__((section(".AdcSection"))) adc3_data[UNI_HAL_ADC_CHANNELS_MAX] = {0};


uni_hal_dma_context_t g_ox_adc_2_dma_ctx = {
        .config = {
                .instance = UNI_HAL_CORE_PERIPH_DMA_1,
                .channel = UNI_HAL_DMA_CHANNEL_1,
        }
};


uni_hal_dma_context_t g_ox_adc_3_dma_ctx = {
        .config = {
                .instance = UNI_HAL_CORE_PERIPH_DMA_1,
                .channel = UNI_HAL_DMA_CHANNEL_2,
        }
};

uni_hal_gpio_pin_context_t g_ox_adc_2_pin_ctx = {
        .gpio_bank = UNI_HAL_CORE_PERIPH_GPIO_F,
        .gpio_pin = UNI_HAL_GPIO_PIN_14,
        .gpio_type = UNI_HAL_GPIO_TYPE_ANALOG,
};

uni_hal_adc_context_t g_ox_adc_2_ctx = {
        .config = {
                .instance = UNI_HAL_CORE_PERIPH_ADC_2,
                .channels_count = 1,
                .channels[0] = 6,
                .pins[0] = &g_ox_adc_2_pin_ctx,
                .dma = &g_ox_adc_2_dma_ctx,
                .data = adc2_data,
                .v_ref = 3300,
        }
};

uni_hal_adc_context_t g_ox_adc_3_ctx = {
        .config = {
                .instance = UNI_HAL_CORE_PERIPH_ADC_3,
                .channels_count = 2,
#if defined(STM32H7)
                .channels[0] = UNI_HAL_ADC_STM32H7_CHANNEL_REFINT,
                .channels[1] = UNI_HAL_ADC_STM32H7_CHANNEL_TEMPSENSOR,
#endif
                .dma = &g_ox_adc_3_dma_ctx,
                .data = adc3_data,
                .v_ref = 3300,
        }
};

ox_adc_context_t g_ox_adc_ctx = {
        .config = {
                .adc_ext = &g_ox_adc_2_ctx,
                .adc_int = &g_ox_adc_3_ctx,
                .telemetry = &g_ox_telemetry_ctx,
                .resistor_up = 20000U,
                .resistor_down = 10000U,
                .vdda_update_interval = 1000U,
        }
};
