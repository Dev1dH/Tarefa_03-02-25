//Bibliotecas
#include <stdio.h>           // Biblioteca padrão da linguagem C (implmentar scanf, por exemplo)
#include "pico/stdlib.h"     // Biblioteca padrão da pico sdk   
#include "ws2812.pio.h"      // Função para habilitar a matriz de LEDs  
#include "hardware/pio.h"    // Biblioteca para programar e controlar o PIO
#include "hardware/timer.h"  // Biblioteca para habilitar timer do hardware
#include "number.h"          // Biblioteca para desenhar cada número na matrix de LED
#include "inc/ssd1306.h"     // Biblioteca da ssd1306
#include "inc/font.h"        // Bibioteca para ler os caracteres digitados
#include "hardware/i2c.h"    // Biblioteca para ler o display da i2c

// Define as pinagens
#define NUM_PIXELS 25   // Define o número de pixels da matriz de LEDs   
#define LED_MATRIX 7    // Define a pinagem da matriz de LEDs
#define LED_BLUE 12     // Define a pinagem do LED azul
#define LED_GREEN 11    // Define a pinagem do LED verde
#define BUTTON_A 5      // Define a pinagem do botão A
#define BUTTON_B 6      // Define a pinagem do botão B
#define I2C_PORT i2c1   // Define a porta do i2c       
#define I2C_SDA 14      // Define a pinagem do SDA
#define I2C_SCL 15      // Define a pinagem do SCL
#define endereco 0x3C   // Endereço
#define FLAG false      // Define a flag

bool led_buffer[NUM_PIXELS]; // Buffer de LEDs
ssd1306_t ssd; // Inicializa a estrutura do display

// Protótipo das funções
void init();                                                        // Função para inicializar gpios 
void init_display();                                                // Função para inicializar o display                
void gpio_irq_handler(uint gpio, uint32_t events);                  // Função de interrupção
void copy_array(bool *dest, const bool *src);                       // Função copiar os arrays
void set_one_led(uint8_t r, uint8_t g, uint8_t b);                  // Função para definir a cor dos LEDs na matriz
static inline void put_pixel(uint32_t pixel_grb);                   // Função para enviar um pixel para o buffer
static inline uint32_t urgb_u32(uint8_t r, uint8_t g, uint8_t b);   // Função para converter um pixel para um inteiro
void gpio_irq_handler2(uint gpio, uint32_t events);

// Função principal
int main()
{   
    init(); //inicializa as gpios dos botões, matrix de leds e led rgb
    init_display();

    // Configuração das interrupções dos botões
    gpio_set_irq_enabled_with_callback(BUTTON_A, GPIO_IRQ_EDGE_FALL, true, &gpio_irq_handler);
    gpio_set_irq_enabled_with_callback(BUTTON_B, GPIO_IRQ_EDGE_FALL, true, &gpio_irq_handler);

    bool cor = true;

    while (true) 
    {   
        char c;
        
        if (stdio_usb_connected()) // verifica se o dispositivo usb está conectada
        {
        if(scanf("%c", &c) == 1)
            { // Lê um caracter
            printf("Caractere recebido: '%c'\n", c);
            switch(c)
                {
                case '0': copy_array(led_buffer, matrix_0); break;    
                case '1': copy_array(led_buffer, matrix_1); break;
                case '2': copy_array(led_buffer, matrix_2); break;
                case '3': copy_array(led_buffer, matrix_3); break;
                case '4': copy_array(led_buffer, matrix_4); break;
                case '5': copy_array(led_buffer, matrix_5); break;
                case '6': copy_array(led_buffer, matrix_6); break;
                case '7': copy_array(led_buffer, matrix_7); break;
                case '8': copy_array(led_buffer, matrix_8); break;
                case '9': copy_array(led_buffer, matrix_9); break;
                default:  copy_array(led_buffer, matrix_turn_off); break;
                }
            
            cor = !cor;
            // Atualiza o conteúdo do display com animações
            ssd1306_fill(&ssd, !cor); // Limpa o display
            ssd1306_rect(&ssd, 3, 3, 122, 58, cor, !cor); // Desenha um retângulo
            ssd1306_draw_string(&ssd, &c, 40, 30); // Desenha uma string
            ssd1306_send_data(&ssd); // Atualiza o display
            }       
        }
    }
return 0;
}

