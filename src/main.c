#include <kernel.h>
#include <drivers/gpio.h>
#include <sys/printk.h>
#include <device.h>

// --- Configuração de LEDs via DeviceTree ---
#define LED_A_NODE DT_ALIAS(led0)  // LED verde
#define LED_B_NODE DT_ALIAS(led2)  // LED vermelho

static const struct gpio_dt_spec ledA = GPIO_DT_SPEC_GET(LED_A_NODE, gpios);
static const struct gpio_dt_spec ledB = GPIO_DT_SPEC_GET(LED_B_NODE, gpios);

// --- Prioridades e tempos ---
#define PRIO_THREAD_A 2   // Maior prioridade (número menor)
#define PRIO_THREAD_B 1   // Menor prioridade
#define TEMPO_A_MS   1000   // Thread A dorme
#define TEMPO_B_MS   1000   // Thread B dorme

// ----------------------------------------------------
// THREAD A — Tarefa curta, alta prioridade
// ----------------------------------------------------
void thread_A(void *p1, void *p2, void *p3)
{
    while (1) {
        uint64_t start_msA, end_msA;

        start_msA = k_uptime_get();
        printk("Thread A iniciou em %llu ms.\n", start_msA);

        gpio_pin_set_dt(&ledA, 1);  // Liga LED verde

        // Simula processamento rápido 
        for (volatile int i = 0; i < 200000; i++) {}

        gpio_pin_set_dt(&ledA, 0);  // Desliga LED verde

        end_msA = k_uptime_get();  // tempo após o delay

        printk("Tempo decorrido (Thread A): %llu ms e terminou em %llu ms.\n", end_msA - start_msA, end_msA);
   
        k_msleep(TEMPO_A_MS);       // Dorme — libera CPU


    }
}

// ----------------------------------------------------
// THREAD B — Tarefa longa, baixa prioridade
// ----------------------------------------------------
void thread_B(void *p1, void *p2, void *p3)
{
    while (1) {
        uint64_t start_msB, end_msB;

        start_msB = k_uptime_get();
        printk("Thread B iniciou em %llu ms.\n", start_msB);

        gpio_pin_set_dt(&ledB, 1);  // Liga LED vermelho

        // Simula processamento mais longo 
        for (volatile int i = 0; i < 10000000; i++) {
            if (i % 100000 == 0) {
                // Aqui não imprimimos nada — apenas ocupamos a CPU
            }
        }

        gpio_pin_set_dt(&ledB, 0);  // Desliga LED vermelho

        end_msB = k_uptime_get();
        printk("Tempo decorrido (Thread B): %llu ms e terminou em %llu ms.\n", end_msB - start_msB, end_msB);

        k_msleep(TEMPO_B_MS);       // Dorme um pouco
    }
}

// ----------------------------------------------------
// Definição das threads
// ----------------------------------------------------
K_THREAD_DEFINE(a_tid, 512, thread_A, NULL, NULL, NULL,
                PRIO_THREAD_A, 0, 0);
K_THREAD_DEFINE(b_tid, 512, thread_B, NULL, NULL, NULL,
                PRIO_THREAD_B, 0, 0);

// ----------------------------------------------------
// Função principal
// ----------------------------------------------------
void main(void)
{
    // Inicializa GPIOs dos LEDs
    if (!device_is_ready(ledA.port) || !device_is_ready(ledB.port)) {
        return;
    }
    gpio_pin_configure_dt(&ledA, GPIO_OUTPUT_INACTIVE);
    gpio_pin_configure_dt(&ledB, GPIO_OUTPUT_INACTIVE);

    printk("=== Demonstração de Preempção com LEDs ===\n");
    printk("Thread A (vermelho): prioridade %d\n", PRIO_THREAD_A);
    printk("Thread B (verde): prioridade %d\n", PRIO_THREAD_B);

    while (1) {
        k_sleep(K_FOREVER); // Main dorme para liberar CPU.
    }
}
