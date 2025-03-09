def calcular_media_funcional(numeros):

    soma = reduce(lambda x, y: x + y, numeros)
    return soma/len(numeros)

def calcular_media_imperativa(numeros):

    soma = 0
    for numero in numeros:
        soma += numero
    return soma/len(numeros)

numeros = [1, 2, 3, 4, 5]

media_funcional = calcular_media_funcional(numeros)
print("Media (funcional):", media_funcional)

media_imperativa = calcular_media_imperativa(numeros)
print("Media (imperativa):", media_imperativa)

# Calcula a média
# A função reduce soma todos os elementos da lista, e o resultado é dividido pelo número de elementos. 
# Essa abordagem é mais concisa e expressiva do que a abordagem imperativa, pois a função reduce encapsula a lógica de iteração e
# acumulação. Além disso, a programação funcional enfatiza a imutabilidade, o que
# contribui para a criação de código mais seguro e previsível.