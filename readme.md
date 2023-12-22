# Conversor AFNe para mínimo AFD

Esse programa foi escrito em C++, para o trabalho da disciplina de Compiladores 1. Nele, é feito a conversão de um AFN (Autômato Finito Não Determinístico) para o mínimo AFD (Autômato Finito Determinístico), retirando as transições em épsilon (vazias) utilizando o algorítimo de Thompson.

Pré-Requisitos:

  - C++

Compilar, pasta src execute o comando:
```
  g++ conversor.cpp -o C
  ./C
```
## Entrada

Adicione os inputs pedidos na ordem pedida:
```
  Quantidade de Estados:
  Quantidade de Estados Finais:
  Quantidade de Transições:
  Quantidade de Símbolos no Alfabeto:
  Alfabeto:
  Estado inicial:
  Estado Final:
  Outros estados:
  Transições:
  E.g: q0,q1,a
```
Importante ressaltar que na parte de transições, o primeiro estado é o estado que está saindo a transição, e o último é para onde ele está indo. O símbolo no final é o símbolo do alfabeto que ele transita, podendo adicionar somente um por vez.

Portanto, se você quiser escrever que o estado 'q0' transita para 'q1' em 'a', deve ser escrito como "q0,q1,a".

## Saída

  A saída é composta por 4 etapas.

  - Primeira: Retorna o valor recebido para verificação
  - Segunda: Retorna as execuções do algorítimo de Thompson, E.g: "q1,q6,e <<< q1,q8::q8,q6". No exemplo apresentado, a transição "q1,q6,e" foi criada a partir da união de "q1,q8,e" e "q8,q6,e". Depois disso o programa retorna o resultado da redução de Thompson.
  - Terceira: Faz a tabela de transições do autômato da etapa anterior, e se necessário faz a conversão de AFN para AFD. Depois imprime o autômato resultante.
  - Quarta: Faz a minimização do AFD resultante da etapa anterior, e imprime o resultado final.