// Função para inicializar os LEDs e Botões
void init(){
    
    stdio_init_all();
    PIO pio = pio0;
    int sm = 0;
    uint offset = pio_add_program(pio, &ws2812_program);
        
    // Configuração do pino do LED verde
    gpio_init(LED_GREEN);
    gpio_set_dir(LED_GREEN, GPIO_OUT);
    gpio_put(LED_GREEN, 0); // Inicialmente apagado
    
    // Configuração do pino do LED azul
    gpio_init(LED_BLUE);
    gpio_set_dir(LED_BLUE, GPIO_OUT);
    gpio_put(LED_BLUE, 0); //Inicialmente apagado
    
    // Configuração do pinos do botão A
    gpio_init(BUTTON_A);
    gpio_set_dir(BUTTON_A, GPIO_IN);
    gpio_pull_up(BUTTON_A);

    // Configuração do pinos do botão B
    gpio_init(BUTTON_B);
    gpio_set_dir(BUTTON_B, GPIO_IN);
    gpio_pull_up(BUTTON_B);

    // Inicialização do programa do WS2812
    ws2812_program_init(pio, sm, offset, LED_MATRIX, 800000, FLAG);

}

// Função para inicializar o display I2C
void init_display(){
    
// inicializa o I2C em 400Khz.
i2c_init(I2C_PORT, 400 * 1000);

gpio_set_function(I2C_SDA, GPIO_FUNC_I2C);  // Set the GPIO pin function to I2C
gpio_set_function(I2C_SCL, GPIO_FUNC_I2C);  // Set the GPIO pin function to I2C
gpio_pull_up(I2C_SDA);                      // Pull up the data line
gpio_pull_up(I2C_SCL);                      // Pull up the clock line

ssd1306_init(&ssd, WIDTH, HEIGHT, false, endereco, I2C_PORT);   // Inicializa o display
ssd1306_config(&ssd);                                           // Configura o display
ssd1306_send_data(&ssd);                                        // Envia os dados para o display

// Limpa o display. O display inicia com todos os pixels apagados.
ssd1306_fill(&ssd, false);
ssd1306_send_data(&ssd);

}

// Função de interrupção com debounce
void gpio_irq_handler(uint gpio, uint32_t events)
{
    volatile static uint32_t last_time = 0; // Último tempo que um botão foi pressionado
    volatile uint32_t current_time = to_ms_since_boot(get_absolute_time()); // Tempo atual

    // Verifica se o botão foi pressionado muito rapidamente
    if (current_time - last_time < 400) {
        return;
    }

    last_time = current_time; // Atualiza o tempo do último botão pressionado
        
    if (gpio == BUTTON_A) { // Verifica se o botão A foi pressionado
        gpio_put(LED_GREEN, !gpio_get(LED_GREEN)); // Inverte o estado atual do LED verde
    } else if (gpio == BUTTON_B) { // Verifica se o botão B foi pressionado
        gpio_put(LED_BLUE, !gpio_get(LED_BLUE));  // Inverte o estado atual do LED azul
    }
}

//Função copiar os arrays
void copy_array(bool *dest, const bool *src)
{
    for (int i = 0; i < NUM_PIXELS; i++)
    {
        dest[i] = src[i];
    }

    set_one_led(30,0,0);
}

// Função para enviar um pixel para o buffer
static inline void put_pixel(uint32_t pixel_grb)
{
    pio_sm_put_blocking(pio0, 0, pixel_grb << 8u);
}

// Função para converter um pixel para um inteiro
static inline uint32_t urgb_u32(uint8_t r, uint8_t g, uint8_t b)
{
    return ((uint32_t)(r) << 8) | ((uint32_t)(g) << 16) | (uint32_t)(b);
}

// Função para definir a cor dos LEDs na matriz
void set_one_led(uint8_t r, uint8_t g, uint8_t b)
{
    // Define a cor com base nos parâmetros fornecidos
    uint32_t color = urgb_u32(r, g, b);

    // Define todos os LEDs com a cor especificada
    for (int i = 0; i < NUM_PIXELS; i++)
    {
        if (led_buffer[i])
        {
            put_pixel(color); // Liga o LED com um no buffer
        }
        else
        {
            put_pixel(0);  // Desliga os LEDs com zero no buffer
        }
    }
}
