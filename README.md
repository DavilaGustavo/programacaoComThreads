<p align="center">
  <img src="https://github.com/user-attachments/assets/7badae08-46ec-4e70-88e7-71c2667d498e">
</p>

# Sistema de Simulação de Reserva de Passagens com Paralelismo
Simulação de um sistema de reserva de passagens para ônibus utilizando programação multithread em C++. O projeto implementa duas versões: uma com problemas de sincronização para demonstração e outra com as devidas correções.

## Funcionalidades
- Simula passageiros tentando reservar assentos de um ônibus
- Sistemas aleatórios para escolha de assentos e descanso
- Sistema de espera e notificação entre passageiros e ônibus
- Variáveis configuráveis para possíveis testes

## Detalhes Técnicos
Foi implementado duas versões do código bastante semelhantes, na qual o `mainCorrigido.cpp` representa o código no seu estado completo e finalizado, representando um sistema eficiente de organização dos passageiros no ônibus utilizando o paralelismo de forma correta.
A segunda versão, `mainProblema.cpp` mostra possíveis problemas ao utilizar o multithread de forma incorreta, resultando em problemas de sincronização, inconsistências e deadlocks.

#### Problemas corrigidos na versão final
- Implementado mutex para controle de acesso
- Utilizado condition variables para sincronização
- Foi previnido race conditions e deadlocks
- Controle adequado de estados do sistema

### Compilação e Execução
```bash
# Compilar versão com problemas
g++ mainProblema.cpp -pthread -o problema

# Compilar versão corrigida
g++ mainCorrigido.cpp -pthread -o corrigido

# Executar
./problema
./corrigido
```

### Dependências
- C++11 ou superior
- pthread library
- STL (Standard Template Library)
