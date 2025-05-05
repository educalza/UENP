import json

def main():
    # Caminho do arquivo JSON com a configuração da máquina
    ler_arquivojson = r"C:\Códigos vscodes\Teoria da Computação\Maquina de Turing Universal\duplo_bal.json"

    try:
        # Tenta abrir o arquivo JSON
        with open(ler_arquivojson, 'r') as arquivoJson:
            # Carrega os dados do arquivo JSON
            arquivoJSON = json.load(arquivoJson)
    
    except FileNotFoundError:
        print("Erro: Arquivo JSON não encontrado.")
        return
    
    # Caminho do arquivo de entrada
    ler_arquivotxt = r"C:\Códigos vscodes\Teoria da Computação\Maquina de Turing Universal\entrada3.txt"
    
    try:
        # Tenta abrir o arquivo de entrada
        with open(ler_arquivotxt, 'r') as arquivoTxt:
            # Lê todas as linhas do arquivo
            arquivoTXT = arquivoTxt.readlines()
    except FileNotFoundError:
        print("Erro: Arquivo de entrada TXT não encontrado.")
        return
    
    # Carrega dados da máquina de Turing
    transitions = arquivoJSON['transitions']
    dicionario_transitions = {(t['from'], t['read']): t for t in transitions}  # Dicionário para busca rápida das transições
    blank_simbol = arquivoJSON['white']
    final_state = arquivoJSON['final']
    initial_state = arquivoJSON['initial']

    with open("saida.txt", "w") as arquivo_saida:
        # Processa cada linha de entrada
        for linha in arquivoTXT:
            fita = list(linha.strip())
            estado_atual = initial_state  # Estado inicial
            posicao_cabecote = 0  # Posição inicial do cabeçote
            
            # Execução da máquina de Turing
            while estado_atual not in final_state:
                chave_transicao = (estado_atual, fita[posicao_cabecote])
                
                # Verifica se existe transição para a configuração atual
                if chave_transicao in dicionario_transitions:
                    transicao = dicionario_transitions[chave_transicao]
                    fita[posicao_cabecote] = transicao['write']
                    estado_atual = transicao['to']
                    
                    # Define a direção do movimento do cabeçote
                    if transicao['dir'] == 'R':
                        posicao_cabecote += 1
                    elif transicao['dir'] == 'L':
                        posicao_cabecote -= 1
                else:
                    break  # Se não houver transição, para a execução
            
                # Expande a fita se o cabeçote sair dos limites
                if posicao_cabecote < 0:
                    fita.insert(0, blank_simbol)
                    posicao_cabecote = 0
                elif posicao_cabecote >= len(fita):
                    fita.append(blank_simbol)

            # Escreve a fita final no arquivo de saída
            arquivo_saida.write(''.join(fita) + '\n')
            
            # Imprime se o estado final foi de aceitação ou rejeição
            if estado_atual in final_state:
                print(1)  # Aceita
            else:
                print(0)  # Rejeita

if __name__ == "__main__":
    main()
