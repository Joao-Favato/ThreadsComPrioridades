#include <kernel.h>
#include <drivers/gpio.h>
#include <logging/log.h>
#include <device.h>

LOG_MODULE_REGISTER(threads_prioridade, LOG_LEVEL_INF);

// --- Configuração de LEDs via DeviceTree ---
#define LED_A_NODE DT_ALIAS(led0)  // LED verde
#define LED_B_NODE DT_ALIAS(led2)  // LED vermelho
#define LED_C_NODE DT_ALIAS(led1)  // LED azul 
#define BUTTON_NODE DT_NODELABEL(user_button_0)

static const struct gpio_dt_spec button = GPIO_DT_SPEC_GET(BUTTON_NODE, gpios);
static struct gpio_callback button_cb_data;
static const struct gpio_dt_spec ledA = GPIO_DT_SPEC_GET(LED_A_NODE, gpios);
static const struct gpio_dt_spec ledB = GPIO_DT_SPEC_GET(LED_B_NODE, gpios);
static const struct gpio_dt_spec ledC = GPIO_DT_SPEC_GET(LED_C_NODE, gpios);
static int64_t last_time = 0;
// --- Prioridades e tempos ---
#define PRIO_THREAD_A  1   // Maior prioridade (número menor)
#define PRIO_THREAD_B  2   // Menor prioridade
#define TEMPO_A_MS   1000   // Thread A dorme
#define TEMPO_B_MS   1000   // Thread B dorme

void thread_A(void *p1, void *p2, void *p3)
{
    while (1) {
        uint32_t start_msA, end_msA;

        start_msA = k_uptime_get();
        LOG_INF("Thread A iniciou em %u ms.", (unsigned long long)start_msA);

        gpio_pin_set_dt(&ledA, 1);  // Liga LED verde

        // Simula processamento rápido 
        for (volatile int i = 0; i < 200000; i++) {}

        gpio_pin_set_dt(&ledA, 0);  // Desliga LED verde

        end_msA = k_uptime_get();  // tempo após o delay

        LOG_INF("Tempo decorrido (Thread A): %u ms e terminou em %u ms.",
                (unsigned long long)(end_msA - start_msA),
                (unsigned long long)end_msA);
        
         k_msleep(TEMPO_A_MS); 
    }
}


void thread_B(void *p1, void *p2, void *p3)
{
    while (1) {
        uint32_t start_msB, end_msB;

        start_msB = k_uptime_get();
        LOG_INF("Thread B iniciou em %u ms.", (unsigned long long)start_msB);

        gpio_pin_set_dt(&ledB, 1);  // Liga LED vermelho

        // Simula processamento mais longo 
        for (volatile int i = 0; i < 10000000; i++) {
            if (i % 100000 == 0) {
                // Aqui não imprimimos nada — apenas ocupamos a CPU
            }
        }

        gpio_pin_set_dt(&ledB, 0);  // Desliga LED vermelho

        end_msB = k_uptime_get();
        LOG_INF("Tempo decorrido (Thread B): %u ms e terminou em %u ms.",
                (unsigned long long)(end_msB - start_msB),
                (unsigned long long)end_msB);

        k_msleep(TEMPO_B_MS);       // Dorme um pouco
    }
}

void button_isr(const struct device *dev, struct gpio_callback *cb, uint32_t pins)
{
    int64_t now = k_uptime_get();

    if ((now - last_time) > 200) {
        gpio_pin_toggle_dt(&ledC);
        last_time = now;
        LOG_INF("Interrupcao do botao: LED azul alternado em %lld ms.",
                (long long)now);
    }
}


// ----------------------------------------------------
// Definição das threads
// ----------------------------------------------------
K_THREAD_DEFINE(a_tid, 1024, thread_A, NULL, NULL, NULL,
                PRIO_THREAD_A, 0, 0);
K_THREAD_DEFINE(b_tid, 1024, thread_B, NULL, NULL, NULL,
                PRIO_THREAD_B, 0, 0);

// ----------------------------------------------------
// Função principal//
// ----------------------------------------------------
void main(void)
{
    // Inicializa GPIOs dos LEDs
    if (!device_is_ready(ledA.port) || !device_is_ready(ledB.port) || !device_is_ready(ledC.port)) {
        return;
    }

    gpio_pin_configure_dt(&ledA, GPIO_OUTPUT_INACTIVE);
    gpio_pin_configure_dt(&ledB, GPIO_OUTPUT_INACTIVE);
    gpio_pin_configure_dt(&ledC, GPIO_OUTPUT_INACTIVE);

    gpio_pin_configure_dt(&button, GPIO_INPUT | GPIO_PULL_UP);
    gpio_init_callback(&button_cb_data, button_isr, BIT(button.pin));
    gpio_add_callback(button.port, &button_cb_data);
    // Habilita a interrupcao depois que o callback ja esta registrado.
    gpio_pin_interrupt_configure_dt(&button, GPIO_INT_EDGE_FALLING);

    LOG_INF("=== Demonstração de Preempção com LEDs ===");
    LOG_INF("Thread A (verde): prioridade %d, tempo de suspensão: %d ms", PRIO_THREAD_A, TEMPO_A_MS);
    LOG_INF("Thread B (vermelho): prioridade %d, tempo de suspensão: %d ms", PRIO_THREAD_B, TEMPO_B_MS);

    while (1) {
        k_sleep(K_FOREVER); // Main dorme para liberar CPU.
    }
}
    
