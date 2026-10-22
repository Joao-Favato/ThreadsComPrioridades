# Threads - Prioridade

## Elaboração e Análise dos diagramas temporais

Tendo como base o código disponibilizado no notion, fiz a adição dos printk para observar os intervalos de execução de cada thread. A partir desses dados, elaborei os gráficos para demonstrar o impacto das prioridades na execução do código e também influência dos tempos k_sleep().

### Gráfico 1
<img width="600" height="304" alt="Grafico" src="evidencias\Gráfico 1.png" /> 

No gráfico 1, as duas threads apresentam a mesma prioridade, e nesse caso a Thread A começa. Em seguida, a Thread B começa a ser executada, e logo depois a Thread A começa a termina, formando um espaço no meio do gráfico onde as duas estão no seu tempo de sleep.

### Gráfico 2
<img width="600" height="304" alt="Grafico" src="evidencias\Gráfico 2.png" /> 

No segundo gráfico, a Thread A tem maior prioridade, por isso começa executando. A seguir, a thread B começa a ser executada, mas ele é sempre interrompida para que a de maior prioridade execute quando o tempo do sleep de A acaba.

### Gráfico 3
<img width="600" height="304" alt="Grafico" src="evidencias\Gráfico 3.png" /> 

No gráfico 3, quem tem maior prioridade é B, por isso ele começa executando e não tem nenhuma interrupção. Ao final de sua execução, a thread A entra em ação, mas logo em seguida há um espaço vazio na CPU pois as duas threads estão em sleep. Tal ciclo se repete continuamente.

## Remoção do k_sleep() nas threads

Ao remover o sleep de uma das threads, ela passou a ser executada constantemente, sem deixar espaço para a outra (a não ser que essa outra tenha prioridade maior). Ao longo dos testes, percebi que, quando removemos o sleep de A, quando a thread B (de prioridade maior) interrompia A, na maioria das vezes a cor ficava misturada, mas em algumas pequenas oportunidades ficava só vermelha, isso porque a thread B estava interrompendo A em momentos diferentes.

## Threads com prioridades iguais

Ao realizar testes de threads com prioridades iguais, verifiquei no meu código que a que era executada primeira era a que tem o nome que vem primeiro na ordem alfabética. Bastou intercambiar os nomes das duas threads no seu define que a cor de início no microcontrolador mudava.

## Threads cooperativas

Verificando o comportamento das threads cooperativas ao longo de testes, atestei que uma thread cooperativa não pode ser interrompida. Em adição a isso, uma thread cooperativa ainda pode interromper uma thread com prioridade maior que 0, já que elas se comportam como a maior prioridade.

## Inanição de Tarefas (Starvation / CPU Starvation)

Como já havia sido verificado durante os testes na parte da remoção do k_sleep(), quando uma thread não tem esse delay de um período inativo, e ainda for a thread com maior prioridade, seu código será rodado continuamente sem dar espaço para outras threads. Tal fenonômeno é conhecido como Inanição de Tarefas (Starvation / CPU Starvation).

<img width="400" height="204" alt="Grafico" src="evidencias\starvation.gif" /> 

## Utilização do botão

A fim de usar o botão para demonstrar o funcionamento das interrupções, primeiramente foram feitos testes para verificar seu funcionamento. A seguir está a evidência através do terminal.

<img width="400" height="204" alt="Grafico" src="evidencias\FuncionamentoBotao.gif" /> 