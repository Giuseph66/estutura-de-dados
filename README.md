# Trabalho T2 – Estrutura de Dados

## Tema
Árvore **2-3-4** 

## Academico
* **Giuseph**

## Professor(a)
Ramilton Costa Gomes Junior

---

## Descrição do projeto
Este repositório contém a implementação em C de uma Árvore 2-3-4 solicitada no Trabalho T2 da disciplina de Estrutura de Dados.

O programa permite:
1. Inserir chaves inteiras na árvore (sem duplicatas).
2. Buscar uma chave e indicar se ela está presente.
3. Remover uma chave, mantendo a árvore balanceada.
4. Exibir a estrutura da árvore em formato lateral, facilitando a visualização de ramos esquerdo e direito.
5. Executar um exemplo automático que insere 10 valores aleatórios (0–999), imprimindo a árvore a cada inserção.

---

## Arquivos principais
| Arquivo               | Descrição                                                  |
|-----------------------|------------------------------------------------------------|
| `Arvore_2-3-4.c`        | Implementação completa da árvore 2-3-4 e interface CLI.  |
| `README.md`           | Este documento.                                            |

---

## Como compilar
Requer um compilador C padrão (GCC, Clang ou similar).

```bash
# No Linux/macOS
gcc Arvore_2-3-4.c -o arvore234
```

No Windows, utilize `gcc` do MinGW ou o compilador do Visual Studio.

---

## Como executar
```bash
./arvore234
```
Será apresentado um menu interativo:

```
--- Árvore 2-3-4 ---
1. Inserir chave
2. Buscar chave
3. Remover chave
4. Exibir árvore
5. Exemplo (10 valores aleatórios de 0 a 999)
0. Sair
Escolha:
```

### Exemplo rápido
1. Escolha `1` e digite um valor inteiro para inserir. O programa imprimirá a árvore atualizada.
2. Escolha `4` a qualquer momento para ver a estrutura completa.
3. Use `3` para remover uma chave.
4. Para uma demonstração automática, escolha `5`.

---

## Licença
Uso acadêmico — Trabalho T2 (2025).
