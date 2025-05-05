import json

class GramaticaLC:
    def __init__(self, gramatica):
        self.gramatica = gramatica
        self.simbolos_nao_terminais = set(gramatica.keys())
        self.simbolos_terminais = self.get_terminais()

    def get_terminais(self):
        terminais = set()
        for producoes in self.gramatica.values():
            for producao in producoes:
                for simbolo in producao:
                    if simbolo.islower() and simbolo not in self.simbolos_nao_terminais:
                        terminais.add(simbolo)
        return terminais

    def simplificar_simbolos_inuteis(self):
        geradores = set()
        
        while True:
            novos_geradores = set()
            for nao_terminal, producoes in self.gramatica.items():
                for producao in producoes:
                    if all(simbolo in self.simbolos_terminais or simbolo in geradores for simbolo in producao):
                        novos_geradores.add(nao_terminal)
            if novos_geradores.issubset(geradores):
                break
            geradores.update(novos_geradores)

        self.gramatica = {
            nao_terminal: [
                producao for producao in producoes 
                if all(simbolo in self.simbolos_terminais or simbolo in geradores for simbolo in producao)
            ]
            for nao_terminal, producoes in self.gramatica.items()
            if nao_terminal in geradores
        }

    def simplificar_simbolos_inalcancaveis(self):
        alcancaveis = set('S')
        verificar = ['S']
        
        while verificar:
            nao_terminal = verificar.pop()
            for producao in self.gramatica.get(nao_terminal, []):
                for simbolo in producao:
                    if simbolo in self.simbolos_nao_terminais and simbolo not in alcancaveis:
                        alcancaveis.add(simbolo)
                        verificar.append(simbolo)

        self.gramatica = {
            nao_terminal: producoes
            for nao_terminal, producoes in self.gramatica.items()
            if nao_terminal in alcancaveis
        }

    def remover_producoes_vazias(self):
        producoes_vazias = set()
        
        for nao_terminal, producoes in self.gramatica.items():
            for producao in producoes:
                if producao == '':
                    producoes_vazias.add(nao_terminal)
        
        while True:
            novos_vazios = set()
            for nao_terminal, producoes in self.gramatica.items():
                for producao in producoes:
                    if all(simbolo in producoes_vazias for simbolo in producao):
                        novos_vazios.add(nao_terminal)
            if novos_vazios.issubset(producoes_vazias):
                break
            producoes_vazias.update(novos_vazios)
        
        novas_producoes = {}
        for nao_terminal, producoes in self.gramatica.items():
            novas_producoes[nao_terminal] = []
            for producao in producoes:
                if producao != '':
                    novas_producoes[nao_terminal].append(producao)
                if any(simbolo in producoes_vazias for simbolo in producao):
                    novas_producoes[nao_terminal].extend(self._producoes_derivadas(producao, producoes_vazias))
        
        self.gramatica = novas_producoes

    def _producoes_derivadas(self, producao, producoes_vazias):
        if not producao:
            return ['']
        
        derivadas = []
        for i, simbolo in enumerate(producao):
            if simbolo in producoes_vazias:
                sub_derivadas = self._producoes_derivadas(producao[:i] + producao[i+1:], producoes_vazias)
                derivadas.extend(sub_derivadas)
        derivadas.append(producao)
        
        return list(set(derivadas))

    def substituir_producoes(self):
        alterou = True
        while alterou:
            alterou = False
            novas_producoes = {}
            for nao_terminal, producoes in self.gramatica.items():
                novas_producoes[nao_terminal] = []
                for producao in producoes:
                    if len(producao) == 1 and producao in self.simbolos_nao_terminais:
                        alterou = True
                        novas_producoes[nao_terminal].extend(self.gramatica[producao])
                    else:
                        novas_producoes[nao_terminal].append(producao)
            self.gramatica = novas_producoes

def carregar_gramatica_de_json(caminho_do_arquivo):
    with open('entrada.json', 'r') as arquivo:
        gramatica = json.load(arquivo)
    return gramatica

def salvar_gramatica_em_json(gramatica, caminho_do_arquivo):
    with open('saida.json', 'w') as arquivo:
        json.dump(gramatica, arquivo, indent=4)

# Exemplo de uso
caminho_do_arquivo = 'entrada.json'
gramatica = carregar_gramatica_de_json(caminho_do_arquivo)

gramatica_lc = GramaticaLC(gramatica)

gramatica_lc.simplificar_simbolos_inuteis()

gramatica_lc.simplificar_simbolos_inalcancaveis()

gramatica_lc.remover_producoes_vazias()

gramatica_lc.substituir_producoes()

salvar_gramatica_em_json(gramatica_lc.gramatica, 'gramatica_simplificada.json')
