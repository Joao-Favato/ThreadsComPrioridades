# Threads - Prioridade

## Elaboração e Análise dos diagramas temporais

Tendo como base o código disponibilizado no notion, fiz a adição dos printk para observar os intervalos de execução de cada thread. A partir desses dados, elaborei os gráficos para demonstrar o impacto das prioridades na execução do código e também influência dos tempos k_sleep().

### Gráfico 1
<img width="400" height="204" alt="Grafico" src="evidencias\Gráfico 1.png" /> 

No gráfico 1, as duas threads apresentam a mesma prioridade, e nesse caso a Thread A começa. Em seguida, a Thread B começa a ser executada, e logo depois a Thread A começa a termina, formando um espaço no meio do gráfico onde as duas estão no seu tempo de sleep.

### Gráfico 2
<img width="400" height="204" alt="Grafico" src="evidencias\Gráfico 2.png" /> 

No segundo gráfico, a Thread A tem maior prioridade, por isso começa executando. A seguir, a thread B começa a ser executada, mas ele é sempre interrompida para que a de maior prioridade execute quando o tempo do sleep de A acaba.

### Gráfico 3
<img width="400" height="204" alt="Grafico" src="evidencias\Gráfico 3.png" /> 

No gráfico 3, quem tem maior prioridade é B, por isso ele começa executando e não tem nenhuma interrupção. Ao final de sua execução, a thread A entra em ação, mas logo em seguida há um espaço vazio na CPU pois as duas threads estão em sleep. Tal ciclo se repete continuamente.